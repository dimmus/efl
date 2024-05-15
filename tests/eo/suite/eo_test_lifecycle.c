#include <stdio.h>

#include <Efl_Eo.h>

#include "eo_suite.h"
#include "eo_test_class_simple.h"

TEST(eo_test_base_del)
{
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);

   efl_del(obj);

   efl_assert_ptr_eq(efl_class_name_get(obj), NULL);

}

TEST(eo_test_base_unref)
{
   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);

   efl_unref(obj);

   efl_assert_ptr_eq(efl_class_name_get(obj), NULL);

}

typedef struct {
   unsigned int time, del_time, invalidate_time, noref_time, destruct_time;
} Helper;

static void
_invalidate(void *data, const Efl_Event *ev)
{
   Helper *help = data;

   efl_assert_ptr_ne(efl_parent_get(ev->object), NULL);

   help->invalidate_time = help->time;
   help->time ++;
}

static void
_noref(void *data, const Efl_Event *ev EFL_UNUSED)
{
   Helper *help = data;

   help->noref_time = help->time;
   help->time ++;
}

static void
_del(void *data, const Efl_Event *ev)
{
   Helper *help = data;

   efl_assert_ptr_eq(efl_parent_get(ev->object), NULL);

   help->del_time = help->time;
   help->time ++;
}

static void
_destruct(void *data, const Efl_Event *ev EFL_UNUSED)
{
   Helper *help = data;

   help->destruct_time = help->time;
   help->time ++;
}

TEST(eo_test_shutdown_eventting)
{
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);
   Helper data;

   data.time = 1;
   data.del_time = 0;
   data.invalidate_time = 0;
   data.noref_time = 0;
   data.destruct_time = 0;

   efl_event_callback_add(obj, EFL_EVENT_DEL, _del, &data);
   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate, &data);
   efl_event_callback_add(obj, EFL_EVENT_NOREF, _noref, &data);
   efl_event_callback_add(obj, EFL_EVENT_DESTRUCT, _destruct, &data);

   efl_del(obj);

   //documented assertions:
   efl_assert_int_ne(data.del_time, 0);
   efl_assert_int_ne(data.invalidate_time, 0);
   efl_assert_int_ne(data.noref_time, 0);
   efl_assert_int_ne(data.destruct_time, 0);

   //everything happened before destruct
   efl_assert_int_le(data.del_time, data.destruct_time);
   efl_assert_int_le(data.invalidate_time, data.destruct_time);
   efl_assert_int_le(data.noref_time, data.destruct_time);

   //invalidate has to happen before the destructor
   efl_assert_int_le(data.invalidate_time, data.destruct_time);

   efl_assert_ptr_eq(efl_class_name_get(obj), NULL);

}

static void
_noref2(void *data EFL_UNUSED, const Efl_Event *ev)
{
   //correctly delete this
   efl_del(ev->object);
   //error unref
   efl_unref(ev->object);
}

TEST(eo_test_del_in_noref)
{
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);

   efl_event_callback_add(obj, EFL_EVENT_NOREF, _noref2, NULL);

   efl_ref(obj);
   DISABLE_ABORT_ON_CRITICAL_START;
   efl_unref(obj); //this fires noref
   DISABLE_ABORT_ON_CRITICAL_END;

   efl_assert_ptr_eq(efl_class_name_get(obj), NULL);

}

static void
_noref3(void *data EFL_UNUSED, const Efl_Event *ev)
{
   //this is a wrong fixme
   efl_unref(ev->object);
}

TEST(eo_test_unref_noref)
{
   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);

   efl_event_callback_add(obj, EFL_EVENT_NOREF, _noref3, NULL);

   DISABLE_ABORT_ON_CRITICAL_START;
   efl_unref(obj);
   DISABLE_ABORT_ON_CRITICAL_END;

   efl_assert_ptr_eq(efl_class_name_get(obj), NULL);

}

typedef struct {
   int shared, unique, invalidate;
} OwnershipEventsCounter;

static void
_ownership_shared_event(void *data, const Efl_Event *ev EFL_UNUSED)
{
   OwnershipEventsCounter *counter = data;
   ++(counter->shared);
}

static void
_ownership_unique_event(void *data, const Efl_Event *ev EFL_UNUSED)
{
   OwnershipEventsCounter *counter = data;
   ++(counter->unique);
}

static void
_invalidate_ownership_event(void *data, const Efl_Event *ev EFL_UNUSED)
{
   OwnershipEventsCounter *counter = data;
   ++(counter->invalidate);
}

