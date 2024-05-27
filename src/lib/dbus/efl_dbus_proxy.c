#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "efl_dbus_private.h"
#include "efl_dbus_private_types.h"

/* TODO: mempool of Efl_Dbus_Proxy, Efl_Dbus_Proxy_Context_Event_Cb and
 * Efl_Dbus_Proxy_Context_Event
 */

typedef struct _Efl_Dbus_Proxy_Context_Event_Cb
{
    EINA_INLIST;
    Efl_Dbus_Proxy_Event_Cb cb;
    const void             *cb_data;
    Efl_Bool                deleted : 1;
} Efl_Dbus_Proxy_Context_Event_Cb;

typedef struct _Efl_Dbus_Proxy_Context_Event
{
    Eina_Inlist *list;
    int          walking;
    Eina_List   *to_delete;
} Efl_Dbus_Proxy_Context_Event;

struct _Efl_Dbus_Proxy
{
    EINA_MAGIC;
    int                          refcount;
    Efl_Dbus_Object             *obj;
    const char                  *interface;
    Eina_Inlist                 *pendings;
    Eina_List                   *handlers;
    Eina_Inlist                 *cbs_free;
    Eina_Inlist                 *data;
    Efl_Dbus_Proxy_Context_Event event_handlers[EFL_DBUS_PROXY_EVENT_LAST];
    Eina_Hash                   *props;
    Efl_Dbus_Signal_Handler     *properties_changed;
    Efl_Bool                     monitor_enabled : 1;
};

#define EFL_DBUS_PROXY_CHECK(proxy)                         \
    do                                                      \
    {                                                       \
        EINA_SAFETY_ON_NULL_RETURN(proxy);                  \
        if (!EINA_MAGIC_CHECK(proxy, EFL_DBUS_PROXY_MAGIC)) \
        {                                                   \
            EINA_MAGIC_FAIL(proxy, EFL_DBUS_PROXY_MAGIC);   \
            return;                                         \
        }                                                   \
        EINA_SAFETY_ON_TRUE_RETURN(proxy->refcount <= 0);   \
    }                                                       \
    while (0)

#define EFL_DBUS_PROXY_CHECK_RETVAL(proxy, retval)                    \
    do                                                                \
    {                                                                 \
        EINA_SAFETY_ON_NULL_RETURN_VAL(proxy, retval);                \
        if (!EINA_MAGIC_CHECK(proxy, EFL_DBUS_PROXY_MAGIC))           \
        {                                                             \
            EINA_MAGIC_FAIL(proxy, EFL_DBUS_PROXY_MAGIC);             \
            return retval;                                            \
        }                                                             \
        EINA_SAFETY_ON_TRUE_RETURN_VAL(proxy->refcount <= 0, retval); \
    }                                                                 \
    while (0)

#define EFL_DBUS_PROXY_CHECK_GOTO(proxy, label)                \
    do                                                         \
    {                                                          \
        EINA_SAFETY_ON_NULL_GOTO(proxy, label);                \
        if (!EINA_MAGIC_CHECK(proxy, EFL_DBUS_PROXY_MAGIC))    \
        {                                                      \
            EINA_MAGIC_FAIL(proxy, EFL_DBUS_PROXY_MAGIC);      \
            goto label;                                        \
        }                                                      \
        EINA_SAFETY_ON_TRUE_GOTO(proxy->refcount <= 0, label); \
    }                                                          \
    while (0)

Efl_Bool
efl_dbus_proxy_init(void)
{
    return EFL_TRUE;
}

void
efl_dbus_proxy_shutdown(void)
{}

static void _efl_dbus_proxy_event_callback_call(Efl_Dbus_Proxy           *proxy,
                                                Efl_Dbus_Proxy_Event_Type type,
                                                const void *event_info);
static void
_efl_dbus_proxy_context_event_cb_del(Efl_Dbus_Proxy_Context_Event    *ce,
                                     Efl_Dbus_Proxy_Context_Event_Cb *ctx);
static void _on_signal_handler_free(void *data, const void *dead_pointer);

static void
_efl_dbus_proxy_call_del(Efl_Dbus_Proxy *proxy)
{
    Efl_Dbus_Proxy_Context_Event *ce;

    _efl_dbus_proxy_event_callback_call(proxy, EFL_DBUS_PROXY_EVENT_DEL, NULL);

   /* clear all del callbacks so we don't call them twice at
    * _efl_dbus_proxy_clear()
    */
    ce = proxy->event_handlers + EFL_DBUS_PROXY_EVENT_DEL;
    while (ce->list)
    {
        Efl_Dbus_Proxy_Context_Event_Cb *ctx;

        ctx = EINA_INLIST_CONTAINER_GET(ce->list,
                                        Efl_Dbus_Proxy_Context_Event_Cb);
        _efl_dbus_proxy_context_event_cb_del(ce, ctx);
    }
}

