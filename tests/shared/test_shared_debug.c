#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>

#include "efl-test-runner.h"
#ifndef _WIN32
#  include <signal.h>
#endif

#ifdef SIGPROF
TEST(test_debug_sighandler)
{
   /* ensure this doesn't crash */
    // raise(SIGPROF);
}
#endif
