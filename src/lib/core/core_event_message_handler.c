#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>

#include "core_private.h"

#define MY_CLASS CORE_EVENT_MESSAGE_HANDLER_CLASS

//////////////////////////////////////////////////////////////////////////

typedef struct _Handler Handler;
typedef struct _Filter  Filter;

struct _Handler
{
  EINA_INLIST;
  Core_Event_Handler_Cb func;
  void                 *data;
  int                   type;
  Efl_Bool              delete_me : 1;
  Efl_Bool              to_add    : 1;
  Efl_Bool              prepend   : 1;
};

struct _Filter
{
  EINA_INLIST;
  Core_Data_Cb   func_start;
  Core_Filter_Cb func_filter;
  Core_End_Cb    func_end;
  void          *data;
  void          *loop_data;
  Efl_Bool       delete_me : 1;
};

typedef struct _Core_Event_Message_Handler_Data Core_Event_Message_Handler_Data;

struct _Core_Event_Message_Handler_Data
{
  int           event_type_count;
  Eina_Inlist **handlers; // array of event_type_count inlists of handlers
  Eina_Inlist  *filters;
  Eina_List    *handlers_delete;
  Eina_List    *handlers_add;
  Eina_List    *filters_delete;
  Eina_List    *filters_add;
  void         *current_event_data;
  int           current_event_type;
  int           handlers_walking;
  int           filters_walking;
};

//////////////////////////////////////////////////////////////////////////

Efl_Bool
_core_event_do_filter(void *handler_pd, Eo *msg_handler, Eo *msg)
{
  Filter                          *f;
  void                            *ev;
  int                              type;
  Core_Event_Message_Handler_Data *eemhd = handler_pd;

  if (!eemhd->filters) return EFL_TRUE;
  if (!efl_isa(msg_handler, MY_CLASS)) return EFL_TRUE;
  eemhd->filters_walking++;
  EINA_INLIST_FOREACH(eemhd->filters, f)
  {
    if (f->delete_me) continue;
    type = -1;
    ev   = NULL;
    core_event_message_data_get(msg, &type, &ev, NULL, NULL);
    if (type >= 0)
    {
      if (!f->func_filter(f->data, f->loop_data, type, ev))
        _efl_loop_message_unsend(msg);
    }
  }
  eemhd->filters_walking--;
  return EFL_TRUE;
}

void
_core_event_filters_call(Eo *obj, Efl_Loop_Data *pd)
{
  Filter                          *f;
  Core_Event_Message_Handler_Data *eemhd;
  Eo                              *core_event_handler =
    efl_provider_find(obj, CORE_EVENT_MESSAGE_HANDLER_CLASS);
  /* If this is not != NULL, then _core_event_init was not called yet, which means,
      there cannot be any registered events yet
    */
  if (!core_event_handler) return;

  eemhd = efl_data_scope_get(core_event_handler, MY_CLASS);
  if (!eemhd) return;
  if (!eemhd->filters) return;
  eemhd->filters_walking++;
  EINA_INLIST_FOREACH(eemhd->filters, f)
  {
    if (f->delete_me) continue;
    if (f->func_start) f->loop_data = f->func_start(f->data);
  }
  _efl_loop_messages_filter(obj, pd, eemhd);
  EINA_INLIST_FOREACH(eemhd->filters, f)
  {
    if (f->delete_me) continue;
    if (f->func_end) f->func_end(f->data, f->loop_data);
  }
  eemhd->filters_walking--;
  if (eemhd->filters_walking == 0)
  {
    EINA_LIST_FREE(eemhd->filters_delete, f)
    {
      free(f);
    }
  }
}

//////////////////////////////////////////////////////////////////////////

EOLIAN static Core_Event_Message *
_core_event_message_handler_message_type_add(Eo *obj,
                                             Core_Event_Message_Handler_Data *pd
                                               EFL_UNUSED)
{
  // XXX: implemented event obj cache
  return efl_add(CORE_EVENT_MESSAGE_CLASS, obj);
}

EOLIAN static int
_core_event_message_handler_type_new(Eo *obj EFL_UNUSED,
                                     Core_Event_Message_Handler_Data *pd)
{
  Eina_Inlist **tmp;
  int           evnum;

  evnum = pd->event_type_count + 1;
  tmp   = realloc(pd->handlers, sizeof(Eina_Inlist *) * (evnum + 1));
  if (!tmp) return 0;
  pd->handlers                  = tmp;
  pd->handlers[CORE_EVENT_NONE] = NULL;
  pd->handlers[evnum]           = NULL;
  pd->event_type_count          = evnum;
  return evnum;
}

