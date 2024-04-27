#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>

#include "efl-test-runner.h"

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
    eina_init();

    return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

TEST(cpu)
{
    assert(eina_cpu_count() > 0);
}
