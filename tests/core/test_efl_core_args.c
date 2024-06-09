#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>
#include "efl_core_suite.h"
#include "core_private.h"

static const char *args[] = { "a", "b", "c", "d", "e", "f", "g", "h" };

static void
_cb_args1(void *data EFL_UNUSED, const Efl_Event *event)
{
  Efl_Loop_Arguments *arge = event->info;
  unsigned int        i;

  fail_if(eina_array_count(arge->argv) != (sizeof(args) / sizeof(args[0])));
  for (i = 0; i < eina_array_count(arge->argv); i++)
  {
    fail_if(!!strcmp(eina_array_data_get(arge->argv, i), args[i]));
  }
  core_main_loop_quit();
}

EFL_START_TEST(core_test_args1)
{
  efl_event_callback_add(efl_main_loop_get(),
                         EFL_LOOP_EVENT_ARGUMENTS,
                         _cb_args1,
                         NULL);
  core_loop_arguments_send(8, args);
  core_main_loop_begin();
}

EFL_END_TEST

static void
_cb_args2(void *data EFL_UNUSED, const Efl_Event *event)
{
  Efl_Loop_Arguments *arge = event->info;

  fail_if(eina_array_count(arge->argv) != 1);
  fail_if(!!strcmp(eina_array_data_get(arge->argv, 0), "hello world"));
  core_main_loop_quit();
}

EFL_START_TEST(core_test_args2)
{
  const char *simple_args[] = { "hello world" };

  efl_event_callback_add(efl_main_loop_get(),
                         EFL_LOOP_EVENT_ARGUMENTS,
                         _cb_args2,
                         NULL);
  core_loop_arguments_send(1, simple_args);
  core_main_loop_begin();
}

EFL_END_TEST

static void
_cb_args3(void *data EFL_UNUSED, const Efl_Event *event)
{
  Efl_Loop_Arguments *arge = event->info;

  fail_if(eina_array_count(arge->argv) != 0);
  core_main_loop_quit();
}

EFL_START_TEST(core_test_args3)
{
  efl_event_callback_add(efl_main_loop_get(),
                         EFL_LOOP_EVENT_ARGUMENTS,
                         _cb_args3,
                         NULL);
  core_loop_arguments_send(0, NULL);
  core_main_loop_begin();
}

EFL_END_TEST

void
core_test_core_args(TCase *tc)
{
  tcase_add_test(tc, core_test_args1);
  tcase_add_test(tc, core_test_args2);
  tcase_add_test(tc, core_test_args3);
}
