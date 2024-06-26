#ifdef HAVE_CONFIG_H
#  include <efl_config.h>
#endif

#include <Efl_Shared.h>

#include "efl_dbus_model_proxy_private.h"
#include "efl_dbus_model_private.h"

#define MY_CLASS      EFL_DBUS_MODEL_PROXY_CLASS
#define MY_CLASS_NAME "Efl_Dbus_Model_Proxy"

static void _efl_dbus_model_proxy_property_get_all_cb(void *,
                                                      const Efl_Dbus_Message *,
                                                      Efl_Dbus_Pending *);
static void _efl_dbus_model_proxy_property_set_cb(void *,
                                                  const Efl_Dbus_Message *,
                                                  Efl_Dbus_Pending *);
static void _efl_dbus_model_proxy_property_set_load_cb(void *,
                                                       const Efl_Dbus_Message *,
                                                       Efl_Dbus_Pending *);
static void _efl_dbus_model_proxy_start_monitor(Efl_Dbus_Model_Proxy_Data *);
static void
_efl_dbus_model_proxy_property_changed_cb(void *, Efl_Dbus_Proxy *, void *);
static void
_efl_dbus_model_proxy_property_invalidated_cb(void *, Efl_Dbus_Proxy *, void *);
static const char *
_efl_dbus_model_proxy_property_type_get(Efl_Dbus_Model_Proxy_Data *,
                                        const char *);
static void
_efl_dbus_model_proxy_create_methods_children(Efl_Dbus_Model_Proxy_Data *);
static void
_efl_dbus_model_proxy_create_signals_children(Efl_Dbus_Model_Proxy_Data *);

typedef struct _Efl_Dbus_Model_Proxy_Property_Set_Data
  Efl_Dbus_Model_Proxy_Property_Set_Data;
typedef struct _Efl_Dbus_Property_Promise Efl_Dbus_Property_Promise;

struct _Efl_Dbus_Model_Proxy_Property_Set_Data
{
  Efl_Dbus_Model_Proxy_Data *pd;

  Eina_Stringshare *property;
  Eina_Promise     *promise;
  Eina_Value       *value;
};

struct _Efl_Dbus_Property_Promise
{
  Eina_Promise     *promise;
  Eina_Stringshare *property;
};

static Efl_Bool
_efl_dbus_model_proxy_load(Efl_Dbus_Model_Proxy_Data *pd)
{
  Efl_Dbus_Introspection_Property *property;
  Eina_List                       *it;

  if (pd->proxy) return EFL_TRUE;

  pd->proxy = efl_dbus_proxy_get(pd->object, pd->name);
  EINA_SAFETY_ON_NULL_RETURN_VAL(pd->proxy, EFL_FALSE);

  EINA_LIST_FOREACH(pd->interface->properties, it, property)
  {
    const Eina_Value_Type *type;
    Eina_Stringshare      *name;
    Eina_Value            *value;

    type  = _dbus_type_to_eina_value_type(property->type[0]);
    name  = eina_stringshare_add(property->name);
    value = eina_value_new(type);

    eina_hash_direct_add(pd->properties, name, value);
  }

  return EFL_TRUE;
}

static void
_efl_dbus_model_proxy_unload(Efl_Dbus_Model_Proxy_Data *pd)
{
  Efl_Dbus_Pending *pending;

  EINA_LIST_FREE(pd->pendings, pending)
  efl_dbus_pending_cancel(pending);

  if (pd->monitoring)
  {
    efl_dbus_proxy_event_callback_del(pd->proxy,
                                      EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED,
                                      _efl_dbus_model_proxy_property_changed_cb,
                                      pd);
    efl_dbus_proxy_event_callback_del(
      pd->proxy,
      EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED,
      _efl_dbus_model_proxy_property_invalidated_cb,
      pd);
  }
  pd->monitoring = EFL_FALSE;

  if (pd->proxy) efl_dbus_proxy_unref(pd->proxy);
  pd->proxy = NULL;
}

static void
_efl_dbus_model_proxy_object_del(void                   *data,
                                 Efl_Dbus_Object *object EFL_UNUSED,
                                 void *event_info        EFL_UNUSED)
{
  Efl_Dbus_Model_Proxy_Data *pd = data;

  pd->object = NULL;
}

static Efl_Object *
_efl_dbus_model_proxy_efl_object_constructor(Eo                        *obj,
                                             Efl_Dbus_Model_Proxy_Data *pd)
{
  obj = efl_constructor(efl_super(obj, MY_CLASS));

  pd->obj        = obj;
  pd->properties = eina_hash_stringshared_new(NULL);

  return obj;
}

