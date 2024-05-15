#include "Efl_Eo.h"
#include "function_overrides_simple.h"
#include "function_overrides_inherit.h"
#include "function_overrides_inherit2.h"
#include "function_overrides_inherit3.h"

#include "../eunit_tests.h"

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
   efl_object_init();

   return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

TEST(function_override)
{
   efl_object_init();

   Efl_Bool called = EFL_FALSE;
   Eo *obj = efl_add_ref(INHERIT2_CLASS, NULL);

   simple_a_set(obj, 1);
   Simple_Public_Data *pd = efl_data_scope_get(obj, SIMPLE_CLASS);
   efl_assert_fail_if(pd->a != 2);

   efl_unref(obj);

   obj = efl_add_ref(INHERIT3_CLASS, NULL);

   simple_a_set(obj, 1);
   pd = efl_data_scope_get(obj, SIMPLE_CLASS);
   efl_assert_fail_if(pd->a != 3);

   efl_unref(obj);

   obj = efl_add_ref(INHERIT2_CLASS, NULL);
   called = inherit2_print(obj);
   efl_assert_fail_if(!called);
   called = inherit2_print(obj);
   called = inherit2_print(obj);
   efl_assert_fail_if(!called);
   efl_unref(obj);

   obj = efl_add_ref(SIMPLE_CLASS, NULL);
   called = inherit2_print(obj);
   efl_assert_fail_if(called);

#ifdef EO_DEBUG
   called = simple_class_print(obj);
   efl_assert_fail_if(!called);
#endif

   called = simple_class_print(SIMPLE_CLASS);
   efl_assert_fail_if(called);

   called = simple_class_print(INHERIT_CLASS);
   efl_assert_fail_if(called);

   called = simple_class_print(INHERIT2_CLASS);
   efl_assert_fail_if(called);

   called = simple_class_print(INHERIT3_CLASS);
   efl_assert_fail_if(called);

#ifdef EO_DEBUG
   called = simple_a_print(SIMPLE_CLASS);
   efl_assert_fail_if(called);
#endif

   efl_constructor(efl_super(obj, SIMPLE_CLASS));
   efl_destructor(efl_super(obj, SIMPLE_CLASS));

   efl_unref(obj);

   efl_object_shutdown();
}