EOLIAN static void *
_core_event_message_handler_handler_add(Eo *obj EFL_UNUSED,
                                        Core_Event_Message_Handler_Data *pd,
                                        int                              type,
                                        void                            *func,
                                        void                            *data)
{
  Handler *h;

  EINA_SAFETY_ON_TRUE_RETURN_VAL((type <= 0) || (type > pd->event_type_count) ||
                                   (!func),
                                 NULL);
  h = calloc(1, sizeof(Handler));
  if (!h) return NULL;
  h->func = func;
  h->data = data;
  h->type = type;
  if (pd->current_event_type == type)
  {
    h->to_add        = EFL_TRUE;
    pd->handlers_add = eina_list_append(pd->handlers_add, h);
  }
  else
    pd->handlers[type] =
      eina_inlist_append(pd->handlers[type], EINA_INLIST_GET(h));
  return h;
}

EOLIAN static void *
_core_event_message_handler_handler_prepend(Eo *obj EFL_UNUSED,
                                            Core_Event_Message_Handler_Data *pd,
                                            int   type,
                                            void *func,
                                            void *data)
{
  Handler *h;

  if ((type < 0) || (type > pd->event_type_count) || (!func)) return NULL;
  h = calloc(1, sizeof(Handler));
  if (!h) return NULL;
  h->func = func;
  h->data = data;
  h->type = type;
  if (pd->current_event_type == type)
  {
    h->to_add        = EFL_TRUE;
    h->prepend       = EFL_TRUE;
    pd->handlers_add = eina_list_append(pd->handlers_add, h);
  }
  else
    pd->handlers[type] =
      eina_inlist_prepend(pd->handlers[type], EINA_INLIST_GET(h));
  return h;
}

EOLIAN static void *
_core_event_message_handler_handler_del(Eo *obj EFL_UNUSED,
                                        Core_Event_Message_Handler_Data *pd,
                                        void *handler)
{
  Handler *h = handler;
  void    *data;

  if (!h) return NULL;
  if ((h->type < 0) || (h->type > pd->event_type_count)) return NULL;
  data = h->data;
  if (pd->handlers_walking > 0)
  {
    if (h->to_add)
    {
      h->to_add        = EFL_FALSE;
      pd->handlers_add = eina_list_remove(pd->handlers_add, h);
    }

    h->delete_me        = EFL_TRUE;
    pd->handlers_delete = eina_list_append(pd->handlers_delete, h);
  }
  else
  {
    if (h->to_add) pd->handlers_add = eina_list_remove(pd->handlers_add, h);
    else
      pd->handlers[h->type] =
        eina_inlist_remove(pd->handlers[h->type], EINA_INLIST_GET(h));
    free(h);
  }
  return data;
}

EOLIAN static void *
_core_event_message_handler_handler_data_get(Eo *obj EFL_UNUSED,
                                             Core_Event_Message_Handler_Data *pd
                                                   EFL_UNUSED,
                                             void *handler)
{
  Handler *h = handler;

  if (!h) return NULL;
  return h->data;
}

EOLIAN static void *
_core_event_message_handler_handler_data_set(Eo *obj EFL_UNUSED,
                                             Core_Event_Message_Handler_Data *pd
                                                   EFL_UNUSED,
                                             void *handler,
                                             void *data)
{
  Handler *h = handler;
  void    *prev_data;

  if (!h) return NULL;
  prev_data = h->data;
  h->data   = data;
  return prev_data;
}

EOLIAN static void *
_core_event_message_handler_filter_add(Eo *obj EFL_UNUSED,
                                       Core_Event_Message_Handler_Data *pd,
                                       void *func_start,
                                       void *func_filter,
                                       void *func_end,
                                       void *data)
{
  Filter *f;

  if (!func_filter) return NULL;
  f = calloc(1, sizeof(Filter));
  if (!f) return NULL;
  f->func_start  = func_start;
  f->func_filter = func_filter;
  f->func_end    = func_end;
  f->data        = data;
  pd->filters    = eina_inlist_append(pd->filters, EINA_INLIST_GET(f));
  return f;
}

EOLIAN static void *
_core_event_message_handler_filter_del(Eo *obj EFL_UNUSED,
                                       Core_Event_Message_Handler_Data *pd,
                                       void                            *filter)
{
  Filter *f = filter;
  void   *data;

  if (!f) return NULL;
  data = f->data;
  if (pd->filters_walking > 0)
  {
    f->delete_me       = EFL_TRUE;
    pd->filters_delete = eina_list_append(pd->filters_delete, f);
  }
  else
  {
    pd->filters = eina_inlist_remove(pd->filters, EINA_INLIST_GET(f));
    free(f);
  }
  return data;
}

EOLIAN static int
_core_event_message_handler_current_type_get(
  Eo *obj                          EFL_UNUSED,
  Core_Event_Message_Handler_Data *pd)
{
  return pd->current_event_type;
}

EOLIAN static void *
_core_event_message_handler_current_event_get(
  Eo *obj                          EFL_UNUSED,
  Core_Event_Message_Handler_Data *pd)
{
  return pd->current_event_data;
}

