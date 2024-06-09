#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>

#include "efl_shared_suite.h"
#ifndef _WIN32
#  include <signal.h>
#endif
#ifdef SIGPROF
EFL_START_TEST(efl_shared_test_debug_sighandler)
{
   /* ensure this doesn't crash */
  raise(SIGPROF);
}

EFL_END_TEST
#endif

void
eina_test_debug(TCase *tc)
{
#ifdef SIGPROF
  if (!eina_streq(getenv("CK_FORK"), "no"))
    tcase_add_test_raise_signal(tc, efl_shared_test_debug_sighandler, SIGPROF);
#endif
}