static Efl_Object *
_efl_dbus_model_proxy_efl_object_finalize(Eo                        *obj,
                                          Efl_Dbus_Model_Proxy_Data *pd)
{
  if (!pd->object || !pd->name || !pd->interface) return NULL;

  if (!_efl_dbus_model_proxy_load(pd)) return NULL;

  if (!efl_dbus_model_connection_get(obj))
    efl_dbus_model_connection_set(obj,
                                  efl_dbus_object_connection_get(pd->object));

  efl_dbus_object_event_callback_add(pd->object,
                                     EFL_DBUS_OBJECT_EVENT_DEL,
                                     _efl_dbus_model_proxy_object_del,
                                     pd);

  return efl_finalize(efl_super(obj, MY_CLASS));
}

static void
_efl_dbus_model_proxy_object_set(Eo *obj                    EFL_UNUSED,
                                 Efl_Dbus_Model_Proxy_Data *pd,
                                 Efl_Dbus_Object           *object)
{
  pd->object = efl_dbus_object_ref(object);
}

static void
_efl_dbus_model_proxy_interface_set(
  Eo *obj                                 EFL_UNUSED,
  Efl_Dbus_Model_Proxy_Data              *pd,
  const Efl_Dbus_Introspection_Interface *interface)
{
  pd->name      = eina_stringshare_add(interface->name);
  pd->interface = interface;
}

static void
_efl_dbus_model_proxy_efl_object_invalidate(Eo                        *obj,
                                            Efl_Dbus_Model_Proxy_Data *pd)
{
  Eo *child;

  EINA_LIST_FREE(pd->childrens, child)
  efl_unref(child);

  _efl_dbus_model_proxy_unload(pd);

  if (pd->object)
  {
    efl_dbus_object_event_callback_del(pd->object,
                                       EFL_DBUS_OBJECT_EVENT_DEL,
                                       _efl_dbus_model_proxy_object_del,
                                       pd);
    efl_dbus_object_unref(pd->object);
  }

  efl_invalidate(efl_super(obj, MY_CLASS));
}

static void
_efl_dbus_model_proxy_efl_object_destructor(Eo                        *obj,
                                            Efl_Dbus_Model_Proxy_Data *pd)
{
  Eina_Hash_Tuple *tuple;
  Eina_Iterator   *it;

  it = eina_hash_iterator_tuple_new(pd->properties);
  EINA_ITERATOR_FOREACH(it, tuple)
  {
    Eina_Stringshare *property = tuple->key;
    Eina_Value       *value    = tuple->data;

    eina_stringshare_del(property);
    eina_value_free(value);
  }
  eina_iterator_free(it);
  eina_hash_free(pd->properties);

  eina_stringshare_del(pd->name);

  efl_destructor(efl_super(obj, MY_CLASS));
}

static Eina_Iterator *
_efl_dbus_model_proxy_efl_model_properties_get(const Eo *obj EFL_UNUSED,
                                               Efl_Dbus_Model_Proxy_Data *pd)
{
  return eina_hash_iterator_key_new(pd->properties);
}

#define PROPERTY_EXIST 1
#define PROPERTY_READ  2
#define PROPERTY_WRITE 4

static unsigned char
efl_dbus_model_proxy_property_check(Efl_Dbus_Model_Proxy_Data *pd,
                                    const char                *property)
{
  Efl_Dbus_Introspection_Property *property_introspection =
    efl_dbus_introspection_property_find(pd->interface->properties, property);
  unsigned char r = 0;

  if (property_introspection == NULL)
  {
    WRN("Property not found: %s", property);
    return 0;
  }

  r = PROPERTY_EXIST;
    // Check read access
  if (property_introspection->access ==
        ELDBUS_INTROSPECTION_PROPERTY_ACCESS_READ ||
      property_introspection->access ==
        ELDBUS_INTROSPECTION_PROPERTY_ACCESS_READWRITE)
    r |= PROPERTY_READ;
    // Check write access
  if (property_introspection->access ==
        ELDBUS_INTROSPECTION_PROPERTY_ACCESS_WRITE ||
      property_introspection->access ==
        ELDBUS_INTROSPECTION_PROPERTY_ACCESS_READWRITE)
    r |= PROPERTY_WRITE;

  return r;
}

