#ifdef HAVE_CONFIG_H
#  include <efl_config.h>
#endif

#include "efl_dbus_model_object_private.h"
#include "efl_dbus_model_private.h"

#include <Efl_Core.h>
#include <Efl_Shared.h>

#define MY_CLASS      EFL_DBUS_MODEL_OBJECT_CLASS
#define MY_CLASS_NAME "Efl_Dbus_Model_Object"

static void _efl_dbus_model_object_introspect_cb(void *,
                                                 const Efl_Dbus_Message *,
                                                 Efl_Dbus_Pending *);
static void _efl_dbus_model_object_create_children(Efl_Dbus_Model_Object_Data *,
                                                   Efl_Dbus_Object *,
                                                   Eina_List *);

static Efl_Object *
_efl_dbus_model_object_efl_object_constructor(Eo                         *obj,
                                              Efl_Dbus_Model_Object_Data *pd)
{
  obj = efl_constructor(efl_super(obj, MY_CLASS));

  pd->obj = obj;

  return obj;
}

static void
_efl_dbus_model_object_bus_set(Eo *obj                     EFL_UNUSED,
                               Efl_Dbus_Model_Object_Data *pd,
                               const char                 *bus)
{
  pd->bus = eina_stringshare_add(bus);
}

static void
_efl_dbus_model_object_path_set(Eo *obj                     EFL_UNUSED,
                                Efl_Dbus_Model_Object_Data *pd,
                                const char                 *path)
{
  pd->path = eina_stringshare_add(path);
}

static Efl_Object *
_efl_dbus_model_object_efl_object_finalize(Eo                         *obj,
                                           Efl_Dbus_Model_Object_Data *pd)
{
  if (!pd->bus || !pd->path) return NULL;

  return efl_finalize(efl_super(obj, MY_CLASS));
}

static void
_efl_dbus_model_object_efl_object_invalidate(Eo                         *obj,
                                             Efl_Dbus_Model_Object_Data *pd)
{
  Efl_Dbus_Pending *pending;
  Efl_Dbus_Object  *object;
  Eo               *child;

  EINA_LIST_FREE(pd->childrens, child)
  efl_unref(child);

  EINA_LIST_FREE(pd->pendings, pending)
  efl_dbus_pending_cancel(pending);

  EINA_LIST_FREE(pd->objects, object)
  efl_dbus_object_unref(object);

  if (pd->introspection)
  {
    efl_dbus_introspection_node_free(pd->introspection);
    pd->introspection = NULL;
  }

  efl_invalidate(efl_super(obj, MY_CLASS));
}

static void
_efl_dbus_model_object_efl_object_destructor(Eo                         *obj,
                                             Efl_Dbus_Model_Object_Data *pd)
{
  eina_stringshare_del(pd->bus);
  eina_stringshare_del(pd->path);

  efl_destructor(efl_super(obj, MY_CLASS));
}

static Efl_Bool
_efl_dbus_model_object_introspect(const Eo                   *obj,
                                  Efl_Dbus_Model_Object_Data *pd,
                                  const char                 *bus,
                                  const char                 *path)
{
  Efl_Dbus_Pending *pending;
  Efl_Dbus_Object  *object;

  DBG("(%p) Introspecting: bus = %s, path = %s", pd->obj, bus, path);

  object = efl_dbus_object_get(efl_dbus_model_connection_get(obj), bus, path);
  if (!object)
  {
    ERR("(%p): Cannot get object: bus=%s, path=%s", pd->obj, bus, path);
    return EFL_FALSE;
  }
  pd->objects = eina_list_append(pd->objects, object);

   // TODO: Register for interface added/removed event
  pending = efl_dbus_object_introspect(object,
                                       &_efl_dbus_model_object_introspect_cb,
                                       pd);
  efl_dbus_pending_data_set(pending, "object", object);
  pd->pendings = eina_list_append(pd->pendings, pending);
  return EFL_TRUE;
}

static Eina_Future *
_efl_dbus_model_object_efl_model_children_slice_get(
  Eo *obj                     EFL_UNUSED,
  Efl_Dbus_Model_Object_Data *pd,
  unsigned                    start,
  unsigned                    count)
{
  Efl_Dbus_Children_Slice_Promise *slice;
  Eina_Promise                    *p;

  if (pd->is_listed)
  {
    Eina_Value v;

    v = efl_model_list_value_get(pd->childrens, start, count);
    return efl_loop_future_resolved(obj, v);
  }

  p = efl_loop_promise_new(obj);

  slice        = calloc(1, sizeof(struct _Efl_Dbus_Children_Slice_Promise));
  slice->p     = p;
  slice->start = start;
  slice->count = count;

  pd->requests = eina_list_prepend(pd->requests, slice);

  if (!pd->pendings)
    _efl_dbus_model_object_introspect(obj, pd, pd->bus, pd->path);
  return efl_future_then(obj, eina_future_new(p));
  ;
}