static void
_efl_dbus_proxy_clear(Efl_Dbus_Proxy *proxy)
{
    Efl_Dbus_Signal_Handler *h;
    Efl_Dbus_Pending        *p;
    Eina_List               *iter, *iter_next;
    Eina_Inlist             *in_l;
    DBG("proxy=%p, refcount=%d, interface=%s, obj=%p",
        proxy,
        proxy->refcount,
        proxy->interface,
        proxy->obj);
    proxy->refcount = 1;
    efl_dbus_object_proxy_del(proxy->obj, proxy, proxy->interface);
    _efl_dbus_proxy_call_del(proxy);

    EINA_LIST_FOREACH_SAFE(proxy->handlers, iter, iter_next, h)
    {
        DBG("proxy=%p delete owned signal handler %p %s",
            proxy,
            h,
            efl_dbus_signal_handler_match_get(h));
        efl_dbus_signal_handler_del(h);
    }

    EINA_INLIST_FOREACH_SAFE(proxy->pendings, in_l, p)
    {
        DBG("proxy=%p delete owned pending call=%p dest=%s path=%s %s.%s()",
            proxy,
            p,
            efl_dbus_pending_destination_get(p),
            efl_dbus_pending_path_get(p),
            efl_dbus_pending_interface_get(p),
            efl_dbus_pending_method_get(p));
        efl_dbus_pending_cancel(p);
    }

    efl_dbus_cbs_free_dispatch(&(proxy->cbs_free), proxy);
    if (proxy->props)
    {
        eina_hash_free(proxy->props);
        proxy->props = NULL;
    }
    proxy->refcount = 0;
}

static void
_efl_dbus_proxy_free(Efl_Dbus_Proxy *proxy)
{
    unsigned int             i;
    Efl_Dbus_Signal_Handler *h;

    DBG("freeing proxy=%p", proxy);
    EINA_LIST_FREE(proxy->handlers, h)
    {
        if (h->dangling)
            efl_dbus_signal_handler_free_cb_del(h,
                                                _on_signal_handler_free,
                                                proxy);
        else
            ERR("proxy=%p alive handler=%p %s",
                proxy,
                h,
                efl_dbus_signal_handler_match_get(h));
    }

    if (proxy->pendings)
        CRI("Proxy %p released with live pending calls!", proxy);

    for (i = 0; i < EFL_DBUS_PROXY_EVENT_LAST; i++)
    {
        Efl_Dbus_Proxy_Context_Event *ce = proxy->event_handlers + i;
        while (ce->list)
        {
            Efl_Dbus_Proxy_Context_Event_Cb *ctx;
            ctx = EINA_INLIST_CONTAINER_GET(ce->list,
                                            Efl_Dbus_Proxy_Context_Event_Cb);
            _efl_dbus_proxy_context_event_cb_del(ce, ctx);
        }
        eina_list_free(ce->to_delete);
    }

    eina_stringshare_del(proxy->interface);
    EINA_MAGIC_SET(proxy, EINA_MAGIC_NONE);
    free(proxy);
}

static void
_on_object_free(void *data, const void *dead_pointer EFL_UNUSED)
{
    Efl_Dbus_Proxy *proxy = data;
    EFL_DBUS_PROXY_CHECK(proxy);
    DBG("proxy=%p, refcount=%d, interface=%s, obj=%p",
        proxy,
        proxy->refcount,
        proxy->interface,
        proxy->obj);
    efl_dbus_data_del_all(&(proxy->data));
    _efl_dbus_proxy_clear(proxy);
    _efl_dbus_proxy_free(proxy);
}