EOLIAN static Efl_Object *
_core_event_message_handler_efl_object_constructor(
  Eo                              *obj,
  Core_Event_Message_Handler_Data *pd)
{
  obj                    = efl_constructor(efl_super(obj, MY_CLASS));
  pd->event_type_count   = 0;
  pd->current_event_type = -1;
  return obj;
}

EOLIAN static void
_core_event_message_handler_efl_object_destructor(
  Eo                              *obj,
  Core_Event_Message_Handler_Data *pd)
{
  Handler *h;
  int      i;

  if (pd->handlers_walking == 0)
  {
    EINA_LIST_FREE(pd->handlers_delete, h)
    {
      pd->handlers[h->type] =
        eina_inlist_remove(pd->handlers[h->type], EINA_INLIST_GET(h));
      free(h);
    }
    EINA_LIST_FREE(pd->handlers_add, h)
    {
      free(h);
    }
    for (i = 1; i <= pd->event_type_count; i++)
    {
      EINA_INLIST_FREE(pd->handlers[i], h)
      {
        pd->handlers[i] =
          eina_inlist_remove(pd->handlers[i], EINA_INLIST_GET(h));
        free(h);
      }
    }
    free(pd->handlers);
    pd->handlers = NULL;
  }
  else
  {
    ERR("Destruction of core_event_message_handler while walking events");
  }
  efl_destructor(efl_super(obj, MY_CLASS));
}

EOLIAN static void
_core_event_message_handler_efl_loop_message_handler_message_call(
  Eo                              *obj,
  Core_Event_Message_Handler_Data *pd,
  Efl_Loop_Message                *message)
{
  Handler    *h;
  int         type = -1;
  void       *data = NULL, *free_func = NULL, *free_data = NULL;
  Core_End_Cb fn_free = NULL;
  Eina_List  *l, *l2;
  int         handled = 0;

  // call legacy handlers which are controled by this class' custom api
  core_event_message_data_steal(message, &type, &data, &free_func, &free_data);
  if ((type >= 0) && (type <= pd->event_type_count))
  {
    if (free_func) fn_free = free_func;
    pd->current_event_data = data;
    pd->current_event_type = type;
    pd->handlers_walking++;
    EINA_INLIST_FOREACH(pd->handlers[type], h)
    {
      if (h->delete_me) continue;
      handled++;
      if (!h->func(h->data, h->type, data)) break;
    }
    pd->handlers_walking--;
    pd->current_event_data = NULL;
    pd->current_event_type = -1;
    EINA_LIST_FOREACH_SAFE(pd->handlers_add, l, l2, h)
    {
      if (h->type == type)
      {
        h->to_add        = EFL_FALSE;
        pd->handlers_add = eina_list_remove_list(pd->handlers_add, l);
        if (h->prepend)
          pd->handlers[type] =
            eina_inlist_prepend(pd->handlers[type], EINA_INLIST_GET(h));
        else
          pd->handlers[type] =
            eina_inlist_append(pd->handlers[type], EINA_INLIST_GET(h));
      }
    }
    if (pd->handlers_walking == 0)
    {
      EINA_LIST_FREE(pd->handlers_delete, h)
      {
        if (h->to_add) pd->handlers_add = eina_list_remove(pd->handlers_add, h);
        else
          pd->handlers[h->type] =
            eina_inlist_remove(pd->handlers[h->type], EINA_INLIST_GET(h));
        free(h);
      }
    }
    if ((type == CORE_EVENT_SIGNAL_EXIT) && (handled == 0))
    {
      Eo *loop = efl_provider_find(obj, EFL_LOOP_CLASS);
      if (loop) efl_loop_quit(loop, eina_value_int_init(0));
    }
  }

  efl_event_callback_call(obj,
                          CORE_EVENT_MESSAGE_HANDLER_EVENT_MESSAGE_ECORE_EVENT,
                          message);
  efl_loop_message_handler_message_call(efl_super(obj, MY_CLASS), message);

  if (data)
  {
    if (fn_free) fn_free(free_data, data);
    else free(data);
  }
}

static Efl_Bool
_flush_cb(void *data, void *handler EFL_UNUSED, void *message)
{
  int *type   = data;
  int  evtype = -1;

  if (!efl_isa(message, CORE_EVENT_MESSAGE_CLASS)) return EFL_TRUE;
  core_event_message_data_get(message, &evtype, NULL, NULL, NULL);
  return *type != evtype;
}

EOLIAN static void
_core_event_message_handler_type_flush(Eo *obj,
                                       Core_Event_Message_Handler_Data *pd
                                           EFL_UNUSED,
                                       int type)
{
  Eo            *loop      = efl_provider_find(obj, EFL_LOOP_CLASS);
  Efl_Loop_Data *loop_data = efl_data_scope_get(loop, EFL_LOOP_CLASS);

  if (loop && loop_data)
  {
    _efl_loop_messages_call(loop, loop_data, _flush_cb, &type);
  }
}

//////////////////////////////////////////////////////////////////////////

#include "core_event_message_handler.eo.c"