static void
_efl_dbus_model_proxy_cancel_cb(Efl_Loop_Consumer *consumer    EFL_UNUSED,
                                void                          *data,
                                const Eina_Future *dead_future EFL_UNUSED)
{
  Efl_Dbus_Model_Proxy_Property_Set_Data *sd = data;

  sd->promise = NULL;
  eina_stringshare_del(sd->property);
  eina_value_free(sd->value);
  free(sd);
}

static Efl_Dbus_Pending *
_efl_dbus_model_proxy_load_all(Efl_Dbus_Model_Proxy_Data *pd,
                               Eina_Promise              *promise,
                               const char                *property,
                               Efl_Dbus_Message_Cb        callback,
                               void                      *data)
{
  Efl_Dbus_Property_Promise *p;
  Efl_Dbus_Pending          *pending = NULL;

  p = calloc(1, sizeof(Efl_Dbus_Property_Promise));
  if (!p)
  {
    if (promise) eina_promise_reject(promise, ENOMEM);
    return NULL;
  }

  p->promise   = promise;
  p->property  = eina_stringshare_add(property);
  pd->promises = eina_list_append(pd->promises, p);

  if (!pd->pendings)
  {
    pending = efl_dbus_proxy_property_get_all(pd->proxy, callback, data);
  }
  return pending;
}

static Eina_Future *
_efl_dbus_model_proxy_efl_model_property_set(Eo *obj EFL_UNUSED,
                                             Efl_Dbus_Model_Proxy_Data *pd,
                                             const char *property,
                                             Eina_Value *value)
{
  Efl_Dbus_Model_Proxy_Property_Set_Data *data = NULL;
  const char                             *signature;
  Efl_Dbus_Pending                       *pending;
  unsigned char                           access;
  Eina_Error                              err = 0;

  DBG("(%p): property=%s", obj, property);

  access = efl_dbus_model_proxy_property_check(pd, property);
  err    = EFL_MODEL_ERROR_NOT_FOUND;
  if (!access) goto on_error;
  err = EFL_MODEL_ERROR_READ_ONLY;
  if (!(access & PROPERTY_WRITE)) goto on_error;

  err       = EFL_MODEL_ERROR_UNKNOWN;
  signature = _efl_dbus_model_proxy_property_type_get(pd, property);
  if (!signature) goto on_error;

  err  = ENOMEM;
  data = calloc(1, sizeof(Efl_Dbus_Model_Proxy_Property_Set_Data));
  if (!data) goto on_error;

  data->pd       = pd;
  data->promise  = efl_loop_promise_new(obj);
  data->property = eina_stringshare_add(property);
  if (!(data->value = eina_value_dup(value))) goto on_error;

  if (!pd->is_loaded)
  {
    pending =
      _efl_dbus_model_proxy_load_all(pd,
                                     data->promise,
                                     property,
                                     _efl_dbus_model_proxy_property_set_load_cb,
                                     data);
  }
  else
  {
    pending =
      efl_dbus_proxy_property_value_set(pd->proxy,
                                        property,
                                        signature,
                                        (Eina_Value *)value,
                                        _efl_dbus_model_proxy_property_set_cb,
                                        data);
  }

  if (pending) pd->pendings = eina_list_append(pd->pendings, pending);
  return efl_future_then(obj,
                         eina_future_new(data->promise),
                         .data = data,
                         .free = _efl_dbus_model_proxy_cancel_cb);

on_error:
  free(data);
  return efl_loop_future_rejected(obj, err);
}

static Eina_Value *
_efl_dbus_model_proxy_efl_model_property_get(const Eo *obj EFL_UNUSED,
                                             Efl_Dbus_Model_Proxy_Data *pd,
                                             const char *property)
{
  Efl_Dbus_Pending *pending;
  unsigned char     access;
  Eina_Error        err = 0;

  access = efl_dbus_model_proxy_property_check(pd, property);
  err    = EFL_MODEL_ERROR_NOT_FOUND;
  if (!access) goto on_error;
  if (!(access & PROPERTY_READ)) goto on_error;

  if (pd->is_loaded)
  {
    Eina_Stringshare *tmp;
    Eina_Value       *value;

    err   = EFL_MODEL_ERROR_NOT_FOUND;
    tmp   = eina_stringshare_add(property);
    value = eina_hash_find(pd->properties, tmp);
    eina_stringshare_del(tmp);
    if (!value) goto on_error;

    return eina_value_dup(value);
  }

  err = ENOMEM;

  pending =
    _efl_dbus_model_proxy_load_all(pd,
                                   NULL,
                                   property,
                                   _efl_dbus_model_proxy_property_get_all_cb,
                                   pd);
  if (pending) pd->pendings = eina_list_append(pd->pendings, pending);
  else goto on_error;

  return eina_value_error_new(EAGAIN);

on_error:
  return eina_value_error_new(err);
}

