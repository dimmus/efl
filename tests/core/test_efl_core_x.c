#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>

#include "efl_core_suite.h"

/* FIXME: Currently disable these tests. They are useless ATM and they just
 * make buildbot complain. Once we add useful tests here we'll also bother
 * with getting X on the server. */
#undef HAVE_CORE_X_XLIB

#ifdef HAVE_CORE_X_XLIB

#  include <Core_X.h>

EFL_START_TEST(core_test_core_x_init)
{
    int ret;

    ret = core_x_init(NULL);
    fail_if(ret != 1);

    ret = core_x_shutdown();
    fail_if(ret != 0);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_x_bell)
{
    int i;
    int ret;

    ret = core_x_init(NULL);
    fail_if(ret != 1);

    printf("You should hear 3 beeps now.\n");
    for (i = 0; i < 3; i++)
    {
        ret = core_x_bell(0);
        fail_if(ret != EFL_TRUE);
        core_x_sync();
        sleep(1);
    }

    core_x_shutdown();
}

EFL_END_TEST

#endif

void
core_test_core_x(TCase *tc EFL_UNUSED)
{
#ifdef HAVE_CORE_X_XLIB
    tcase_add_test(tc, core_test_core_x_init);
    tcase_add_test(tc, core_test_core_x_bell);
#endif
}
