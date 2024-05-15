#include "Efl_Eo.h"
#include "composite_objects_simple.h"
#include "composite_objects_comp.h"

#include "../eunit_tests.h"

#include "efl-test-runner.h"

static int cb_called = EFL_FALSE;

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
   efl_object_init();

   return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

static void
_a_changed_cb(void *data, const Efl_Event *event)
{
   int new_a = *((int *) event->info);
   testlog("%s event_info:'%d' data:'%s'\n", __func__, new_a, (const char *) data);

   cb_called = EFL_TRUE;
}

TEST(composite_objects)
{
   efl_object_init();

   Eo *obj = efl_add_ref(COMP_CLASS, NULL);
   efl_event_callback_add(obj, EV_A_CHANGED, _a_changed_cb, NULL);

   efl_assert_fail_if(!efl_isa(obj, COMP_CLASS));
   efl_assert_fail_if(!efl_isa(obj, SIMPLE_CLASS));

   int a = 0;
   cb_called = EFL_FALSE;
   simple_a_set(obj, 1);
   efl_assert_fail_if(!cb_called);

   /* Test functions from all across the chain. */
   cb_called = EFL_FALSE;
   simple_a_set1(obj, 1);
   efl_assert_fail_if(!cb_called);
   cb_called = EFL_FALSE;
   simple_a_set15(obj, 1);
   efl_assert_fail_if(!cb_called);
   cb_called = EFL_FALSE;
   simple_a_set32(obj, 1);
   efl_assert_fail_if(!cb_called);

   a = simple_a_get(obj);
   efl_assert_fail_if(a != 1);

   /* disable the callback forwarder, and fail if it's still called. */
   Eo *simple = NULL;
   simple = efl_key_data_get(obj, "simple-obj");
   efl_ref(simple);
   efl_event_callback_forwarder_del(simple, EV_A_CHANGED, obj);

   cb_called = EFL_FALSE;
   simple_a_set(obj, 2);
   efl_assert_fail_if(cb_called);

   efl_assert_fail_if(!efl_composite_part_is(simple));
   efl_assert_fail_if(!efl_composite_detach(obj, simple));
   efl_assert_fail_if(efl_composite_detach(obj, simple));
   efl_assert_fail_if(efl_composite_part_is(simple));
   efl_assert_fail_if(!efl_composite_attach(obj, simple));
   efl_assert_fail_if(!efl_composite_part_is(simple));
   efl_assert_fail_if(efl_composite_attach(obj, simple));

   efl_unref(simple);
   efl_unref(obj);

   efl_object_shutdown();
}