EAPI Efl_Dbus_Proxy *
efl_dbus_proxy_get(Efl_Dbus_Object *obj, const char *interface)
{
    Efl_Dbus_Proxy *proxy;

    EINA_SAFETY_ON_NULL_RETURN_VAL(obj, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(interface, NULL);

    proxy = efl_dbus_object_proxy_get(obj, interface);
    if (proxy) return efl_dbus_proxy_ref(proxy);

    proxy = calloc(1, sizeof(Efl_Dbus_Proxy));
    EINA_SAFETY_ON_NULL_RETURN_VAL(proxy, NULL);

    proxy->refcount  = 1;
    proxy->obj       = obj;
    proxy->interface = eina_stringshare_add(interface);
    EINA_MAGIC_SET(proxy, EFL_DBUS_PROXY_MAGIC);
    if (!efl_dbus_object_proxy_add(obj, proxy)) goto cleanup;
    efl_dbus_object_free_cb_add(obj, _on_object_free, proxy);

    return proxy;

cleanup:
    eina_stringshare_del(proxy->interface);
    free(proxy);
    return NULL;
}

static void _on_signal_handler_free(void *data, const void *dead_pointer);

static void
_efl_dbus_proxy_unref(Efl_Dbus_Proxy *proxy)
{
    proxy->refcount--;
    if (proxy->refcount > 0) return;

    efl_dbus_object_free_cb_del(proxy->obj, _on_object_free, proxy);
    efl_dbus_data_del_all(&(proxy->data));
    _efl_dbus_proxy_clear(proxy);
    _efl_dbus_proxy_free(proxy);
}

EAPI Efl_Dbus_Proxy *
efl_dbus_proxy_ref(Efl_Dbus_Proxy *proxy)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    DBG("proxy=%p, pre-refcount=%d, interface=%s, obj=%p",
        proxy,
        proxy->refcount,
        proxy->interface,
        proxy->obj);
    proxy->refcount++;
    return proxy;
}

EAPI void
efl_dbus_proxy_unref(Efl_Dbus_Proxy *proxy)
{
    EFL_DBUS_PROXY_CHECK(proxy);
    DBG("proxy=%p, pre-refcount=%d, interface=%s, obj=%p",
        proxy,
        proxy->refcount,
        proxy->interface,
        proxy->obj);
    _efl_dbus_proxy_unref(proxy);
}

EAPI void
efl_dbus_proxy_free_cb_add(Efl_Dbus_Proxy  *proxy,
                           Efl_Dbus_Free_Cb cb,
                           const void      *data)
{
    EFL_DBUS_PROXY_CHECK(proxy);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    proxy->cbs_free = efl_dbus_cbs_free_add(proxy->cbs_free, cb, data);
}

EAPI void
efl_dbus_proxy_free_cb_del(Efl_Dbus_Proxy  *proxy,
                           Efl_Dbus_Free_Cb cb,
                           const void      *data)
{
    EFL_DBUS_PROXY_CHECK(proxy);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    proxy->cbs_free = efl_dbus_cbs_free_del(proxy->cbs_free, cb, data);
}

EAPI void
efl_dbus_proxy_data_set(Efl_Dbus_Proxy *proxy,
                        const char     *key,
                        const void     *data)
{
    EFL_DBUS_PROXY_CHECK(proxy);
    EINA_SAFETY_ON_NULL_RETURN(key);
    EINA_SAFETY_ON_NULL_RETURN(data);
    efl_dbus_data_set(&(proxy->data), key, data);
}

EAPI void *
efl_dbus_proxy_data_get(const Efl_Dbus_Proxy *proxy, const char *key)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(key, NULL);
    return efl_dbus_data_get(&(((Efl_Dbus_Proxy *)proxy)->data), key);
}

EAPI void *
efl_dbus_proxy_data_del(Efl_Dbus_Proxy *proxy, const char *key)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(key, NULL);
    return efl_dbus_data_del(&(((Efl_Dbus_Proxy *)proxy)->data), key);
}

static Eina_Value *
_iter_hash_value_set(Eina_Hash             *props,
                     const char            *key,
                     Efl_Dbus_Message_Iter *var) EINA_ARG_NONNULL(1, 2, 3)
{
    Eina_Value *st_value = _message_iter_struct_to_eina_value(var);
    Eina_Value *value;
    Eina_Value  stack_value;

    eina_value_struct_value_get(st_value, "arg0", &stack_value);

    value = eina_hash_find(props, key);
    if (!value)
    {
        value = eina_value_new(eina_value_type_get(&stack_value));
        eina_hash_add(props, key, value);
    }

    eina_value_flush(value);
    eina_value_copy(&stack_value, value);

    eina_value_flush(&stack_value);
    eina_value_free(st_value);
    return value;
}

