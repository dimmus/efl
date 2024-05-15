#include <Efl_Eo.h>

#include "eo_suite.h"
#include "eo_test_class_simple.h"

TEST(eo_test_simple)
{
   assert(efl_object_init()); /* one init by test suite */
   assert(efl_object_shutdown());
}

#if 0
TEST(eo_test_init_shutdown)
{
   Eo *obj;

   efl_assert_str_eq("Efl.Object", efl_class_name_get(EFL_OBJECT_CLASS));

   /* XXX-1: Essential for the next test to assign the wrong op. */
   obj = efl_add_ref(SIMPLE_CLASS, NULL);
   simple_a_set(obj, 1);
   /* XXX-1: Essential for the next test to cache the op. */
   efl_assert_int_eq(1, simple_a_get(obj));

   efl_unref(obj);
   efl_assert_fail_if(efl_object_shutdown());

   assert(efl_object_init());
   efl_assert_str_eq("Efl.Object", efl_class_name_get(EFL_OBJECT_CLASS));

   /* XXX-1: Verify that the op was not cached. */
   obj = efl_add_ref(SIMPLE_CLASS, NULL);
   simple_a_set(obj, 1);
   efl_assert_int_eq(1, simple_a_get(obj));
}
#endif