#include <stdio.h>

#include <Efl_Eo.h>

#include "eo_suite.h"
#include "eo_error_msgs.h"
#include "eo_test_class_simple.h"

static struct log_ctx ctx;

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
   efl_object_init();

   return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

TEST(eo_pure_virtual_fct_call)
{
   eina_log_print_cb_set(eo_test_print_cb, &ctx);

   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);
   efl_assert_fail_if(!obj);

   TEST_EO_ERROR("_efl_object_call_resolve", "in %s:%d: you called a pure virtual func '%s' (%d) of class '%s'.");
   simple_pure_virtual(obj);
   assert(ctx.did);

   efl_unref(obj);
   eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
}

TEST(eo_api_not_implemented_call)
{
   eina_log_print_cb_set(eo_test_print_cb, &ctx);

   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);
   efl_assert_fail_if(!obj);

   TEST_EO_ERROR("simple_no_implementation", "Unable to resolve op for api func %p for obj=%p (%s)");
   simple_no_implementation(obj);
   assert(ctx.did);

   efl_unref(obj);
   eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
}

TEST(eo_op_not_found_in_super)
{
   eina_log_print_cb_set(eo_test_print_cb, &ctx);

   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);
   efl_assert_fail_if(!obj);

   TEST_EO_ERROR("_efl_object_call_resolve", "in %s:%d: func '%s' (%d) could not be resolved on %s for class '%s' for super of '%s'.");
   simple_a_set(efl_super(obj, SIMPLE_CLASS), 10);
   assert(ctx.did);

   efl_unref(obj);
   eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
}

//the fallback code that will be called

static Efl_Bool fallback_called;

static void
test_func(void)
{
   fallback_called = EFL_TRUE;
}

//implementation of the test function

void simple_error_test(Eo *obj);
EFL_VOID_FUNC_BODY_FALLBACK(simple_error_test, test_func(););
static void
_test(Eo *obj EFL_UNUSED, void *pd EFL_UNUSED)
{

}

//create a new class to call those things on

static Efl_Bool
_errorcase_class_initializer(Efl_Class *klass)
{
   EFL_OPS_DEFINE(ops,
         EFL_OBJECT_OP_FUNC(simple_error_test, _test),
   );

   return efl_class_functions_set(klass, &ops, NULL);
}

static const Efl_Class_Description errorcase_class_desc = {
     EO_VERSION,
     "Simple errorcase",
     EFL_CLASS_TYPE_REGULAR,
     0,
     _errorcase_class_initializer,
     NULL,
     NULL
};

EFL_DEFINE_CLASS(simple_errorcase_class_get, &errorcase_class_desc, EO_CLASS, NULL)

TEST(eo_fallbackcall_execute)
{

   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);

   fallback_called = EFL_FALSE;
   simple_error_test(NULL);
   efl_assert_int_eq(fallback_called, 1);

   fallback_called = EFL_FALSE;
   simple_error_test(obj);
   efl_assert_int_eq(fallback_called, 1);

}