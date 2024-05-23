#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#ifdef _WIN32
# include <evil_private.h> /* setenv */
#endif

#include "efl_eo_gen_suite.h"
#include "../efl_check.h"
#include <Efl_Eo_Gen.h>

static const Efl_Test_Case etc[] = {
  { "Eolian Parsing", eolian_parsing_test},
  { "Eolian Static Analysis", eolian_static_test},
  { "Eolian Generation", eolian_generation_test},
  { "Eolian Aux", eolian_aux_test},
  { NULL, NULL }
};

SUITE_INIT(efl_eo_gen)
{
   ck_assert_int_eq(eolian_init(), 1);
}

SUITE_SHUTDOWN(efl_eo_gen)
{
   ck_assert_int_eq(eolian_shutdown(), 0);
}

int
main(int argc, char **argv)
{
   int failed_count;

   setenv("CK_FORK", "no", 0);

   if (!_efl_test_option_disp(argc, argv, etc))
     return 0;

#ifdef NEED_RUN_IN_TREE
   putenv("EFL_RUN_IN_TREE=1");
#endif

   failed_count = _efl_suite_build_and_run(argc - 1, (const char **)argv + 1,
                                           "Efl_Eo_Gen", etc, SUITE_INIT_FN(efl_eo_gen), SUITE_SHUTDOWN_FN(efl_eo_gen));

   return (failed_count == 0) ? 0 : 255;
}
