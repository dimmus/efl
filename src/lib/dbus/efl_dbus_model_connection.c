#ifdef HAVE_CONFIG_H
# include <efl_config.h>
#endif

#include <Efl_Core.h>
#include "efl_dbus_model_connection_private.h"
#include "efl_dbus_model_private.h"

#define MY_CLASS EFL_DBUS_MODEL_CONNECTION_CLASS
#define MY_CLASS_NAME "Efl_Dbus_Model_Connection"

static void _efl_dbus_model_connection_names_list_cb(void *, const Efl_Dbus_Message *, Efl_Dbus_Pending *);

static Efl_Object*
_efl_dbus_model_connection_efl_object_constructor(Eo *obj, Efl_Dbus_Model_Connection_Data *pd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));

   pd->obj = obj;

   return obj;
}

static void
_efl_dbus_model_connection_efl_object_invalidate(Eo *obj, Efl_Dbus_Model_Connection_Data *pd)
{
   Efl_Dbus_Children_Slice_Promise *slice;

   if (pd->pending) efl_dbus_pending_cancel(pd->pending);

   EINA_LIST_FREE(pd->requests, slice)
     {
        eina_promise_reject(slice->p, EFL_MODEL_ERROR_UNKNOWN);
        free(slice);
     }

   pd->childrens = eina_list_free(pd->childrens);

   efl_invalidate(efl_super(obj, EFL_DBUS_MODEL_CONNECTION_CLASS));
}

static void
_efl_dbus_model_children_list(const Eo *obj, Efl_Dbus_Model_Connection_Data *pd)
{
   Efl_Dbus_Model_Data *sd;

   if (pd->pending || pd->is_listed) return ;

   sd = efl_data_scope_get(obj, EFL_DBUS_MODEL_CLASS);

   pd->pending = efl_dbus_names_list(sd->connection,
                                   &_efl_dbus_model_connection_names_list_cb,
                                   pd);
}

static Eina_Future *
_efl_dbus_model_connection_efl_model_children_slice_get(Eo *obj,
                                                      Efl_Dbus_Model_Connection_Data *pd,
                                                      unsigned int start,
                                                      unsigned int count)
{
   Efl_Dbus_Children_Slice_Promise* slice;
   Eina_Promise *p;

   if (pd->is_listed)
     {
        Eina_Value v;

        v = efl_model_list_value_get(pd->childrens, start, count);
        return efl_loop_future_resolved(obj, v);
     }

   p = efl_loop_promise_new(obj);

   slice = calloc(1, sizeof (Efl_Dbus_Children_Slice_Promise));
   slice->p = p;
   slice->start = start;
   slice->count = count;

   pd->requests = eina_list_prepend(pd->requests, slice);

   _efl_dbus_model_children_list(obj, pd);
   return efl_future_then(obj, eina_future_new(p));;
}

static unsigned int
_efl_dbus_model_connection_efl_model_children_count_get(const Eo *obj,
                                                      Efl_Dbus_Model_Connection_Data *pd)
{
   _efl_dbus_model_children_list(obj, pd);
   return eina_list_count(pd->childrens);
}

static void
_efl_dbus_model_connection_names_list_cb(void *data,
                                       const Efl_Dbus_Message *msg,
                                       Efl_Dbus_Pending *pending EFL_UNUSED)
{
   Efl_Dbus_Model_Connection_Data *pd = (Efl_Dbus_Model_Connection_Data*) data;
   Efl_Dbus_Model_Data *sd;
   Efl_Dbus_Children_Slice_Promise *slice;
   const char *error_name, *error_text;
   Efl_Dbus_Message_Iter *array = NULL;
   const char *bus;

   pd->pending = NULL;

   if (efl_dbus_message_error_get(msg, &error_name, &error_text))
     {
        ERR("%s: %s", error_name, error_text);
        return;
     }

   if (!efl_dbus_message_arguments_get(msg, "as", &array))
     {
        ERR("%s", "Error getting arguments.");
        return;
     }

   sd = efl_data_scope_get(pd->obj, EFL_DBUS_MODEL_CLASS);

   while (efl_dbus_message_iter_get_and_next(array, 's', &bus))
     {
        Eo *child;

        DBG("(%p): bus = %s", pd->obj, bus);

        child = efl_add(EFL_DBUS_MODEL_OBJECT_CLASS, pd->obj,
                        efl_dbus_model_connection_set(efl_added, sd->connection),
                        efl_dbus_model_object_bus_set(efl_added, bus),
                        efl_dbus_model_object_path_set(efl_added, "/"));

        pd->childrens = eina_list_append(pd->childrens, child);
     }

   pd->is_listed = EFL_TRUE;

   efl_event_callback_call(pd->obj, EFL_MODEL_EVENT_CHILDREN_COUNT_CHANGED, NULL);

   EINA_LIST_FREE(pd->requests, slice)
     {
        Eina_Value v;

        v = efl_model_list_value_get(pd->childrens,
                                     slice->start, slice->count);
        eina_promise_resolve(slice->p, v);

        free(slice);
     }
}

#include "efl_dbus_model_connection.eo.c"
