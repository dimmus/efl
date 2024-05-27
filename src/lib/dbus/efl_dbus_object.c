#include "efl_dbus_private.h"
#include "efl_dbus_private_types.h"
#include <dbus/dbus.h>

/* TODO: mempool of Efl_Dbus_Object, Efl_Dbus_Object_Context_Event_Cb and
 * Efl_Dbus_Object_Context_Event
 */

#define EFL_DBUS_OBJECT_CHECK(obj)                         \
    do                                                     \
    {                                                      \
        EINA_SAFETY_ON_NULL_RETURN(obj);                   \
        if (!EINA_MAGIC_CHECK(obj, EFL_DBUS_OBJECT_MAGIC)) \
        {                                                  \
            EINA_MAGIC_FAIL(obj, EFL_DBUS_OBJECT_MAGIC);   \
            return;                                        \
        }                                                  \
        EINA_SAFETY_ON_TRUE_RETURN(obj->refcount <= 0);    \
    }                                                      \
    while (0)

#define EFL_DBUS_OBJECT_CHECK_RETVAL(obj, retval)                   \
    do                                                              \
    {                                                               \
        EINA_SAFETY_ON_NULL_RETURN_VAL(obj, retval);                \
        if (!EINA_MAGIC_CHECK(obj, EFL_DBUS_OBJECT_MAGIC))          \
        {                                                           \
            EINA_MAGIC_FAIL(obj, EFL_DBUS_OBJECT_MAGIC);            \
            return retval;                                          \
        }                                                           \
        EINA_SAFETY_ON_TRUE_RETURN_VAL(obj->refcount <= 0, retval); \
    }                                                               \
    while (0)

#define EFL_DBUS_OBJECT_CHECK_GOTO(obj, label)               \
    do                                                       \
    {                                                        \
        EINA_SAFETY_ON_NULL_GOTO(obj, label);                \
        if (!EINA_MAGIC_CHECK(obj, EFL_DBUS_OBJECT_MAGIC))   \
        {                                                    \
            EINA_MAGIC_FAIL(obj, EFL_DBUS_OBJECT_MAGIC);     \
            goto label;                                      \
        }                                                    \
        EINA_SAFETY_ON_TRUE_GOTO(obj->refcount <= 0, label); \
    }                                                        \
    while (0)

Efl_Bool
efl_dbus_object_init(void)
{
    return EFL_TRUE;
}

void
efl_dbus_object_shutdown(void)
{}

static void
_efl_dbus_object_event_callback_call(Efl_Dbus_Object           *obj,
                                     Efl_Dbus_Object_Event_Type type,
                                     const void                *event_info);
static void
_efl_dbus_object_context_event_cb_del(Efl_Dbus_Object_Context_Event    *ce,
                                      Efl_Dbus_Object_Context_Event_Cb *ctx);
static void _on_connection_free(void *data, const void *dead_pointer);
static void _on_signal_handler_free(void *data, const void *dead_pointer);

static void
_efl_dbus_object_call_del(Efl_Dbus_Object *obj)
{
    Efl_Dbus_Object_Context_Event *ce;

    _efl_dbus_object_event_callback_call(obj, EFL_DBUS_OBJECT_EVENT_DEL, NULL);

   /* clear all del callbacks so we don't call them twice at
    * _efl_dbus_object_clear()
    */
    ce = obj->event_handlers + EFL_DBUS_OBJECT_EVENT_DEL;
    while (ce->list)
    {
        Efl_Dbus_Object_Context_Event_Cb *ctx;

        ctx = EINA_INLIST_CONTAINER_GET(ce->list,
                                        Efl_Dbus_Object_Context_Event_Cb);
        _efl_dbus_object_context_event_cb_del(ce, ctx);
    }
}