TEST(eo_test_ownership_events)
{
   OwnershipEventsCounter counter = {0,};
   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);

   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_SHARED, _ownership_shared_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_UNIQUE, _ownership_unique_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate_ownership_event, &counter);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 0);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 2);
   efl_assert_int_eq(counter.invalidate, 0);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 2);
   efl_assert_int_eq(counter.invalidate, 1);
}

TEST(eo_test_ownership_events_with_parent)
{
   OwnershipEventsCounter counter = {0,};
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);

   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_SHARED, _ownership_shared_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_UNIQUE, _ownership_unique_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate_ownership_event, &counter);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 0);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 2);
   efl_assert_int_eq(counter.invalidate, 0);

   efl_del(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 2);
   efl_assert_int_eq(counter.invalidate, 1);

   efl_unref(par);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 2);
}

TEST(eo_test_ownership_events_with_parent_invalidate)
{
   OwnershipEventsCounter counter = {0,};
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);

   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_SHARED, _ownership_shared_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_UNIQUE, _ownership_unique_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate_ownership_event, &counter);

   /* Kill parent */
   efl_unref(par);
   efl_assert_int_eq(counter.shared, 0);
   efl_assert_int_eq(counter.unique, 0);
   efl_assert_int_eq(counter.invalidate, 1);
}

TEST(eo_test_ownership_events_with_parent_invalidate2)
{
   OwnershipEventsCounter counter = {0,};
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);

   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_SHARED, _ownership_shared_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_UNIQUE, _ownership_unique_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate_ownership_event, &counter);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 0);
   efl_assert_int_eq(counter.invalidate, 0);

   /* Kill parent */
   efl_unref(par);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);
   efl_assert_int_eq(counter.invalidate, 1);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);
   efl_assert_int_eq(counter.invalidate, 1);
}

TEST(eo_test_ownership_events_with_parent_invalidate3)
{
   OwnershipEventsCounter counter = {0,};
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);

   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_SHARED, _ownership_shared_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_UNIQUE, _ownership_unique_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate_ownership_event, &counter);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 0);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_assert_int_eq(counter.invalidate, 0);

   /* Kill parent */
   efl_unref(par);
   efl_assert_int_eq(counter.shared, 2);
   efl_assert_int_eq(counter.unique, 1);
   efl_assert_int_eq(counter.invalidate, 1);
}

TEST(eo_test_ownership_events_self_invalidate)
{
   OwnershipEventsCounter counter = {0,};
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);

   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_SHARED, _ownership_shared_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_OWNERSHIP_UNIQUE, _ownership_unique_event, &counter);
   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate_ownership_event, &counter);

   efl_assert_int_eq(counter.shared, 0);
   efl_assert_int_eq(counter.unique, 0);
   efl_assert_int_eq(counter.invalidate, 0);

   efl_ref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 0);
   efl_assert_int_eq(counter.invalidate, 0);

   efl_del(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);
   efl_assert_int_eq(counter.invalidate, 1);

   /* Kill parent */
   efl_unref(par);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);
   efl_assert_int_eq(counter.invalidate, 1);

   efl_unref(obj);
   efl_assert_int_eq(counter.shared, 1);
   efl_assert_int_eq(counter.unique, 1);
   efl_assert_int_eq(counter.invalidate, 1);
}

typedef struct {
  Eo *par;
  Efl_Bool called;
} Invalidating_Test_Helper;

static void
_invalidate2(void *data, const Efl_Event *ev EFL_UNUSED)
{
   Invalidating_Test_Helper *iev = data;

   iev->called = EFL_TRUE;
   efl_assert_int_eq(efl_invalidating_get(iev->par), EFL_TRUE);
   efl_assert_int_eq(efl_invalidated_get(iev->par), EFL_FALSE);
}

TEST(eo_test_invalidating_get)
{
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par);
   Invalidating_Test_Helper data = {par, EFL_FALSE};

   efl_event_callback_add(obj, EFL_EVENT_INVALIDATE, _invalidate2, &data);

   efl_unref(par);
   efl_assert_int_eq(data.called, EFL_TRUE);
}

TEST(eo_test_alive_get)
{
   Efl_Bool res;
   Eo *par = efl_add_ref(SIMPLE_CLASS, NULL);
   Eo *obj = efl_add(SIMPLE_CLASS, par,
      res = efl_alive_get(efl_added)
   );
   efl_assert_int_eq(res, 0);
   efl_assert_int_eq(efl_alive_get(obj), 1);
   efl_ref(obj);
   efl_assert_int_eq(efl_alive_get(obj), 1);
   efl_del(obj);
   efl_assert_int_eq(efl_alive_get(obj), 0);
   efl_unref(obj);
   efl_assert_ptr_eq(efl_class_name_get(obj), NULL);

}