static void
_efl_dbus_model_proxy_listed(Efl_Dbus_Model_Proxy_Data *pd)
{
  if (!pd->is_listed)
  {
    _efl_dbus_model_proxy_create_methods_children(pd);
    _efl_dbus_model_proxy_create_signals_children(pd);

    efl_event_callback_call(pd->obj,
                            EFL_MODEL_EVENT_CHILDREN_COUNT_CHANGED,
                            NULL);
    pd->is_listed = EFL_TRUE;
  }
}

static Eina_Future *
_efl_dbus_model_proxy_efl_model_children_slice_get(
  Eo *obj                    EFL_UNUSED,
  Efl_Dbus_Model_Proxy_Data *pd,
  unsigned                   start,
  unsigned                   count)
{
  Eina_Value v;

  _efl_dbus_model_proxy_listed(pd);

  v = efl_model_list_value_get(pd->childrens, start, count);
  return efl_loop_future_resolved(obj, v);
}

static unsigned int
_efl_dbus_model_proxy_efl_model_children_count_get(
  const Eo *obj              EFL_UNUSED,
  Efl_Dbus_Model_Proxy_Data *pd)
{
  _efl_dbus_model_proxy_listed(pd);
  return eina_list_count(pd->childrens);
}

static void
_efl_dbus_model_proxy_create_methods_children(Efl_Dbus_Model_Proxy_Data *pd)
{
  Efl_Dbus_Introspection_Method *method;
  Eina_List                     *it;

  EINA_LIST_FOREACH(pd->interface->methods, it, method)
  {
    const char *bus;
    const char *path;
    const char *interface_name;
    const char *method_name;
    Eo         *child;

    bus = efl_dbus_object_bus_name_get(pd->object);
    if (!bus) continue;

    path = efl_dbus_object_path_get(pd->object);
    if (!path) continue;

    interface_name = pd->interface->name;
    if (!interface_name) continue;

    method_name = method->name;
    if (!method_name) continue;

    INF("(%p) Creating method child: bus = %s, path = %s, method = %s::%s",
        pd->obj,
        bus,
        path,
        interface_name,
        method_name);

    child = efl_add_ref(EFL_DBUS_MODEL_METHOD_CLASS,
                        pd->obj,
                        efl_dbus_model_method_proxy_set(efl_added, pd->proxy),
                        efl_dbus_model_method_set(efl_added, method));

    if (child) pd->childrens = eina_list_append(pd->childrens, child);
    else
      ERR("Could not create method child: bus = %s, path = %s method = "
          "%s::%s.",
          bus,
          path,
          interface_name,
          method_name);
  }
}

static void
_efl_dbus_model_proxy_create_signals_children(Efl_Dbus_Model_Proxy_Data *pd)
{
  Eina_List                     *it;
  Efl_Dbus_Introspection_Signal *signal;

  EINA_LIST_FOREACH(pd->interface->signals, it, signal)
  {
    const char *bus;
    const char *path;
    const char *interface_name;
    const char *signal_name;
    Eo         *child;

    bus = efl_dbus_object_bus_name_get(pd->object);
    if (!bus) continue;

    path = efl_dbus_object_path_get(pd->object);
    if (!path) continue;

    interface_name = pd->interface->name;
    if (!interface_name) continue;

    signal_name = signal->name;
    if (!signal_name) continue;

    DBG("(%p) Creating signal child: bus = %s, path = %s, signal = %s::%s",
        pd->obj,
        bus,
        path,
        interface_name,
        signal_name);

    child = efl_add_ref(
      EFL_DBUS_MODEL_SIGNAL_CLASS,
      pd->obj,
      efl_dbus_model_signal_constructor(efl_added, pd->proxy, signal));

    if (child) pd->childrens = eina_list_append(pd->childrens, child);
    else
      ERR("Could not create signal child: bus = %s, path = %s signal = "
          "%s::%s.",
          bus,
          path,
          interface_name,
          signal_name);
  }
}

static const char *
_efl_dbus_model_proxy_proxy_name_get(const Eo *obj              EFL_UNUSED,
                                     Efl_Dbus_Model_Proxy_Data *pd)
{
  return pd->name;
}

