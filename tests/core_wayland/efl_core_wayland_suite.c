#include "efl_core_wayland_suite.h"

static const Efl_Test_Case etc[] =
{
   { "Session", test_session },
   { "Display", test_display },
   { "Window",  test_window },
   { "Input",   test_input },
   { "Output",  test_output },
   { NULL, NULL }
};

SUITE_INIT(core_wl)
{
   ck_assert_int_eq(ecore_wl_init(), 1);
}

SUITE_SHUTDOWN(core_wl)
{
   ck_assert_int_eq(ecore_wl_shutdown(), 0);
}

int
main(int argc, char **argv)
{
   int count;

   if (!_efl_test_option_disp(argc, argv, etc)) return 0;

#ifdef NEED_RUN_IN_TREE
   putenv("EFL_RUN_IN_TREE=1");
#endif

   count =
     _efl_suite_build_and_run(argc - 1,
                              (const char **)argv + 1, "Efl_Core_Wl", etc,
                              SUITE_INIT_FN(core_wl),
                              SUITE_SHUTDOWN_FN(core_wl));

   return (count == 0) ? 0 : 255;
}
