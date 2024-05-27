#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>
#include "efl_core_suite.h"

static void
_core_quit(void *data)
{
    Efl_Bool *bob = data;

    *bob = EFL_TRUE;
    core_main_loop_quit();
}

EFL_START_TEST(core_test_job)
{
    Efl_Bool  bob = EFL_FALSE;
    Core_Job *job;

    job = core_job_add(&_core_quit, &bob);
    fail_if(!job);

    core_main_loop_begin();

    fail_if(bob != EFL_TRUE);
}

EFL_END_TEST

#ifndef _WIN32
#  define NUM_SIGNALS 2000

static Efl_Bool
_signal_cb(void *data, int t EFL_UNUSED, void *ev EFL_UNUSED)
{
    int *called = data;
    (*called)++;
    if (*called == NUM_SIGNALS) core_main_loop_quit();
    return CORE_CALLBACK_RENEW;
}

static void
_core_signal_job(void *data EFL_UNUSED)
{
    for (unsigned int i = 0; i < NUM_SIGNALS; i++)
        raise(SIGUSR2);
}

EFL_START_TEST(core_test_job_signal)
{
    int called = 0;
    core_job_add(_core_signal_job, NULL);
    core_event_handler_add(CORE_EVENT_SIGNAL_USER, _signal_cb, &called);

    core_main_loop_begin();
    ck_assert_int_eq(called, NUM_SIGNALS);
}

EFL_END_TEST
#endif

void
core_test_core_job(TCase *tc)
{
    tcase_add_test(tc, core_test_job);
#ifndef _WIN32
    tcase_add_test(tc, core_test_job_signal);
#endif
}
