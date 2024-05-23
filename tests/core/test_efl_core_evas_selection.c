#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <Core_Evas.h>
#include <Efl_Core.h>

#include "efl_core_suite.h"

#define WINDOW_HEIGHT 200
#define WINDOW_WIDTH 200

static int log_abort;
static int log_abort_level;

void
fail_on_errors_teardown(void)
{
   eina_log_abort_on_critical_set(log_abort);
   eina_log_abort_on_critical_level_set(log_abort_level);
}

void
fail_on_errors_setup(void)
{
   log_abort = eina_log_abort_on_critical_get();
   log_abort_level = eina_log_abort_on_critical_level_get();
   eina_log_abort_on_critical_level_set(2);
   eina_log_abort_on_critical_set(1);
}

static Core_Evas *ee;

void
_setup(void)
{
   core_evas_init();
   ee = core_evas_buffer_new(50, 50);
}

void
_teardown(void)
{
   core_evas_free(ee);
   core_evas_shutdown();
}

EFL_START_TEST(core_test_selection_get_twice)
{
   //this is just running this and checking that we do not error
   for (int i = 0; i < CORE_EVAS_SELECTION_BUFFER_LAST; ++i)
     {
        if (i == CORE_EVAS_SELECTION_BUFFER_DRAG_AND_DROP_BUFFER) continue;
        const char *types[] = {eina_stringshare_add("text/plain")};
        ck_assert_ptr_ne(core_evas_selection_get(ee, 0, i, EINA_C_ARRAY_ITERATOR_NEW(types)), NULL);
     }
   for (int i = 0; i < CORE_EVAS_SELECTION_BUFFER_LAST; ++i)
     {
        if (i == CORE_EVAS_SELECTION_BUFFER_DRAG_AND_DROP_BUFFER) continue;
        const char *types[] = {eina_stringshare_add("text/plain")};
        ck_assert_ptr_ne(core_evas_selection_get(ee, 0, i, EINA_C_ARRAY_ITERATOR_NEW(types)), NULL);
     }
}
EFL_END_TEST

EFL_START_TEST(core_test_selection_claim_twice)
{
   //this is just running this and checking that we do not error
   for (int i = 0; i < CORE_EVAS_SELECTION_BUFFER_LAST; ++i)
     {
        if (i == CORE_EVAS_SELECTION_BUFFER_DRAG_AND_DROP_BUFFER) continue;
        Eina_Content *content = eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL("asdf"), "text/plain");
        ck_assert_int_eq(core_evas_selection_set(ee, 0, i, content), EFL_TRUE);
     }
   for (int i = 0; i < CORE_EVAS_SELECTION_BUFFER_LAST; ++i)
     {
        if (i == CORE_EVAS_SELECTION_BUFFER_DRAG_AND_DROP_BUFFER) continue;
        Eina_Content *content = eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL("asdf"), "text/plain");
        ck_assert_int_eq(core_evas_selection_set(ee, 0, i, content), EFL_TRUE);
     }
}
EFL_END_TEST
void core_test_core_evas_selection(TCase *tc)
{
   tcase_add_checked_fixture(tc, fail_on_errors_setup, fail_on_errors_teardown);
   tcase_add_checked_fixture(tc, _setup, _teardown);
   tcase_add_test(tc, core_test_selection_get_twice);
   tcase_add_test(tc, core_test_selection_claim_twice);
}
