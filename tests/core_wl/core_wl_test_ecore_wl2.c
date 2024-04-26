#include "core_wl_suite.h"

EFL_START_TEST(ecore_wl_simple)
{}
EFL_END_TEST

EFL_START_TEST(wl2_session_recovery_disable)
{
   //FIXME: Need some discussion about how to validate this API in TC.
    ecore_wl_session_recovery_disable();
}
EFL_END_TEST

void
ecore_wl_test_init(TCase *tc)
{
    tcase_add_test(tc, ecore_wl_simple);
    if (getenv("WAYLAND_DISPLAY") && (getenv("E_START")))
    {
        tcase_add_test(tc, wl2_session_recovery_disable);
    }
}