static void
_property_changed_iter(void *data, const void *key, Efl_Dbus_Message_Iter *var)
{
    Efl_Dbus_Proxy *proxy = data;
    const char     *skey  = key;

    Eina_Value *value = _iter_hash_value_set(proxy->props, skey, var);

    Efl_Dbus_Proxy_Event_Property_Changed event = { .name  = skey,
                                                    .value = value,
                                                    .proxy = proxy };
    _efl_dbus_proxy_event_callback_call(proxy,
                                        EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED,
                                        &event);
}

static void
_properties_changed(void *data, const Efl_Dbus_Message *msg)
{
    Efl_Dbus_Proxy        *proxy = data;
    Efl_Dbus_Message_Iter *array, *invalidate;
    const char            *iface;
    const char            *invalidate_prop;

    if (!efl_dbus_message_arguments_get(msg,
                                        "sa{sv}as",
                                        &iface,
                                        &array,
                                        &invalidate))
    {
        ERR("Error getting data from properties changed signal.");
        return;
    }
    if (proxy->props)
        efl_dbus_message_iter_dict_iterate(array,
                                           "sv",
                                           _property_changed_iter,
                                           proxy);

    while (
        efl_dbus_message_iter_get_and_next(invalidate, 's', &invalidate_prop))
    {
        Efl_Dbus_Proxy_Event_Property_Removed event;
        event.interface = proxy->interface;
        event.name      = invalidate_prop;
        event.proxy     = proxy;
        if (proxy->props) eina_hash_del(proxy->props, event.name, NULL);
        _efl_dbus_proxy_event_callback_call(
            proxy,
            EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED,
            &event);
    }
}

static void
_props_cache_free(void *data)
{
    Eina_Value *value = data;
    eina_value_free(value);
}

EAPI void
efl_dbus_proxy_event_callback_add(Efl_Dbus_Proxy           *proxy,
                                  Efl_Dbus_Proxy_Event_Type type,
                                  Efl_Dbus_Proxy_Event_Cb   cb,
                                  const void               *cb_data)
{
    Efl_Dbus_Proxy_Context_Event    *ce;
    Efl_Dbus_Proxy_Context_Event_Cb *ctx;

    EFL_DBUS_PROXY_CHECK(proxy);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    EINA_SAFETY_ON_TRUE_RETURN(type >= EFL_DBUS_PROXY_EVENT_LAST);

    ce = proxy->event_handlers + type;

    ctx = calloc(1, sizeof(Efl_Dbus_Proxy_Context_Event_Cb));
    EINA_SAFETY_ON_NULL_RETURN(ctx);
    ctx->cb      = cb;
    ctx->cb_data = cb_data;

    ce->list = eina_inlist_append(ce->list, EINA_INLIST_GET(ctx));

    if (type == EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED)
    {
        if (proxy->properties_changed) return;
        if (!proxy->props)
            proxy->props = eina_hash_string_superfast_new(_props_cache_free);
        proxy->properties_changed =
            efl_dbus_proxy_properties_changed_callback_add(proxy,
                                                           _properties_changed,
                                                           proxy);
    }
    else if (type == EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED)
    {
        if (proxy->properties_changed) return;
        proxy->properties_changed =
            efl_dbus_proxy_properties_changed_callback_add(proxy,
                                                           _properties_changed,
                                                           proxy);
    }
}

static void
_efl_dbus_proxy_context_event_cb_del(Efl_Dbus_Proxy_Context_Event    *ce,
                                     Efl_Dbus_Proxy_Context_Event_Cb *ctx)
{
    ce->list = eina_inlist_remove(ce->list, EINA_INLIST_GET(ctx));
    free(ctx);
}

