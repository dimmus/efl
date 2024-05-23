#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <Efl_Core.h>

#include "../efl_check.h"

#include "efl_core_suite.h"

static const Efl_Test_Case etc[] = {
  { "Ecore", core_test_ecore },
  { "idle", core_test_core_idle },
  { "poller", core_test_core_idle },
// #if HAVE_CORE_X
//   { "Core_X", core_test_core_x },
// #endif
  // { "Core_Imf", core_test_core_imf },
// #if HAVE_CORE_AUDIO
//   { "Ecore Audio", core_test_core_audio},
// #endif
  { "Core_Timers", core_test_timer },
  // { "Core_Evas", core_test_core_evas },
  { "Core_Animators", core_test_animator },
  { "Eina_Thread_Queue", core_test_core_thread_eina_thread_queue },
  { "Eina_Thread_Queue", core_test_core_thread_eina_thread_queue2 },
// #if HAVE_CORE_FB
//   { "Core_Fb", core_test_core_fb },
// #endif
  // { "Core_Input", core_test_core_input },
  // { "Core_File", core_test_core_file },
  { "Core_Job", core_test_core_job },
  { "Core_Args", core_test_core_args },
  { "Core_Pipe", core_test_core_pipe },
  // { "Core_Evas_Selection", core_test_core_evas_selection },
  { NULL, NULL }
};

static Core_Timer *timeout;
int timeout_reached = 0;

static Efl_Bool
timeout_cb(void *)
{
#if CHECK_MINOR_VERSION >= 11
   const char *tcname = tcase_name();

   timeout_reached = 1;

   if (tcname && strstr(tcname, "download"))
     {
        fprintf(stderr, "test timeout reached: download failed, probably network issue. skipping\n");
        core_main_loop_quit();
     }
   else
#endif
     ck_abort_msg("test timeout reached!");
   timeout = NULL;
   return EFL_FALSE;
}

SUITE_INIT(efl_core)
{
   timeout_reached = 0;
   ck_assert_int_eq(core_init(), 1);
#ifdef HAVE_FORK
   if (eina_streq(getenv("CK_FORK"), "no")) return;
#endif
   timeout = core_timer_add(5.0, timeout_cb, NULL);
   ck_assert_msg(!!timeout, "timeout timer creation failed!");
}

SUITE_SHUTDOWN(efl_core)
{
   core_timer_del(timeout);
   timeout = NULL;
   ck_assert_int_eq(core_shutdown(), 0);
}

int
main(int argc, char **argv)
{
   int failed_count;

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

#ifdef NEED_RUN_IN_TREE
   putenv("EFL_RUN_IN_TREE=1");
#endif

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Efl_Core", etc, SUITE_INIT_FN(efl_core), SUITE_SHUTDOWN_FN(efl_core));

   return (failed_count == 0) ? 0 : 255;
}