static void
_efl_dbus_model_proxy_start_monitor(Efl_Dbus_Model_Proxy_Data *pd)
{
  if (pd->monitoring) return;

  pd->monitoring = EFL_TRUE;

  efl_dbus_proxy_event_callback_add(pd->proxy,
                                    EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED,
                                    _efl_dbus_model_proxy_property_changed_cb,
                                    pd);

  efl_dbus_proxy_event_callback_add(
    pd->proxy,
    EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED,
    _efl_dbus_model_proxy_property_invalidated_cb,
    pd);
}

static void
_efl_dbus_model_proxy_property_changed_cb(void                 *data,
                                          Efl_Dbus_Proxy *proxy EFL_UNUSED,
                                          void                 *event_info)
{
  Efl_Dbus_Model_Proxy_Data *pd = (Efl_Dbus_Model_Proxy_Data *)data;
  Efl_Dbus_Object_Event_Property_Changed *event =
    (Efl_Dbus_Object_Event_Property_Changed *)event_info;
  Eina_Value *prop_value;
  Efl_Bool    ret;

  prop_value = eina_hash_find(pd->properties, event->name);
  if (!prop_value) return;

  ret = eina_value_copy(event->value, prop_value);
  if (!ret) return;

  efl_model_properties_changed(pd->obj, event->name);
}

static void
_efl_dbus_model_proxy_property_invalidated_cb(void                 *data,
                                              Efl_Dbus_Proxy *proxy EFL_UNUSED,
                                              void                 *event_info)
{
  Efl_Dbus_Model_Proxy_Data             *pd = (Efl_Dbus_Model_Proxy_Data *)data;
  Efl_Dbus_Proxy_Event_Property_Changed *event =
    (Efl_Dbus_Proxy_Event_Property_Changed *)event_info;

  efl_model_property_invalidated_notify(pd->obj, event->name);
}

static Eina_Array *
_efl_dbus_model_proxy_property_get_all_load(const Efl_Dbus_Message    *msg,
                                            Efl_Dbus_Model_Proxy_Data *pd)
{
  Efl_Dbus_Message_Iter *values = NULL;
  Efl_Dbus_Message_Iter *entry;
  Eina_Array            *changed_properties;
  Eina_Stringshare      *tmp = NULL;
  const char            *error_name, *error_text;

  if (efl_dbus_message_error_get(msg, &error_name, &error_text))
  {
    ERR("%s: %s", error_name, error_text);
    return NULL;
  }

  if (!efl_dbus_message_arguments_get(msg, "a{sv}", &values))
  {
    ERR("%s", "Error getting arguments.");
    return NULL;
  }

  changed_properties = eina_array_new(1);
  while (efl_dbus_message_iter_get_and_next(values, 'e', &entry))
  {
    const char            *property;
    Efl_Dbus_Message_Iter *variant;
    Eina_Value            *struct_value;
    Eina_Value            *prop_value;
    Eina_Value             arg0;
    Efl_Bool               ret;

    if (!efl_dbus_message_iter_arguments_get(entry, "sv", &property, &variant))
      continue;

    struct_value = efl_dbus_message_iter_struct_like_to_eina_value(variant);
    if (!struct_value) goto on_error;

    ret = eina_value_struct_value_get(struct_value, "arg0", &arg0);
    eina_value_free(struct_value);
    if (!ret) goto on_error;

    tmp        = eina_stringshare_add(property);
    prop_value = eina_hash_find(pd->properties, tmp);
    if (!prop_value) goto on_error;

    ret = eina_value_copy(&arg0, prop_value);
    if (!ret) goto on_error;

    eina_value_flush(&arg0);

    ret = eina_array_push(changed_properties, tmp);
    if (!ret) goto on_error;

        // Reset tmp to NULL to avoid double free.
    tmp = NULL;
  }

  pd->is_loaded = EFL_TRUE;
  return changed_properties;

on_error:
  eina_stringshare_del(tmp);
  while ((tmp = eina_array_pop(changed_properties)))
    eina_stringshare_del(tmp);
  eina_array_free(changed_properties);
  return NULL;
}

static void
_efl_dbus_model_proxy_promise_clean(Efl_Dbus_Property_Promise *p,
                                    Eina_Error                 err)
{
  if (p->promise) eina_promise_reject(p->promise, err);
  eina_stringshare_del(p->property);
  free(p);
}