EAPI void
efl_dbus_proxy_event_callback_del(Efl_Dbus_Proxy           *proxy,
                                  Efl_Dbus_Proxy_Event_Type type,
                                  Efl_Dbus_Proxy_Event_Cb   cb,
                                  const void               *cb_data)
{
    Efl_Dbus_Proxy_Context_Event    *ce;
    Efl_Dbus_Proxy_Context_Event_Cb *iter, *found = NULL;

    EFL_DBUS_PROXY_CHECK(proxy);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    EINA_SAFETY_ON_TRUE_RETURN(type >= EFL_DBUS_PROXY_EVENT_LAST);

    ce = proxy->event_handlers + type;

    EINA_INLIST_FOREACH(ce->list, iter)
    {
        if (cb != iter->cb) continue;
        if ((cb_data) && (cb_data != iter->cb_data)) continue;

        found = iter;
        break;
    }

    EINA_SAFETY_ON_NULL_RETURN(found);
    EINA_SAFETY_ON_TRUE_RETURN(found->deleted);

    if (ce->walking)
    {
        found->deleted = EFL_TRUE;
        ce->to_delete  = eina_list_append(ce->to_delete, found);
        return;
    }

    _efl_dbus_proxy_context_event_cb_del(ce, found);

    if (type == EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED)
    {
        Efl_Dbus_Proxy_Context_Event *ce_prop_remove;
        ce_prop_remove =
            proxy->event_handlers + EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED;
        if (!ce->list && !proxy->monitor_enabled)
        {
            eina_hash_free(proxy->props);
            proxy->props = NULL;
        }

        if (!ce_prop_remove->list && !ce->list && !proxy->monitor_enabled)
        {
            efl_dbus_signal_handler_unref(proxy->properties_changed);
            proxy->properties_changed = NULL;
        }
    }
    else if (type == EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED)
    {
        Efl_Dbus_Proxy_Context_Event *ce_prop_changed;
        ce_prop_changed =
            proxy->event_handlers + EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED;

        if (!ce_prop_changed->list && !ce->list && !proxy->monitor_enabled)
        {
            efl_dbus_signal_handler_unref(proxy->properties_changed);
            proxy->properties_changed = NULL;
        }
    }
}

static void
_efl_dbus_proxy_event_callback_call(Efl_Dbus_Proxy           *proxy,
                                    Efl_Dbus_Proxy_Event_Type type,
                                    const void               *event_info)
{
    Efl_Dbus_Proxy_Context_Event    *ce;
    Efl_Dbus_Proxy_Context_Event_Cb *iter;

    ce = proxy->event_handlers + type;

    ce->walking++;
    EINA_INLIST_FOREACH(ce->list, iter)
    {
        if (iter->deleted) continue;
        iter->cb((void *)iter->cb_data, proxy, (void *)event_info);
    }
    ce->walking--;
    if (ce->walking > 0) return;

    EINA_LIST_FREE(ce->to_delete, iter)
    _efl_dbus_proxy_context_event_cb_del(ce, iter);
}

EAPI Efl_Dbus_Object *
efl_dbus_proxy_object_get(const Efl_Dbus_Proxy *proxy)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    return proxy->obj;
}

EAPI const char *
efl_dbus_proxy_interface_get(const Efl_Dbus_Proxy *proxy)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    return proxy->interface;
}

static void
_on_proxy_message_cb(void                   *data,
                     const Efl_Dbus_Message *msg,
                     Efl_Dbus_Pending       *pending)
{
    Efl_Dbus_Message_Cb cb    = efl_dbus_pending_data_del(pending, "__user_cb");
    Efl_Dbus_Proxy     *proxy = efl_dbus_pending_data_del(pending, "__proxy");

    EFL_DBUS_PROXY_CHECK(proxy);
    proxy->pendings =
        eina_inlist_remove(proxy->pendings, EINA_INLIST_GET(pending));
    cb(data, msg, pending);
}

static Efl_Dbus_Pending *
_efl_dbus_proxy_send(Efl_Dbus_Proxy     *proxy,
                     Efl_Dbus_Message   *msg,
                     Efl_Dbus_Message_Cb cb,
                     const void         *cb_data,
                     double              timeout)
{
    Efl_Dbus_Pending *pending;

    if (!cb)
    {
        _efl_dbus_connection_send(proxy->obj->conn, msg, NULL, NULL, timeout);
        return NULL;
    }
    pending = _efl_dbus_connection_send(proxy->obj->conn,
                                        msg,
                                        _on_proxy_message_cb,
                                        cb_data,
                                        timeout);
    EINA_SAFETY_ON_NULL_RETURN_VAL(pending, NULL);

    efl_dbus_pending_data_set(pending, "__user_cb", cb);
    efl_dbus_pending_data_set(pending, "__proxy", proxy);
    proxy->pendings =
        eina_inlist_append(proxy->pendings, EINA_INLIST_GET(pending));

    return pending;
}

static Efl_Dbus_Message *
_efl_dbus_proxy_send_and_block(Efl_Dbus_Proxy   *proxy,
                               Efl_Dbus_Message *msg,
                               double            timeout)
{
    return _efl_dbus_connection_send_and_block(proxy->obj->conn, msg, timeout);
}

EAPI Efl_Dbus_Pending *
efl_dbus_proxy_send(Efl_Dbus_Proxy     *proxy,
                    Efl_Dbus_Message   *msg,
                    Efl_Dbus_Message_Cb cb,
                    const void         *cb_data,
                    double              timeout)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(msg, NULL);

    return _efl_dbus_proxy_send(proxy, msg, cb, cb_data, timeout);
}

