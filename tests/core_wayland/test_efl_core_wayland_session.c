#include "efl_core_wayland_suite.h"

EFL_START_TEST(simple)
{}

EFL_END_TEST

EFL_START_TEST(core_wayland_session_recovery_disable)
{
   //FIXME: Need some discussion about how to validate this API in TC.
    ecore_wl_session_recovery_disable();
}

EFL_END_TEST

void
test_session(TCase *tc)
{
    tcase_add_test(tc, simple);
    if (getenv("WAYLAND_DISPLAY") && (getenv("E_START")))
    {
        tcase_add_test(tc, core_wayland_session_recovery_disable);
    }
}
