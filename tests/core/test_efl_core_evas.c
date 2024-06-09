#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Core_Evas.h>
#include <Efl_Core.h>

#include "efl_core_suite.h"

#define WINDOW_HEIGHT 200
#define WINDOW_WIDTH  200

EFL_START_TEST(core_test_core_evas_associate)
{
  Core_Evas   *ee;
  Evas        *canvas;
  Evas_Object *bg;
  int          ret;

  ret = core_evas_init();
  fail_if(ret == 0);

  ee = core_evas_buffer_new(WINDOW_WIDTH, WINDOW_HEIGHT);
  fail_if(ee == NULL);

  canvas = core_evas_get(ee);
  fail_if(core_evas_core_evas_get(canvas) != ee);

  bg  = evas_object_rectangle_add(canvas);
  ret = core_evas_object_associate(ee, bg, CORE_EVAS_OBJECT_ASSOCIATE_BASE);
  fail_if(ret == 0);

  core_evas_free(ee);

  ret = core_evas_shutdown();
  fail_if(ret != 0);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_evas_cocoa)
{
  int                ret;
  Core_Evas         *ee;
  Core_Cocoa_Window *win;

  ret = core_evas_init();
  fail_if(ret != 1);

  ret = core_evas_engine_type_supported_get(CORE_EVAS_ENGINE_OPENGL_COCOA);
  if (ret == EFL_TRUE)
  {
        /* Engine supported. Shall not fail... */
    ee = core_evas_cocoa_new(NULL, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    fail_if(ee == NULL);
    win = core_evas_cocoa_window_get(ee);
    fail_if(win == NULL);
  }
  else
  {
        /* Engine not supported. Shall not succeed... */
    ee = core_evas_cocoa_new(NULL, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    fail_if(ee != NULL);
    win = core_evas_cocoa_window_get(ee);
    fail_if(win != NULL);
  }

  core_evas_free(ee);

  ret = core_evas_shutdown();
  fail_if(ret != 0);
}

EFL_END_TEST

static Eina_Value
_verify_and_exit(void                          *data,
                 const Eina_Value               value,
                 const Eina_Future *dead_future EFL_UNUSED)
{
  ck_assert_ptr_eq(eina_value_type_get(&value), EINA_VALUE_TYPE_CONTENT);
  Eina_Content *content   = eina_value_to_content(&value);
  Eina_Content *reference = data;

  ck_assert_int_eq(eina_content_data_get(content).len,
                   eina_content_data_get(reference).len);
  ck_assert_str_eq(eina_content_data_get(content).mem,
                   eina_content_data_get(reference).mem);
  ck_assert_str_eq(eina_content_type_get(content),
                   eina_content_type_get(reference));

  efl_loop_quit(efl_main_loop_get(), eina_value_int_init(0));

  return EINA_VALUE_EMPTY;
}

EFL_START_TEST(core_test_core_evas_fallback_selection)
{
  Core_Evas *ee;
  core_evas_init();

  ee = core_evas_buffer_new(WINDOW_WIDTH, WINDOW_HEIGHT);
  fail_if(ee == NULL);

  for (int i = 0; i < CORE_EVAS_SELECTION_BUFFER_LAST; ++i)
  {
    ck_assert_int_eq(core_evas_selection_exists(ee, 0, i), EFL_FALSE);
  }
  Eina_Content *content =
    eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL("asdf"), "text/plain");
  Eina_Content *ref =
    eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL("asdf"), "text/plain");
  core_evas_selection_set(ee,
                          0,
                          CORE_EVAS_SELECTION_BUFFER_SELECTION_BUFFER,
                          content);

  const char *types[] = { eina_stringshare_add("text/plain") };

  Eina_Future *f =
    core_evas_selection_get(ee,
                            0,
                            CORE_EVAS_SELECTION_BUFFER_SELECTION_BUFFER,
                            EINA_C_ARRAY_ITERATOR_NEW(types));
  ck_assert_ptr_ne(f, NULL);
  eina_future_then(f, _verify_and_exit, ref);
  efl_task_run(efl_main_loop_get());
  core_evas_shutdown();
}

EFL_END_TEST

void
core_test_core_evas(TCase *tc)
{
  tcase_add_test(tc, core_test_core_evas_associate);
  tcase_add_test(tc, core_test_core_evas_cocoa);
  tcase_add_test(tc, core_test_core_evas_fallback_selection);
}