EAPI Efl_Dbus_Message *
efl_dbus_proxy_send_and_block(Efl_Dbus_Proxy   *proxy,
                              Efl_Dbus_Message *msg,
                              double            timeout)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(msg, NULL);

    return _efl_dbus_proxy_send_and_block(proxy, msg, timeout);
}

EAPI Efl_Dbus_Message *
efl_dbus_proxy_method_call_new(Efl_Dbus_Proxy *proxy, const char *member)
{
    Efl_Dbus_Message *msg;
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);

    msg = efl_dbus_message_method_call_new(
        efl_dbus_object_bus_name_get(proxy->obj),
        efl_dbus_object_path_get(proxy->obj),
        proxy->interface,
        member);
    return msg;
}

static Efl_Dbus_Pending *
_efl_dbus_proxy_vcall(Efl_Dbus_Proxy     *proxy,
                      const char         *member,
                      Efl_Dbus_Message_Cb cb,
                      const void         *cb_data,
                      double              timeout,
                      const char         *signature,
                      va_list             ap)
{
    Efl_Dbus_Message *msg = efl_dbus_proxy_method_call_new(proxy, member);
    EINA_SAFETY_ON_NULL_RETURN_VAL(msg, NULL);

    if (!efl_dbus_message_arguments_vappend(msg, signature, ap))
    {
        efl_dbus_message_unref(msg);
        ERR("Error setting arguments");
        return NULL;
    }

    return _efl_dbus_proxy_send(proxy, msg, cb, cb_data, timeout);
}

EAPI Efl_Dbus_Pending *
efl_dbus_proxy_call(Efl_Dbus_Proxy     *proxy,
                    const char         *member,
                    Efl_Dbus_Message_Cb cb,
                    const void         *cb_data,
                    double              timeout,
                    const char         *signature,
                    ...)
{
    Efl_Dbus_Pending *pending;
    va_list           ap;

    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(member, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(signature, NULL);

    va_start(ap, signature);
    pending = _efl_dbus_proxy_vcall(proxy,
                                    member,
                                    cb,
                                    cb_data,
                                    timeout,
                                    signature,
                                    ap);
    va_end(ap);

    return pending;
}

EAPI Efl_Dbus_Pending *
efl_dbus_proxy_vcall(Efl_Dbus_Proxy     *proxy,
                     const char         *member,
                     Efl_Dbus_Message_Cb cb,
                     const void         *cb_data,
                     double              timeout,
                     const char         *signature,
                     va_list             ap)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(member, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(signature, NULL);

    return _efl_dbus_proxy_vcall(proxy,
                                 member,
                                 cb,
                                 cb_data,
                                 timeout,
                                 signature,
                                 ap);
}

static void
_on_signal_handler_free(void *data, const void *dead_pointer)
{
    Efl_Dbus_Proxy *proxy = data;
    EFL_DBUS_PROXY_CHECK(proxy);
    proxy->handlers = eina_list_remove(proxy->handlers, dead_pointer);
}

EAPI Efl_Dbus_Signal_Handler *
efl_dbus_proxy_signal_handler_add(Efl_Dbus_Proxy    *proxy,
                                  const char        *member,
                                  Efl_Dbus_Signal_Cb cb,
                                  const void        *cb_data)
{
    Efl_Dbus_Signal_Handler *handler;
    const char              *name, *path;

    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(cb, NULL);

    name = efl_dbus_object_bus_name_get(proxy->obj);
    path = efl_dbus_object_path_get(proxy->obj);

    handler = _efl_dbus_signal_handler_add(proxy->obj->conn,
                                           name,
                                           path,
                                           proxy->interface,
                                           member,
                                           cb,
                                           cb_data);
    EINA_SAFETY_ON_NULL_RETURN_VAL(handler, NULL);
    DBG("signal handler added: proxy=%p handler=%p cb=%p", proxy, handler, cb);

    efl_dbus_signal_handler_free_cb_add(handler,
                                        _on_signal_handler_free,
                                        proxy);
    proxy->handlers = eina_list_append(proxy->handlers, handler);

    return handler;
}

EAPI Efl_Dbus_Pending *
efl_dbus_proxy_property_get(Efl_Dbus_Proxy     *proxy,
                            const char         *name,
                            Efl_Dbus_Message_Cb cb,
                            const void         *data)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);
    return efl_dbus_proxy_call(proxy->obj->properties,
                               "Get",
                               cb,
                               data,
                               -1,
                               "ss",
                               proxy->interface,
                               name);
}

