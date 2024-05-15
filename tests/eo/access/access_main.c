#include "Efl_Eo.h"
#include "access_simple.h"
#include "access_inherit.h"

#include "efl-test-runner.h"

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
   efl_object_init();

   return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

TEST(eo_access)
{
   efl_object_init();

   Eo *obj = efl_add_ref(INHERIT_CLASS, NULL);

   simple_a_set(obj, 1);
   inherit_prot_print(obj);

   Simple_Public_Data *pd = efl_data_scope_get(obj, SIMPLE_CLASS);
   testlog("Pub: %d\n", pd->public_x2);

   efl_unref(obj);
   efl_object_shutdown();
}

