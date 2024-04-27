/*
 * Copyright (C) 2020 Expertise Solutions Cons em Inf
 */

#include <check.h>
#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#ifdef HAVE_UNISTD
#  include <unistd.h>
#endif

#ifdef _WIN32
#  include <evil_private.h> /* mkdir */
#endif

#include <Efl_Shared.h>

#include "efl-test-runner.h"

static void
thread_cleanup_fn(void *arg)
{
    *(int *)arg = 1;
}

static void *
thread_fn_execute(void *arg, Eina_Thread t EFL_UNUSED)
{
    EINA_THREAD_CLEANUP_PUSH(thread_cleanup_fn, arg);
    EINA_THREAD_CLEANUP_POP(1);
    return NULL;
}

static void *
thread_fn_skip(void *arg, Eina_Thread t EFL_UNUSED)
{
    EINA_THREAD_CLEANUP_PUSH(thread_cleanup_fn, arg);
    EINA_THREAD_CLEANUP_POP(0);
    return NULL;
}

static void *
thread_fn_cancel(void *arg, Eina_Thread t EFL_UNUSED)
{
    Eina_Condition *cond = arg;

    efl_assert_true(eina_thread_cancellable_set(EFL_TRUE, NULL));
    efl_assert_true(eina_condition_signal(cond));

    for (size_t i = 0; i < 100; ++i)
    {
        eina_thread_cancel_checkpoint();
#ifdef _WIN32
        Sleep(100);
#else
        usleep(100 * 1000);
#endif
    }

    return NULL;
}

TEST(thread_test_cleanup_execute)
{
    Eina_Thread t;
    int         flag = 0;
    efl_assert_true(eina_thread_create(&t,
                                       EINA_THREAD_NORMAL,
                                       -1,
                                       thread_fn_execute,
                                       &flag));
    eina_thread_join(t);
    efl_assert_uint_eq(flag, 1);
}

TEST(thread_test_cleanup_skip)
{
    Eina_Thread t;
    int         flag = 2;
    efl_assert_true(
        eina_thread_create(&t, EINA_THREAD_NORMAL, -1, thread_fn_skip, &flag));
    eina_thread_join(t);
    efl_assert_uint_eq(flag, 2);
}

TEST(thread_test_cancel)
{
    Eina_Thread    t;
    Eina_Lock      mutex;
    Eina_Condition cond;

    efl_assert_true(eina_lock_new(&mutex));
    efl_assert_true(eina_condition_new(&cond, &mutex));

    efl_assert_true(eina_thread_create(&t,
                                       EINA_THREAD_NORMAL,
                                       -1,
                                       thread_fn_cancel,
                                       &cond));
    efl_assert_true(eina_lock_take(&mutex));
    efl_assert_true(eina_condition_wait(&cond));
    efl_assert_true(eina_thread_cancel(t));
    efl_assert_ptr_eq(eina_thread_join(t), EINA_THREAD_JOIN_CANCELED);

    eina_condition_free(&cond);
    eina_lock_free(&mutex);
}