static inline Efl_Bool
_type_is_number(char sig)
{
    switch (sig)
    {
        case 'y':
        case 'b':
        case 'n':
        case 'q':
        case 'i':
        case 'u':
        case 'x':
        case 't':
        case 'd':
        case 'h':
            return EFL_TRUE;
        default:
            break;
    }
    return EFL_FALSE;
}

EAPI Efl_Dbus_Pending *
efl_dbus_proxy_property_set(Efl_Dbus_Proxy     *proxy,
                            const char         *name,
                            const char         *sig,
                            const void         *value,
                            Efl_Dbus_Message_Cb cb,
                            const void         *data)
{
    Efl_Dbus_Message      *msg;
    Efl_Dbus_Message_Iter *iter, *variant;

    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(sig, NULL);
    EINA_SAFETY_ON_FALSE_RETURN_VAL(dbus_signature_validate_single(sig, NULL),
                                    NULL);
    EINA_SAFETY_ON_FALSE_RETURN_VAL((_type_is_number(sig[0]) || value), NULL);

    msg  = efl_dbus_proxy_method_call_new(proxy->obj->properties, "Set");
    iter = efl_dbus_message_iter_get(msg);
    efl_dbus_message_iter_basic_append(iter, 's', proxy->interface);
    efl_dbus_message_iter_basic_append(iter, 's', name);
    variant = efl_dbus_message_iter_container_new(iter, 'v', sig);
    if (dbus_type_is_basic(sig[0]))
        dbus_message_iter_append_basic(&variant->dbus_iterator, sig[0], &value);
    else
    {
        if (!_message_iter_from_eina_value_struct(sig, variant, value))
        {
            efl_dbus_message_unref(msg);
            return NULL;
        }
    }
    efl_dbus_message_iter_container_close(iter, variant);

    return efl_dbus_proxy_send(proxy->obj->properties, msg, cb, data, -1);
}

EAPI Efl_Dbus_Pending *
efl_dbus_proxy_property_value_set(Efl_Dbus_Proxy     *proxy,
                                  const char         *name,
                                  const char         *sig,
                                  const Eina_Value   *value,
                                  Efl_Dbus_Message_Cb cb,
                                  const void         *data)
{
    Efl_Dbus_Message      *msg;
    Efl_Dbus_Message_Iter *iter, *variant;

    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(sig, NULL);
    EINA_SAFETY_ON_FALSE_RETURN_VAL(dbus_signature_validate_single(sig, NULL),
                                    NULL);
    EINA_SAFETY_ON_FALSE_RETURN_VAL((_type_is_number(sig[0]) || value), NULL);

    msg  = efl_dbus_proxy_method_call_new(proxy->obj->properties, "Set");
    iter = efl_dbus_message_iter_get(msg);
    efl_dbus_message_iter_basic_append(iter, 's', proxy->interface);
    efl_dbus_message_iter_basic_append(iter, 's', name);
    variant = efl_dbus_message_iter_container_new(iter, 'v', sig);
    if (dbus_type_is_basic(sig[0]))
    {
        if (!_message_iter_from_eina_value(sig, variant, value)) goto error;
    }
    else
    {
        if (!_message_iter_from_eina_value_struct(sig, variant, value))
            goto error;
    }
    efl_dbus_message_iter_container_close(iter, variant);

    return efl_dbus_proxy_send(proxy->obj->properties, msg, cb, data, -1);

error:
    efl_dbus_message_unref(msg);
    return NULL;
}

EAPI Efl_Dbus_Pending *
efl_dbus_proxy_property_get_all(Efl_Dbus_Proxy     *proxy,
                                Efl_Dbus_Message_Cb cb,
                                const void         *data)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    return efl_dbus_proxy_call(proxy->obj->properties,
                               "GetAll",
                               cb,
                               data,
                               -1,
                               "s",
                               proxy->interface);
}

EAPI Efl_Dbus_Signal_Handler *
efl_dbus_proxy_properties_changed_callback_add(Efl_Dbus_Proxy    *proxy,
                                               Efl_Dbus_Signal_Cb cb,
                                               const void        *data)
{
    Efl_Dbus_Signal_Handler *sh;
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    sh = efl_dbus_proxy_signal_handler_add(proxy->obj->properties,
                                           "PropertiesChanged",
                                           cb,
                                           data);
    EINA_SAFETY_ON_NULL_RETURN_VAL(sh, NULL);
    efl_dbus_signal_handler_match_extra_set(sh, "arg0", proxy->interface, NULL);
    return sh;
}