static void
_efl_dbus_model_proxy_property_get_all_cb(void                   *data,
                                          const Efl_Dbus_Message *msg,
                                          Efl_Dbus_Pending       *pending)
{
  Efl_Dbus_Model_Proxy_Data *pd = (Efl_Dbus_Model_Proxy_Data *)data;
  Efl_Dbus_Property_Promise *p;
  Eina_Stringshare          *sp;
  Eina_Array                *properties;
  Efl_Model_Property_Event   evt;

  pd->pendings = eina_list_remove(pd->pendings, pending);

  properties = _efl_dbus_model_proxy_property_get_all_load(msg, pd);
  if (!properties)
  {
    EINA_LIST_FREE(pd->promises, p)
    _efl_dbus_model_proxy_promise_clean(p, EFL_MODEL_ERROR_NOT_FOUND);
    return;
  }

  EINA_LIST_FREE(pd->promises, p)
  _efl_dbus_model_proxy_promise_clean(p, EFL_MODEL_ERROR_READ_ONLY);

  _efl_dbus_model_proxy_start_monitor(pd);

  evt.changed_properties = properties;
  efl_event_callback_call(pd->obj, EFL_MODEL_EVENT_PROPERTIES_CHANGED, &evt);
  while ((sp = eina_array_pop(properties)))
    eina_stringshare_del(sp);
  eina_array_free(properties);
}

static void
_efl_dbus_model_proxy_property_set_load_cb(void                   *data,
                                           const Efl_Dbus_Message *msg,
                                           Efl_Dbus_Pending       *pending)
{
  Efl_Dbus_Model_Proxy_Property_Set_Data *set_data =
    (Efl_Dbus_Model_Proxy_Property_Set_Data *)data;
  Efl_Dbus_Model_Proxy_Data *pd = set_data->pd;
  Eina_Array                *properties;
  Eina_Stringshare          *sp;
  const char                *signature;

  pd->pendings = eina_list_remove(pd->pendings, pending);

  signature = _efl_dbus_model_proxy_property_type_get(pd, set_data->property);

  properties = _efl_dbus_model_proxy_property_get_all_load(msg, pd);
  if (!signature || !properties)
  {
    eina_promise_reject(set_data->promise, EFL_MODEL_ERROR_UNKNOWN);
    goto end;
  }

  pending =
    efl_dbus_proxy_property_value_set(pd->proxy,
                                      set_data->property,
                                      signature,
                                      set_data->value,
                                      _efl_dbus_model_proxy_property_set_cb,
                                      set_data);
  pd->pendings = eina_list_append(pd->pendings, pending);

end:
  if (!properties) return;
  while ((sp = eina_array_pop(properties)))
    eina_stringshare_del(sp);
  eina_array_free(properties);
}

static void
_efl_dbus_model_proxy_property_set_cb(void                   *data,
                                      const Efl_Dbus_Message *msg,
                                      Efl_Dbus_Pending       *pending)
{
  Efl_Dbus_Model_Proxy_Property_Set_Data *sd =
    (Efl_Dbus_Model_Proxy_Property_Set_Data *)data;
  Efl_Dbus_Model_Proxy_Data *pd = sd->pd;
  const char                *error_name, *error_text;
  Eina_Value                *value;

  pd->pendings = eina_list_remove(pd->pendings, pending);

  if (efl_dbus_message_error_get(msg, &error_name, &error_text))
  {
    ERR("%s: %s", error_name, error_text);
    eina_promise_reject(sd->promise, EFL_MODEL_ERROR_UNKNOWN);
    return;
  }

  value = eina_hash_find(pd->properties, sd->property);
  if (value)
  {
    efl_model_properties_changed(pd->obj, sd->property);
    if (sd->promise)
      eina_promise_resolve(sd->promise, eina_value_reference_copy(value));
  }
  else
  {
    if (sd->promise)
      eina_promise_reject(sd->promise, EFL_MODEL_ERROR_NOT_FOUND);
  }
}

static const char *
_efl_dbus_model_proxy_property_type_get(Efl_Dbus_Model_Proxy_Data *pd,
                                        const char                *property)
{
  Efl_Dbus_Introspection_Property *property_introspection =
    efl_dbus_introspection_property_find(pd->interface->properties, property);

  if (property_introspection == NULL)
  {
    WRN("Property not found: %s", property);
    return NULL;
  }

  return property_introspection->type;
}

#include "efl_dbus_model_proxy.eo.c"