static void
_efl_dbus_object_clear(Efl_Dbus_Object *obj)
{
    Efl_Dbus_Signal_Handler *h;
    Efl_Dbus_Pending        *p;
    Eina_List               *iter, *iter_next;
    Eina_Inlist             *in_l;
    DBG("obj=%p, refcount=%d, name=%s, path=%s",
        obj,
        obj->refcount,
        obj->name,
        obj->path);

    obj->refcount = 1;
    _efl_dbus_object_call_del(obj);
    efl_dbus_connection_name_object_del(obj->conn, obj);

   /* NOTE: obj->proxies is deleted from obj->cbs_free. */

    EINA_LIST_FOREACH_SAFE(obj->signal_handlers, iter, iter_next, h)
    {
        DBG("obj=%p delete owned signal handler %p %s",
            obj,
            h,
            efl_dbus_signal_handler_match_get(h));
        efl_dbus_signal_handler_del(h);
    }
    EINA_INLIST_FOREACH_SAFE(obj->pendings, in_l, p)
    {
        DBG("obj=%p delete owned pending call=%p dest=%s path=%s %s.%s()",
            obj,
            p,
            efl_dbus_pending_destination_get(p),
            efl_dbus_pending_path_get(p),
            efl_dbus_pending_interface_get(p),
            efl_dbus_pending_method_get(p));
        efl_dbus_pending_cancel(p);
    }

    efl_dbus_cbs_free_dispatch(&(obj->cbs_free), obj);
    obj->refcount = 0;
}

static void
_efl_dbus_object_free(Efl_Dbus_Object *obj)
{
    unsigned int             i;
    Efl_Dbus_Signal_Handler *h;

    if (obj->proxies)
    {
        Eina_Iterator  *iterator = eina_hash_iterator_data_new(obj->proxies);
        Efl_Dbus_Proxy *proxy;
        EINA_ITERATOR_FOREACH(iterator, proxy)
            ERR("obj=%p alive proxy=%p %s",
                obj,
                proxy,
                efl_dbus_proxy_interface_get(proxy));
        eina_iterator_free(iterator);
        eina_hash_free(obj->proxies);
    }

    EINA_LIST_FREE(obj->signal_handlers, h)
    {
        if (h->dangling)
            efl_dbus_signal_handler_free_cb_del(h,
                                                _on_signal_handler_free,
                                                obj);
        else
            ERR("obj=%p alive handler=%p %s",
                obj,
                h,
                efl_dbus_signal_handler_match_get(h));
    }

    if (obj->pendings) CRI("Object %p released with live pending calls!", obj);

    for (i = 0; i < EFL_DBUS_OBJECT_EVENT_LAST; i++)
    {
        Efl_Dbus_Object_Context_Event *ce = obj->event_handlers + i;
        while (ce->list)
        {
            Efl_Dbus_Object_Context_Event_Cb *ctx;

            ctx = EINA_INLIST_CONTAINER_GET(ce->list,
                                            Efl_Dbus_Object_Context_Event_Cb);
            _efl_dbus_object_context_event_cb_del(ce, ctx);
        }
        eina_list_free(ce->to_delete);
    }

    if (obj->interfaces_added)
        efl_dbus_signal_handler_del(obj->interfaces_added);
    if (obj->interfaces_removed)
        efl_dbus_signal_handler_del(obj->interfaces_removed);
    if (obj->properties_changed)
        efl_dbus_signal_handler_del(obj->properties_changed);
    eina_stringshare_del(obj->name);
    eina_stringshare_del(obj->path);
    EINA_MAGIC_SET(obj, EINA_MAGIC_NONE);

    free(obj);
}

static void
_on_connection_free(void *data, const void *dead_pointer EFL_UNUSED)
{
    Efl_Dbus_Object *obj = data;
    EFL_DBUS_OBJECT_CHECK(obj);
    _efl_dbus_object_clear(obj);
    _efl_dbus_object_free(obj);
}