static void
_property_iter(void *data, const void *key, Efl_Dbus_Message_Iter *var)
{
    Efl_Dbus_Proxy *proxy = data;
    const char     *skey  = key;

    _iter_hash_value_set(proxy->props, skey, var);
}

static void
_on_monitored_proxy_del(void                 *data,
                        Efl_Dbus_Proxy *proxy EFL_UNUSED,
                        void *event_info      EFL_UNUSED)
{
    Efl_Dbus_Pending *pending = data;
    efl_dbus_pending_cancel(pending);
}

static void
_props_get_all(void                   *data,
               const Efl_Dbus_Message *msg,
               Efl_Dbus_Pending       *pending)
{
    Efl_Dbus_Proxy                      *proxy = data;
    Efl_Dbus_Message_Iter               *dict;
    const char                          *name, *error_msg;
    Efl_Dbus_Proxy_Event_Property_Loaded event;

    efl_dbus_proxy_event_callback_del(proxy,
                                      EFL_DBUS_PROXY_EVENT_DEL,
                                      _on_monitored_proxy_del,
                                      pending);

    if (efl_dbus_message_error_get(msg, &name, &error_msg))
    {
        /* don't print warnings for user-canceled calls */
        if (!eina_streq(name, "org.enlightenment.DBus.Canceled"))
            WRN("Error getting all properties of %s %s, error message: %s %s",
                proxy->obj->name,
                proxy->obj->path,
                name,
                error_msg);
        return;
    }

    if (!efl_dbus_message_arguments_get(msg, "a{sv}", &dict))
    {
        char *txt;

        if (efl_dbus_message_arguments_get(msg, "s", &txt))
            WRN("Error getting data from properties getAll: %s", txt);
        return;
    }
    efl_dbus_message_iter_dict_iterate(dict, "sv", _property_iter, proxy);

    event.proxy = proxy;
    _efl_dbus_proxy_event_callback_call(proxy,
                                        EFL_DBUS_PROXY_EVENT_PROPERTY_LOADED,
                                        &event);
}

EAPI Efl_Bool
efl_dbus_proxy_properties_monitor(Efl_Dbus_Proxy *proxy, Efl_Bool enable)
{
    Efl_Dbus_Pending *pending;
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, EFL_FALSE);
    if (proxy->monitor_enabled == enable)
        return proxy->props ? !!eina_hash_population(proxy->props) : EFL_FALSE;

    proxy->monitor_enabled = enable;
    if (!enable)
    {
        Efl_Dbus_Proxy_Context_Event *ce_prop_changed, *ce_prop_removed;
        ce_prop_changed =
            proxy->event_handlers + EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED;
        ce_prop_removed =
            proxy->event_handlers + EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED;

        if (!ce_prop_changed->list)
        {
            eina_hash_free(proxy->props);
            proxy->props = NULL;
        }
        if (!ce_prop_changed->list && !ce_prop_removed->list)
        {
            efl_dbus_signal_handler_unref(proxy->properties_changed);
            proxy->properties_changed = NULL;
        }
        return EFL_TRUE;
    }

    if (!proxy->props)
        proxy->props = eina_hash_string_superfast_new(_props_cache_free);

    pending = efl_dbus_proxy_property_get_all(proxy, _props_get_all, proxy);
    efl_dbus_proxy_event_callback_add(proxy,
                                      EFL_DBUS_PROXY_EVENT_DEL,
                                      _on_monitored_proxy_del,
                                      pending);

    if (proxy->properties_changed) return !!eina_hash_population(proxy->props);
    proxy->properties_changed =
        efl_dbus_proxy_properties_changed_callback_add(proxy,
                                                       _properties_changed,
                                                       proxy);
    return !!eina_hash_population(proxy->props);
}

EAPI Eina_Value *
efl_dbus_proxy_property_local_get(Efl_Dbus_Proxy *proxy, const char *name)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(proxy->props, NULL);
    return eina_hash_find(proxy->props, name);
}

EAPI const Eina_Hash *
efl_dbus_proxy_property_local_get_all(Efl_Dbus_Proxy *proxy)
{
    EFL_DBUS_PROXY_CHECK_RETVAL(proxy, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(proxy->props, NULL);
    return proxy->props;
}
