#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include <Efl_Eo_Interfaces.h>
#include <Efl_Core.h>
#include <Efl_Eo.h>

#include "core_private.h"
#include "efl_loop_model.eo.h"

typedef struct _Efl_Loop_Model_Watcher_Data Efl_Loop_Model_Watcher_Data;

struct _Efl_Loop_Model_Watcher_Data
{
  const char   *property;
  Eina_Promise *p;
  Eo           *obj;
};

static void _propagate_future(void *data, const Efl_Event *event);

static void
_efl_loop_model_wathcer_free(Efl_Loop_Model_Watcher_Data *wd)
{
  efl_event_callback_del(wd->obj,
                         EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                         _propagate_future,
                         wd);
  eina_stringshare_del(wd->property);
  free(wd);
}

static void
_propagate_future(void *data, const Efl_Event *event)
{
  Efl_Model_Property_Event    *ev = event->info;
  const char                  *property;
  unsigned int                 i;
  Eina_Array_Iterator          it;
  Efl_Loop_Model_Watcher_Data *wd = data;

  EINA_ARRAY_ITER_NEXT(ev->changed_properties, i, property, it)
  if (property == wd->property || eina_streq(property, wd->property))
  {
    Eina_Value *v = efl_model_property_get(wd->obj, wd->property);

    if (eina_value_type_get(v) == EINA_VALUE_TYPE_ERROR)
    {
      Eina_Error err = 0;

      eina_value_get(v, &err);

      if (err == EAGAIN) return; // Not ready yet

      eina_promise_reject(wd->p, err);
    }
    else
    {
      eina_promise_resolve(wd->p, eina_value_reference_copy(v));
    }

    eina_value_free(v);
    _efl_loop_model_wathcer_free(wd);
    break;
  }
}

static void
_event_cancel(void *data, const Eina_Promise *dead_ptr EFL_UNUSED)
{
  _efl_loop_model_wathcer_free(data);
}

static Eina_Future *
_efl_loop_model_efl_model_property_ready_get(Eo         *obj,
                                             void *pd    EFL_UNUSED,
                                             const char *property)
{
  Eina_Value  *value = efl_model_property_get(obj, property);
  Eina_Future *f;

  if (eina_value_type_get(value) == EINA_VALUE_TYPE_ERROR)
  {
    Eina_Error err = 0;

    eina_value_get(value, &err);
    eina_value_free(value);

    if (err == EAGAIN)
    {
      Efl_Loop_Model_Watcher_Data *wd =
        calloc(1, sizeof(Efl_Loop_Model_Watcher_Data));

      wd->obj      = obj;
      wd->property = eina_stringshare_add(property);
      wd->p =
        eina_promise_new(efl_loop_future_scheduler_get(obj), _event_cancel, wd);

      efl_event_callback_add(obj,
                             EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                             _propagate_future,
                             wd);
      return efl_future_then(obj, eina_future_new(wd->p));
    }

    return eina_future_rejected(efl_loop_future_scheduler_get(obj), err);
  }
  f = eina_future_resolved(efl_loop_future_scheduler_get(obj),
                           eina_value_reference_copy(value));
  eina_value_free(value);
  return efl_future_then(obj, f);
}

static Eina_Value
_unpack_from_array(void *data           EFL_UNUSED,
                   Eina_Value           v,
                   const Eina_Future *f EFL_UNUSED)
{
  Eo *object = NULL;

  if (eina_value_type_get(&v) == EINA_VALUE_TYPE_ERROR) return v;
  if (eina_value_type_get(&v) != EINA_VALUE_TYPE_ARRAY)
    return eina_value_error_init(EINVAL);
  if (eina_value_array_count(&v) != 1) return eina_value_error_init(EINVAL);

  eina_value_array_get(&v, 0, &object);

  return eina_value_object_init(object);
}

static Eina_Future *
_efl_loop_model_efl_model_children_index_get(Eo            *obj,
                                             void *pd       EFL_UNUSED,
                                             Eina_Iterator *indexes)
{
  Eina_Future *r;
  Eina_Array   futures;
  unsigned int idx;

  eina_array_step_set(&futures, sizeof(Eina_Array), 8);

  EINA_ITERATOR_FOREACH(indexes, idx)
    eina_array_push(&futures,
                    eina_future_then(efl_model_children_slice_get(obj, idx, 1),
                                     _unpack_from_array,
                                     NULL,
                                     NULL));
  eina_iterator_free(indexes);

  r =
    efl_future_then(obj,
                    eina_future_all_iterator(eina_array_iterator_new(&futures)),
                    .success      = _efl_future_all_repack,
                    .success_type = EINA_VALUE_TYPE_ARRAY);

  eina_array_flush(&futures);

  return r;
}

static void
_noref_death(void *data EFL_UNUSED, const Efl_Event *event)
{
  efl_event_callback_del(event->object, EFL_EVENT_NOREF, _noref_death, NULL);
   // For safety reason and in case multiple call to volatile has been made
    // we check that there is still a parent at this point in EFL_EVENT_NOREF
  efl_del(event->object);
}

static void
_efl_loop_model_volatile_make(Eo *obj, void *pd EFL_UNUSED)
{
  // Just to make sure we do not double register this callback, we first remove
  // any potentially previous one.
  efl_event_callback_del(obj, EFL_EVENT_NOREF, _noref_death, NULL);
  efl_event_callback_add(obj, EFL_EVENT_NOREF, _noref_death, NULL);
}

static Eina_Future *
_efl_loop_model_efl_model_property_set(Eo         *obj,
                                       void *pd    EFL_UNUSED,
                                       const char *property,
                                       Eina_Value *value)
{
  Eina_Error err;

  if (!value)
    return efl_loop_future_rejected(obj, EFL_MODEL_ERROR_INCORRECT_VALUE);
  err = efl_property_reflection_set(obj, property, *value);
  if (err) return efl_loop_future_rejected(obj, err);

  return efl_loop_future_resolved(obj,
                                  efl_property_reflection_get(obj, property));
}

static Eina_Value *
_efl_loop_model_efl_model_property_get(const Eo   *obj,
                                       void *pd    EFL_UNUSED,
                                       const char *property)
{
  Eina_Value *r;
  Eina_Value  direct;

  direct = efl_property_reflection_get(obj, property);
  r      = eina_value_dup(&direct);
  eina_value_flush(&direct);

  return r;
}

static void
_efl_loop_model_efl_object_invalidate(Eo *obj, void *pd EFL_UNUSED)
{
  efl_event_callback_del(obj, EFL_EVENT_NOREF, _noref_death, NULL);

  efl_invalidate(efl_super(obj, EFL_LOOP_MODEL_CLASS));
}

#include "efl_loop_model.eo.c"