EAPI Efl_Dbus_Object *
efl_dbus_object_get(Efl_Dbus_Connection *conn,
                    const char          *bus,
                    const char          *path)
{
    Efl_Dbus_Object *obj;

    EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(bus, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(path, NULL);

    obj = efl_dbus_connection_name_object_get(conn, bus, path);
    if (obj) return efl_dbus_object_ref(obj);

    obj = calloc(1, sizeof(Efl_Dbus_Object));
    EINA_SAFETY_ON_NULL_RETURN_VAL(obj, NULL);

    obj->conn     = conn;
    obj->refcount = 1;
    obj->path     = eina_stringshare_add(path);
    obj->name     = eina_stringshare_add(bus);
    obj->proxies  = eina_hash_string_small_new(NULL);
    EINA_SAFETY_ON_NULL_GOTO(obj->proxies, cleanup);
    EINA_MAGIC_SET(obj, EFL_DBUS_OBJECT_MAGIC);

    efl_dbus_connection_name_object_set(conn, obj);
    efl_dbus_connection_free_cb_add(obj->conn, _on_connection_free, obj);

    obj->properties =
        efl_dbus_proxy_get(obj, EFL_DBUS_FDO_INTERFACE_PROPERTIES);

    return obj;

cleanup:
    eina_stringshare_del(obj->path);
    eina_stringshare_del(obj->name);
    free(obj);

    return NULL;
}

static void _on_signal_handler_free(void *data, const void *dead_pointer);

static void
_efl_dbus_object_unref(Efl_Dbus_Object *obj)
{
    obj->refcount--;
    if (obj->refcount > 0) return;

    efl_dbus_connection_free_cb_del(obj->conn, _on_connection_free, obj);
    _efl_dbus_object_clear(obj);
    _efl_dbus_object_free(obj);
}

EAPI Efl_Dbus_Object *
efl_dbus_object_ref(Efl_Dbus_Object *obj)
{
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    DBG("obj=%p, pre-refcount=%d, name=%s, path=%s",
        obj,
        obj->refcount,
        obj->name,
        obj->path);
    obj->refcount++;
    return obj;
}

EAPI void
efl_dbus_object_unref(Efl_Dbus_Object *obj)
{
    EFL_DBUS_OBJECT_CHECK(obj);
    DBG("obj=%p, pre-refcount=%d, name=%s, path=%s",
        obj,
        obj->refcount,
        obj->name,
        obj->path);
    _efl_dbus_object_unref(obj);
}

EAPI void
efl_dbus_object_free_cb_add(Efl_Dbus_Object *obj,
                            Efl_Dbus_Free_Cb cb,
                            const void      *data)
{
    EFL_DBUS_OBJECT_CHECK(obj);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    obj->cbs_free = efl_dbus_cbs_free_add(obj->cbs_free, cb, data);
}

EAPI void
efl_dbus_object_free_cb_del(Efl_Dbus_Object *obj,
                            Efl_Dbus_Free_Cb cb,
                            const void      *data)
{
    EFL_DBUS_OBJECT_CHECK(obj);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    obj->cbs_free = efl_dbus_cbs_free_del(obj->cbs_free, cb, data);
}

static void
_cb_interfaces_added(void *data, const Efl_Dbus_Message *msg)
{
    Efl_Dbus_Object       *obj = data;
    const char            *obj_path;
    Efl_Dbus_Message_Iter *array_ifaces, *entry_iface;

    if (!efl_dbus_message_arguments_get(msg,
                                        "oa{sa{sv}}",
                                        &obj_path,
                                        &array_ifaces))
        return;

    while (efl_dbus_message_iter_get_and_next(array_ifaces, 'e', &entry_iface))
    {
        const char                           *iface_name;
        Efl_Dbus_Object_Event_Interface_Added event;

        efl_dbus_message_iter_basic_get(entry_iface, &iface_name);
        event.proxy = efl_dbus_proxy_get(obj, iface_name);
        EINA_SAFETY_ON_NULL_RETURN(event.proxy);
        event.interface = iface_name;
        _efl_dbus_object_event_callback_call(obj,
                                             EFL_DBUS_OBJECT_EVENT_IFACE_ADDED,
                                             &event);
        efl_dbus_proxy_unref(event.proxy);
    }
}

static void
_cb_interfaces_removed(void *data, const Efl_Dbus_Message *msg)
{
    Efl_Dbus_Object       *obj = data;
    const char            *obj_path, *iface;
    Efl_Dbus_Message_Iter *array_ifaces;

    if (!efl_dbus_message_arguments_get(msg, "oas", &obj_path, &array_ifaces))
        return;

    while (efl_dbus_message_iter_get_and_next(array_ifaces, 's', &iface))
    {
        Efl_Dbus_Object_Event_Interface_Removed event;
        event.interface = iface;
        _efl_dbus_object_event_callback_call(
            obj,
            EFL_DBUS_OBJECT_EVENT_IFACE_REMOVED,
            &event);
    }
}

static void
_property_changed_iter(void *data, const void *key, Efl_Dbus_Message_Iter *var)
{
    Efl_Dbus_Proxy                        *proxy = data;
    const char                            *skey  = key;
    Eina_Value                            *st_value, stack_value;
    Efl_Dbus_Object_Event_Property_Changed event;

    st_value = _message_iter_struct_to_eina_value(var);
    eina_value_struct_value_get(st_value, "arg0", &stack_value);

    event.interface = efl_dbus_proxy_interface_get(proxy);
    event.proxy     = proxy;
    event.name      = skey;
    event.value     = &stack_value;
    _efl_dbus_object_event_callback_call(efl_dbus_proxy_object_get(proxy),
                                         EFL_DBUS_OBJECT_EVENT_PROPERTY_CHANGED,
                                         &event);
    eina_value_free(st_value);
    eina_value_flush(&stack_value);
}

static void
_cb_properties_changed(void *data, const Efl_Dbus_Message *msg)
{
    Efl_Dbus_Object       *obj = data;
    Efl_Dbus_Proxy        *proxy;
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

    proxy = efl_dbus_proxy_get(obj, iface);
    EINA_SAFETY_ON_NULL_RETURN(proxy);

    if (obj->event_handlers[EFL_DBUS_OBJECT_EVENT_PROPERTY_CHANGED].list)
        efl_dbus_message_iter_dict_iterate(array,
                                           "sv",
                                           _property_changed_iter,
                                           proxy);

    if (!obj->event_handlers[EFL_DBUS_OBJECT_EVENT_PROPERTY_REMOVED].list)
        goto end;

    while (
        efl_dbus_message_iter_get_and_next(invalidate, 's', &invalidate_prop))
    {
        Efl_Dbus_Object_Event_Property_Removed event;
        event.interface = iface;
        event.name      = invalidate_prop;
        event.proxy     = proxy;
        _efl_dbus_object_event_callback_call(
            obj,
            EFL_DBUS_OBJECT_EVENT_PROPERTY_REMOVED,
            &event);
    }
end:
    efl_dbus_proxy_unref(proxy);
}

EAPI void
efl_dbus_object_event_callback_add(Efl_Dbus_Object           *obj,
                                   Efl_Dbus_Object_Event_Type type,
                                   Efl_Dbus_Object_Event_Cb   cb,
                                   const void                *cb_data)
{
    Efl_Dbus_Object_Context_Event    *ce;
    Efl_Dbus_Object_Context_Event_Cb *ctx;

    EFL_DBUS_OBJECT_CHECK(obj);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    EINA_SAFETY_ON_TRUE_RETURN(type >= EFL_DBUS_OBJECT_EVENT_LAST);

    ce = obj->event_handlers + type;

    ctx = calloc(1, sizeof(Efl_Dbus_Object_Context_Event_Cb));
    EINA_SAFETY_ON_NULL_RETURN(ctx);

    ctx->cb      = cb;
    ctx->cb_data = cb_data;

    ce->list = eina_inlist_append(ce->list, EINA_INLIST_GET(ctx));

    switch (type)
    {
        case EFL_DBUS_OBJECT_EVENT_IFACE_ADDED:
            {
                if (obj->interfaces_added) break;
                obj->interfaces_added = _efl_dbus_signal_handler_add(
                    obj->conn,
                    obj->name,
                    NULL,
                    EFL_DBUS_FDO_INTERFACE_OBJECT_MANAGER,
                    "InterfacesAdded",
                    _cb_interfaces_added,
                    obj);
                EINA_SAFETY_ON_NULL_RETURN(obj->interfaces_added);
                efl_dbus_signal_handler_match_extra_set(obj->interfaces_added,
                                                        "arg0",
                                                        obj->path,
                                                        NULL);
                break;
            }
        case EFL_DBUS_OBJECT_EVENT_IFACE_REMOVED:
            {
                if (obj->interfaces_removed) break;
                obj->interfaces_removed = _efl_dbus_signal_handler_add(
                    obj->conn,
                    obj->name,
                    NULL,
                    EFL_DBUS_FDO_INTERFACE_OBJECT_MANAGER,
                    "InterfacesRemoved",
                    _cb_interfaces_removed,
                    obj);
                EINA_SAFETY_ON_NULL_RETURN(obj->interfaces_removed);
                efl_dbus_signal_handler_match_extra_set(obj->interfaces_removed,
                                                        "arg0",
                                                        obj->path,
                                                        NULL);
                break;
            }
        case EFL_DBUS_OBJECT_EVENT_PROPERTY_CHANGED:
        case EFL_DBUS_OBJECT_EVENT_PROPERTY_REMOVED:
            {
                if (obj->properties_changed) break;
                obj->properties_changed = efl_dbus_object_signal_handler_add(
                    obj,
                    EFL_DBUS_FDO_INTERFACE_PROPERTIES,
                    "PropertiesChanged",
                    _cb_properties_changed,
                    obj);
                EINA_SAFETY_ON_NULL_RETURN(obj->properties_changed);
                break;
            }
        default:
            break;
    }
}

static void
_efl_dbus_object_context_event_cb_del(Efl_Dbus_Object_Context_Event    *ce,
                                      Efl_Dbus_Object_Context_Event_Cb *ctx)
{
    ce->list = eina_inlist_remove(ce->list, EINA_INLIST_GET(ctx));
    free(ctx);
}

EAPI void
efl_dbus_object_event_callback_del(Efl_Dbus_Object           *obj,
                                   Efl_Dbus_Object_Event_Type type,
                                   Efl_Dbus_Object_Event_Cb   cb,
                                   const void                *cb_data)
{
    Efl_Dbus_Object_Context_Event    *ce;
    Efl_Dbus_Object_Context_Event_Cb *iter, *found = NULL;

    EFL_DBUS_OBJECT_CHECK(obj);
    EINA_SAFETY_ON_NULL_RETURN(cb);
    EINA_SAFETY_ON_TRUE_RETURN(type >= EFL_DBUS_OBJECT_EVENT_LAST);

    ce = obj->event_handlers + type;

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

    _efl_dbus_object_context_event_cb_del(ce, found);

    switch (type)
    {
        case EFL_DBUS_OBJECT_EVENT_IFACE_ADDED:
            {
                if (obj->event_handlers[EFL_DBUS_OBJECT_EVENT_IFACE_ADDED].list)
                    break;
                efl_dbus_signal_handler_del(obj->interfaces_added);
                obj->interfaces_added = NULL;
                break;
            }
        case EFL_DBUS_OBJECT_EVENT_IFACE_REMOVED:
            {
                if (obj->event_handlers[EFL_DBUS_OBJECT_EVENT_IFACE_REMOVED]
                        .list)
                    break;
                efl_dbus_signal_handler_del(obj->interfaces_removed);
                obj->interfaces_removed = NULL;
                break;
            }
        case EFL_DBUS_OBJECT_EVENT_PROPERTY_CHANGED:
        case EFL_DBUS_OBJECT_EVENT_PROPERTY_REMOVED:
            {
                if (obj->event_handlers[EFL_DBUS_OBJECT_EVENT_PROPERTY_CHANGED]
                        .list ||
                    obj->event_handlers[EFL_DBUS_OBJECT_EVENT_PROPERTY_REMOVED]
                        .list)
                    break;
                efl_dbus_signal_handler_del(obj->properties_changed);
                obj->properties_changed = NULL;
                break;
            }
        default:
            break;
    }
}

static void
_efl_dbus_object_event_callback_call(Efl_Dbus_Object           *obj,
                                     Efl_Dbus_Object_Event_Type type,
                                     const void                *event_info)
{
    Efl_Dbus_Object_Context_Event    *ce;
    Efl_Dbus_Object_Context_Event_Cb *iter;

    ce = obj->event_handlers + type;

    ce->walking++;
    EINA_INLIST_FOREACH(ce->list, iter)
    {
        if (iter->deleted) continue;
        iter->cb((void *)iter->cb_data, obj, (void *)event_info);
    }
    ce->walking--;
    if (ce->walking > 0) return;

    EINA_LIST_FREE(ce->to_delete, iter)
    _efl_dbus_object_context_event_cb_del(ce, iter);
}

EAPI Efl_Dbus_Connection *
efl_dbus_object_connection_get(const Efl_Dbus_Object *obj)
{
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    return obj->conn;
}

EAPI const char *
efl_dbus_object_bus_name_get(const Efl_Dbus_Object *obj)
{
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    return obj->name;
}

EAPI const char *
efl_dbus_object_path_get(const Efl_Dbus_Object *obj)
{
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    return obj->path;
}

static void
_on_object_message_cb(void                   *data,
                      const Efl_Dbus_Message *msg,
                      Efl_Dbus_Pending       *pending)
{
    Efl_Dbus_Message_Cb cb  = efl_dbus_pending_data_del(pending, "__user_cb");
    Efl_Dbus_Object    *obj = efl_dbus_pending_data_del(pending, "__object");

    EFL_DBUS_OBJECT_CHECK(obj);
    obj->pendings = eina_inlist_remove(obj->pendings, EINA_INLIST_GET(pending));

    cb(data, msg, pending);
}

EAPI Efl_Dbus_Pending *
efl_dbus_object_send(Efl_Dbus_Object    *obj,
                     Efl_Dbus_Message   *msg,
                     Efl_Dbus_Message_Cb cb,
                     const void         *cb_data,
                     double              timeout)
{
    Efl_Dbus_Pending *pending;

    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(msg, NULL);

    if (!cb)
    {
        _efl_dbus_connection_send(obj->conn, msg, NULL, NULL, timeout);
        return NULL;
    }
    pending = _efl_dbus_connection_send(obj->conn,
                                        msg,
                                        _on_object_message_cb,
                                        cb_data,
                                        timeout);
    EINA_SAFETY_ON_NULL_RETURN_VAL(pending, NULL);

    efl_dbus_pending_data_set(pending, "__user_cb", cb);
    efl_dbus_pending_data_set(pending, "__object", obj);
    obj->pendings = eina_inlist_append(obj->pendings, EINA_INLIST_GET(pending));

    return pending;
}

static void
_on_signal_handler_free(void *data, const void *dead_pointer)
{
    Efl_Dbus_Object *obj = data;
    EFL_DBUS_OBJECT_CHECK(obj);
    obj->signal_handlers = eina_list_remove(obj->signal_handlers, dead_pointer);
}

EAPI Efl_Dbus_Signal_Handler *
efl_dbus_object_signal_handler_add(Efl_Dbus_Object   *obj,
                                   const char        *interface,
                                   const char        *member,
                                   Efl_Dbus_Signal_Cb cb,
                                   const void        *cb_data)
{
    Efl_Dbus_Signal_Handler *handler;

    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(cb, NULL);

    handler = _efl_dbus_signal_handler_add(obj->conn,
                                           obj->name,
                                           obj->path,
                                           interface,
                                           member,
                                           cb,
                                           cb_data);
    EINA_SAFETY_ON_NULL_RETURN_VAL(handler, NULL);

    efl_dbus_signal_handler_free_cb_add(handler, _on_signal_handler_free, obj);
    obj->signal_handlers = eina_list_append(obj->signal_handlers, handler);

    return handler;
}

EAPI Efl_Dbus_Message *
efl_dbus_object_method_call_new(Efl_Dbus_Object *obj,
                                const char      *interface,
                                const char      *member)
{
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(interface, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(member, NULL);

    return efl_dbus_message_method_call_new(obj->name,
                                            obj->path,
                                            interface,
                                            member);
}

Efl_Bool
efl_dbus_object_proxy_add(Efl_Dbus_Object *obj, Efl_Dbus_Proxy *proxy)
{
    return eina_hash_add(obj->proxies,
                         efl_dbus_proxy_interface_get(proxy),
                         proxy);
}

Efl_Dbus_Proxy *
efl_dbus_object_proxy_get(Efl_Dbus_Object *obj, const char *interface)
{
    return eina_hash_find(obj->proxies, interface);
}

Efl_Bool
efl_dbus_object_proxy_del(Efl_Dbus_Object *obj,
                          Efl_Dbus_Proxy  *proxy,
                          const char      *interface)
{
    return eina_hash_del(obj->proxies, interface, proxy);
}

EAPI Efl_Dbus_Pending *
efl_dbus_object_peer_ping(Efl_Dbus_Object    *obj,
                          Efl_Dbus_Message_Cb cb,
                          const void         *data)
{
    Efl_Dbus_Message *msg;
    Efl_Dbus_Pending *p;
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    msg = efl_dbus_object_method_call_new(obj,
                                          EFL_DBUS_FDO_INTEFACE_PEER,
                                          "Ping");
    p   = efl_dbus_object_send(obj, msg, cb, data, -1);
    return p;
}

EAPI Efl_Dbus_Pending *
efl_dbus_object_peer_machine_id_get(Efl_Dbus_Object    *obj,
                                    Efl_Dbus_Message_Cb cb,
                                    const void         *data)
{
    Efl_Dbus_Message *msg;
    Efl_Dbus_Pending *p;
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    msg = efl_dbus_object_method_call_new(obj,
                                          EFL_DBUS_FDO_INTEFACE_PEER,
                                          "GetMachineId");
    p   = efl_dbus_object_send(obj, msg, cb, data, -1);
    return p;
}

EAPI Efl_Dbus_Pending *
efl_dbus_object_introspect(Efl_Dbus_Object    *obj,
                           Efl_Dbus_Message_Cb cb,
                           const void         *data)
{
    Efl_Dbus_Message *msg;
    Efl_Dbus_Pending *p;
    EFL_DBUS_OBJECT_CHECK_RETVAL(obj, NULL);
    msg = efl_dbus_object_method_call_new(obj,
                                          EFL_DBUS_FDO_INTERFACE_INTROSPECTABLE,
                                          "Introspect");
    p   = efl_dbus_object_send(obj, msg, cb, data, -1);
    return p;
}
