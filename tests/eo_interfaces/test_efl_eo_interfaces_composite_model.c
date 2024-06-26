/* EFL - EFL library
 * Copyright (C) 2013 Cedric Bail
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "efl_eo_interfaces_suite.h"

#include <Efl_Eo_Interfaces.h>
#include <Efl_Core.h>

static const int child_number = 7;
static const int base_ints[]  = { 41, 42, 43, 44, 45, 46, 47 };

static void
_cleanup(Eo *o                          EFL_UNUSED,
         void *data                     EFL_UNUSED,
         const Eina_Future *dead_future EFL_UNUSED)
{
  core_main_loop_quit();
}

static Eina_Value
_children_slice_get_then(Eo *o            EFL_UNUSED,
                         void *data       EFL_UNUSED,
                         const Eina_Value v)
{
  unsigned int  i, len;
  Efl_Model    *child;
  Eina_Future **all;

  all = alloca(sizeof(Eina_Future *) * (eina_value_array_count(&v) + 1));

  EINA_VALUE_ARRAY_FOREACH(&v, len, i, child)
  {
    Eina_Value *p_int   = NULL;
    Eina_Value *p_true  = NULL;
    Eina_Value *p_false = NULL;
    int         v_int   = -1;
    Efl_Bool    v_true  = EFL_FALSE;
    Efl_Bool    v_false = EFL_TRUE;

    p_int   = efl_model_property_get(child, "test_p_int");
    p_true  = efl_model_property_get(child, "test_p_true");
    p_false = efl_model_property_get(child, "test_p_false");

    fail_if(!eina_value_int_get(p_int, &v_int));
    fail_if(!eina_value_bool_get(p_true, &v_true));
    fail_if(!eina_value_bool_get(p_false, &v_false));

    all[i] = efl_model_property_set(child,
                                    "test_odd_even",
                                    v_int & 0x1 ? p_true : p_false);

    fail_if(v_int != base_ints[i]);
    fail_if(v_true != EFL_TRUE);
    fail_if(v_false != EFL_FALSE);

    eina_value_free(p_int);
    eina_value_free(p_true);
    eina_value_free(p_false);
  }
  all[i] = EINA_FUTURE_SENTINEL;

  return eina_future_as_value(eina_future_all_array(all));
}

static Eina_Value
_children_odd_even_ready(Eo                *model,
                         void *data         EFL_UNUSED,
                         const Eina_Value v EFL_UNUSED)
{
  return eina_future_as_value(
    efl_model_children_slice_get(model, 0, child_number));
}

static Eina_Value
_children_odd_even_check(Eo *model, void *data EFL_UNUSED, const Eina_Value v)
{
  unsigned int i, len;
  Efl_Model   *child, *first_child = NULL;

  EINA_VALUE_ARRAY_FOREACH(&v, len, i, child)
  {
    Eina_Value *p_odd_even = NULL;
    Eina_Value *p_int      = NULL;
    Efl_Bool    v_odd_even = EFL_TRUE;
    int         v_int      = -1;

    if (!first_child) first_child = child;

    p_int      = efl_model_property_get(child, "test_p_int");
    p_odd_even = efl_model_property_get(child, "test_odd_even");

    fail_if(!eina_value_bool_get(p_odd_even, &v_odd_even));
    fail_if(!eina_value_int_get(p_int, &v_int));

    fail_if(v_int & 0x1 ? !v_odd_even : v_odd_even);

    eina_value_free(p_int);
    eina_value_free(p_odd_even);
  }

  efl_model_child_del(model, first_child);

  return eina_future_as_value(
    efl_model_children_slice_get(model, 0, child_number - 1));
}

static Eina_Value
_children_odd_even_check_after_removal(Eo *model        EFL_UNUSED,
                                       void *data       EFL_UNUSED,
                                       const Eina_Value v)
{
  unsigned int i, len;
  Efl_Model   *child;

  EINA_VALUE_ARRAY_FOREACH(&v, len, i, child)
  {
    Eina_Value *p_odd_even = NULL;
    Eina_Value *p_int      = NULL;
    Efl_Bool    v_odd_even = EFL_TRUE;
    int         v_int      = -1;

    p_int      = efl_model_property_get(child, "test_p_int");
    p_odd_even = efl_model_property_get(child, "test_odd_even");

    fail_if(!eina_value_bool_get(p_odd_even, &v_odd_even));
    fail_if(!eina_value_int_get(p_int, &v_int));

    fail_if(v_int & 0x1 ? !v_odd_even : v_odd_even);
    fail_if(v_int != base_ints[i + 1]);

    eina_value_free(p_int);
    eina_value_free(p_odd_even);
  }

  return v;
}

static Eina_Value
_assert_on_error(Eo *o EFL_UNUSED, void *data EFL_UNUSED, Eina_Error error)
{
  ck_abort_msg(
    "Assert on error triggered duing Boolean Model test with error '%s'.\n",
    eina_error_msg_get(error));
  return eina_value_error_init(error);
}

static unsigned int request[3] = { 3, 1, 5 };

static Eina_Value
_children_random_index(Eo *model        EFL_UNUSED,
                       void *data       EFL_UNUSED,
                       const Eina_Value v)
{
  unsigned int i, len;
  Efl_Model   *child;

  EINA_VALUE_ARRAY_FOREACH(&v, len, i, child)
  {
    unsigned int index;

    index = efl_composite_model_index_get(child);
    ck_assert_int_eq(index, request[i]);
  }

  return v;
}

EFL_START_TEST(efl_test_boolean_model)
{
  Efl_Generic_Model *base_model, *child;
  int                i;
  Eina_Value         v = { 0 };
  Efl_Boolean_Model *model;
  Eina_Future       *future;

  eina_value_setup(&v, EINA_VALUE_TYPE_INT);

  base_model = efl_add_ref(EFL_GENERIC_MODEL_CLASS, efl_main_loop_get());
  ck_assert(!!base_model);

  for (i = 0; i < child_number; ++i)
  {
    child = efl_model_child_add(base_model);
    ck_assert(!!child);
    ck_assert(eina_value_set(&v, base_ints[i]));
    efl_model_property_set(child, "test_p_int", &v);
  }

  model = efl_add_ref(
    EFL_BOOLEAN_MODEL_CLASS,
    efl_main_loop_get(),
    efl_ui_view_model_set(efl_added, base_model),
    efl_boolean_model_boolean_add(efl_added, "test_p_true", EFL_TRUE),
    efl_boolean_model_boolean_add(efl_added, "test_p_false", EFL_FALSE),
    efl_boolean_model_boolean_add(efl_added, "test_odd_even", EFL_FALSE));
  ck_assert(!!model);

  future =
    efl_model_children_slice_get(model, 0, efl_model_children_count_get(model));
  future = efl_future_then(model,
                           future,
                           .success      = _children_slice_get_then,
                           .success_type = EINA_VALUE_TYPE_ARRAY);
  future = efl_future_then(model,
                           future,
                           .success      = _children_odd_even_ready,
                           .success_type = EINA_VALUE_TYPE_ARRAY);
  future = efl_future_then(model,
                           future,
                           .success      = _children_odd_even_check,
                           .success_type = EINA_VALUE_TYPE_ARRAY);
  future = efl_future_then(model,
                           future,
                           .success = _children_odd_even_check_after_removal,
                           .success_type = EINA_VALUE_TYPE_ARRAY);
  future =
    efl_future_then(model, future, .error = _assert_on_error, .free = _cleanup);

  core_main_loop_begin();

  future =
    efl_model_children_index_get(model, EINA_C_ARRAY_ITERATOR_NEW(request));
  future = efl_future_then(model,
                           future,
                           .success      = _children_random_index,
                           .success_type = EINA_VALUE_TYPE_ARRAY);
  future =
    efl_future_then(model, future, .error = _assert_on_error, .free = _cleanup);

  core_main_loop_begin();
}

EFL_END_TEST

typedef struct _Efl_Filter_Model_Wait Efl_Filter_Model_Wait;

struct _Efl_Filter_Model_Wait
{
  Efl_Model    *child;
  Efl_Model    *parent;
  Eina_Promise *p;
};

static void _wait_for_it(void *data, const Efl_Event *event);

static void
_wait_cleanup(Efl_Filter_Model_Wait *mw)
{
  efl_event_callback_del(mw->child,
                         EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                         _wait_for_it,
                         mw);
  efl_unref(mw->child);
  efl_unref(mw->parent);
  free(mw);
}

static void
_wait_for_it(void *data, const Efl_Event *event)
{
  Efl_Model_Property_Event *ev = event->info;
  Efl_Filter_Model_Wait    *mw = data;
  const char               *property;
  unsigned int              i;
  Eina_Array_Iterator       it;

  EINA_ARRAY_ITER_NEXT(ev->changed_properties, i, property, it)
  {
    if (!strcmp(property, "ready"))
    {
      Eina_Value *value = efl_model_property_get(event->object, "ready");
      Efl_Bool    r;
      Eina_Error  err = 0;

      if (eina_value_type_get(value) == EINA_VALUE_TYPE_BOOL &&
          eina_value_bool_get(value, &r) && r)
      {
        eina_value_free(value);
        eina_promise_resolve(mw->p, eina_value_bool_init(EFL_TRUE));
        _wait_cleanup(mw);
        return;
      }
      if (eina_value_type_get(value) == EINA_VALUE_TYPE_ERROR &&
          eina_value_error_get(value, &err) && err != EAGAIN)
      {
        eina_value_free(value);
        eina_promise_reject(mw->p, err);
        _wait_cleanup(mw);
        return;
      }
      eina_value_free(value);
      break;
    }
  }
}

static void
_wait_cancel(void *data, const Eina_Promise *dead_ptr EFL_UNUSED)
{
  _wait_cleanup(data);
}

static Eina_Future *
_wait_ready_true(Efl_Model *parent, Efl_Model *child)
{
  Efl_Filter_Model_Wait *mw;
  Eina_Value            *value = efl_model_property_get(child, "ready");
  Efl_Bool               r     = EFL_FALSE;
  Eina_Error             err   = 0;

  if (eina_value_type_get(value) == EINA_VALUE_TYPE_BOOL &&
      eina_value_bool_get(value, &r) && r)
  {
    eina_value_free(value);
    return efl_loop_future_resolved(parent, eina_value_bool_init(EFL_TRUE));
  }
  if (eina_value_type_get(value) == EINA_VALUE_TYPE_ERROR &&
      eina_value_error_get(value, &err) && err != EAGAIN)
  {
    eina_value_free(value);
    return efl_loop_future_rejected(parent, err);
  }

  eina_value_free(value);

  mw = calloc(1, sizeof(Efl_Filter_Model_Wait));
  if (!mw) return efl_loop_future_rejected(parent, ENOMEM);

  mw->p =
    eina_promise_new(efl_loop_future_scheduler_get(parent), _wait_cancel, mw);
  mw->child  = efl_ref(child);
  mw->parent = efl_ref(parent);

  efl_event_callback_add(child,
                         EFL_MODEL_EVENT_PROPERTIES_CHANGED,
                         _wait_for_it,
                         mw);

  return efl_future_then(child, eina_future_new(mw->p));
}

static Eina_Future *
_filter_cb(void *data EFL_UNUSED, Efl_Filter_Model *parent, Efl_Model *child)
{
  return _wait_ready_true(parent, child);
}

static Eina_Value
_fetch_and_switch(Eo *o EFL_UNUSED, void *data EFL_UNUSED, const Eina_Value v)
{
  unsigned int  i, len;
  Eo           *target = NULL;
  Eina_Future **all;

  all = calloc(1, sizeof(Eina_Future *) * (eina_value_array_count(&v) + 1));
  if (!all) return eina_value_error_init(ENOMEM);

  EINA_VALUE_ARRAY_FOREACH(&v, len, i, target)
  {
    all[i] =
      efl_model_property_set(target, "ready", eina_value_bool_new(EFL_TRUE));
  }
  all[i] = EINA_FUTURE_SENTINEL;

  return eina_future_as_value(eina_future_all_array(all));
}

static void
_count_changed(void *data, const Efl_Event *ev)
{
  Eina_Promise *p = data;

  if (efl_model_children_count_get(ev->object) != (unsigned int)child_number)
    return;

  efl_event_callback_del(ev->object,
                         EFL_MODEL_EVENT_CHILDREN_COUNT_CHANGED,
                         _count_changed,
                         p);
  eina_promise_resolve(p, eina_value_bool_init(EFL_TRUE));
}

static void
_wait_count_cancel(void *data, const Eina_Promise *dead_ptr EFL_UNUSED)
{
  Eo *filter = data;

  efl_unref(filter);
}

static Eina_Value
_wait_count(Eo *filter, void *data EFL_UNUSED, const Eina_Value v EFL_UNUSED)
{
  Eina_Promise *p;
  Eina_Future  *racing[3];
  Eina_Future  *race;

  p = eina_promise_new(efl_loop_future_scheduler_get(filter),
                       _wait_count_cancel,
                       efl_ref(filter));
  efl_event_callback_add(filter,
                         EFL_MODEL_EVENT_CHILDREN_COUNT_CHANGED,
                         _count_changed,
                         p);

  racing[0] = eina_future_new(p);
  racing[1] = efl_loop_timeout(efl_loop_get(filter), 0.3);
  racing[2] = EINA_FUTURE_SENTINEL;

  race = eina_future_race_array(racing);

  return eina_future_as_value(race);
}

static Eina_Value
_count_and_fetch(Eo                *filter,
                 void *data         EFL_UNUSED,
                 const Eina_Value v EFL_UNUSED)
{
  ck_assert_int_eq(efl_model_children_count_get(filter), child_number);

  return eina_future_as_value(
    efl_model_children_slice_get(filter, 0, child_number));
}

static Eina_Value
_check_index(Eo *o EFL_UNUSED, void *data EFL_UNUSED, const Eina_Value v)
{
  unsigned int i, len;
  Eo          *target = NULL;

  EINA_VALUE_ARRAY_FOREACH(&v, len, i, target)
  {
    Eina_Value *p_original;
    uint64_t    idx, original;

    idx        = efl_composite_model_index_get(target);
    p_original = efl_model_property_get(target, "original");
    fail_if(!eina_value_uint64_convert(p_original, &original));
    ck_assert(original < (uint64_t)child_number);
    ck_assert(idx < (uint64_t)child_number);

    eina_value_free(p_original);
  }

  return v;
}

EFL_START_TEST(efl_test_filter_model)
{
  Efl_Generic_Model *base_model, *child;
  int                i;
  Eina_Value         v = { 0 };
  Efl_Boolean_Model *model;
  Efl_Boolean_Model *filter;
  Eina_Future       *future;

  eina_value_setup(&v, EINA_VALUE_TYPE_INT);

  base_model = efl_add_ref(EFL_GENERIC_MODEL_CLASS, efl_main_loop_get());
  ck_assert(!!base_model);

  for (i = 0; i < child_number; ++i)
  {
    child = efl_model_child_add(base_model);
    ck_assert(!!child);
    ck_assert(eina_value_set(&v, base_ints[i]));
    efl_model_property_set(child, "test_p_int", &v);
    efl_model_property_set(child, "original", eina_value_int_new(i));
  }

  model = efl_add_ref(
    EFL_BOOLEAN_MODEL_CLASS,
    efl_main_loop_get(),
    efl_ui_view_model_set(efl_added, base_model),
    efl_boolean_model_boolean_add(efl_added, "test_p_true", EFL_TRUE),
    efl_boolean_model_boolean_add(efl_added, "test_p_false", EFL_FALSE),
    efl_boolean_model_boolean_add(efl_added, "ready", EFL_FALSE));
  ck_assert(!!model);

  filter =
    efl_add_ref(EFL_FILTER_MODEL_CLASS,
                efl_main_loop_get(),
                efl_ui_view_model_set(efl_added, model),
                efl_filter_model_filter_set(efl_added, NULL, _filter_cb, NULL));
  ck_assert(!!filter);
  ck_assert_int_eq(efl_model_children_count_get(filter), 0);

  future =
    efl_model_children_slice_get(model, 0, efl_model_children_count_get(model));
  future = efl_future_then(model, future, .success = _fetch_and_switch);
  future = efl_future_then(filter, future, .success = _wait_count);
  future = efl_future_then(filter, future, .success = _count_and_fetch);
  future =
    efl_future_then(filter, future, .success = _check_index, .free = _cleanup);

  core_main_loop_begin();

  efl_unref(filter);
  efl_del(filter);
}

EFL_END_TEST

void
efl_test_case_boolean_model(TCase *tc)
{
  tcase_add_test(tc, efl_test_boolean_model);
  tcase_add_test(tc, efl_test_filter_model);
}