static unsigned int
_efl_dbus_model_object_efl_model_children_count_get(
  const Eo *obj               EFL_UNUSED,
  Efl_Dbus_Model_Object_Data *pd)
{
  if (!pd->is_listed && !pd->pendings)
    _efl_dbus_model_object_introspect(obj, pd, pd->bus, pd->path);
  return eina_list_count(pd->childrens);
}

static const char *
_efl_dbus_model_object_bus_get(const Eo *obj               EFL_UNUSED,
                               Efl_Dbus_Model_Object_Data *pd)
{
  return pd->bus;
}

static const char *
_efl_dbus_model_object_path_get(const Eo *obj               EFL_UNUSED,
                                Efl_Dbus_Model_Object_Data *pd)
{
  return pd->path;
}

static char *
_efl_dbus_model_object_concatenate_path(const char *root_path,
                                        const char *relative_path)
{
  Efl_Strbuf *buffer;
  const char *format        = (!eina_streq(root_path, "/")) ? "%s/%s" : "%s%s";
  char       *absolute_path = NULL;

  buffer = eina_strbuf_new();
  eina_strbuf_append_printf(buffer, format, root_path, relative_path);
  absolute_path = eina_strbuf_string_steal(buffer);

  eina_strbuf_free(buffer);
  return absolute_path;
}

static void
_efl_dbus_model_object_introspect_nodes(Efl_Dbus_Model_Object_Data *pd,
                                        const char *current_path,
                                        Eina_List  *nodes)
{
  Efl_Dbus_Introspection_Node *node;
  Eina_List                   *it;

  EINA_LIST_FOREACH(nodes, it, node)
  {
    const char *relative_path;
    char       *absolute_path;

    relative_path = node->name;
    if (!relative_path) continue;

    absolute_path =
      _efl_dbus_model_object_concatenate_path(current_path, relative_path);
    if (!absolute_path) continue;

    _efl_dbus_model_object_introspect(pd->obj, pd, pd->bus, absolute_path);

    free(absolute_path);
  }
}

static void
_efl_dbus_model_object_create_children(Efl_Dbus_Model_Object_Data *pd,
                                       Efl_Dbus_Object            *object,
                                       Eina_List                  *interfaces)
{
  Efl_Dbus_Introspection_Interface *interface;
  const char                       *current_path;
  Eina_List                        *l;

  current_path = efl_dbus_object_path_get(object);
  if (!current_path) return;

  EINA_LIST_FOREACH(interfaces, l, interface)
  {
    Eo *child;

    DBG("(%p) Creating child: bus = %s, path = %s, interface = %s",
        pd->obj,
        pd->bus,
        current_path,
        interface->name);

        // TODO: increment reference to keep 'interface' in memory
    child =
      efl_add_ref(EFL_DBUS_MODEL_PROXY_CLASS,
                  pd->obj,
                  efl_dbus_model_proxy_object_set(efl_added, object),
                  efl_dbus_model_proxy_interface_set(efl_added, interface));

    if (child) pd->childrens = eina_list_append(pd->childrens, child);
  }
}

static void
_efl_dbus_model_object_introspect_cb(void                   *data,
                                     const Efl_Dbus_Message *msg,
                                     Efl_Dbus_Pending       *pending)
{
  Efl_Dbus_Model_Object_Data      *pd = (Efl_Dbus_Model_Object_Data *)data;
  Efl_Dbus_Children_Slice_Promise *slice;
  Efl_Dbus_Object                 *object;
  const char                      *error_name;
  const char                      *error_text;
  const char                      *xml = NULL;
  const char                      *current_path;

  pd->pendings = eina_list_remove(pd->pendings, pending);
  object       = efl_dbus_pending_data_get(pending, "object");

  if (efl_dbus_message_error_get(msg, &error_name, &error_text))
  {
    ERR("%s: %s", error_name, error_text);
        //efl_model_error_notify(pd->obj);
    return;
  }

  if (!efl_dbus_message_arguments_get(msg, "s", &xml))
  {
    ERR("Error getting arguments.");
    return;
  }

  if (!xml)
  {
    ERR("No XML.");
    return;
  }

  current_path      = efl_dbus_object_path_get(object);
  pd->introspection = efl_dbus_introspection_parse(xml);

  DBG("(%p): introspect of bus = %s, path = %s =>\n%s",
      pd->obj,
      pd->bus,
      current_path,
      xml);

  _efl_dbus_model_object_introspect_nodes(pd,
                                          current_path,
                                          pd->introspection->nodes);
  _efl_dbus_model_object_create_children(pd,
                                         object,
                                         pd->introspection->interfaces);

  if (eina_list_count(pd->pendings) != 0) return;

  efl_event_callback_call(pd->obj,
                          EFL_MODEL_EVENT_CHILDREN_COUNT_CHANGED,
                          NULL);

  pd->is_listed = EFL_TRUE;

  EINA_LIST_FREE(pd->requests, slice)
  {
    Eina_Value v;

    v = efl_model_list_value_get(pd->childrens, slice->start, slice->count);
    eina_promise_resolve(slice->p, v);

    free(slice);
  }
}

#include "efl_dbus_model_object.eo.c"
