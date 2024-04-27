/*
 * Copyright (C) 2013 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <time.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

#ifndef HAVE_CLOCK_GETTIME

#  ifdef __MACH__
#    include <mach/clock.h>
#    include <mach/mach.h>

#    define CLOCK_REALTIME 0

int
clock_gettime(int mode, struct timespec *ts)
{
    clock_serv_t    cclock;
    mach_timespec_t mts;
    kern_return_t   err;

    err = host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    if (EINA_UNLIKELY(err != KERN_SUCCESS)) return err;

    err = clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    if (EINA_UNLIKELY(err != KERN_SUCCESS)) return err;

    ts->tv_sec  = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;

    return 0;
}

#  elif defined(_WIN32)

#    define CLOCK_REALTIME 0

int
clock_gettime(int mode, struct timespec *ts)
{
    FILETIME       sys_time;
    ULARGE_INTEGER li_sys_time;

    GetSystemTimeAsFileTime(&sys_time);
    li_sys_time.u.LowPart  = sys_time.dwLowDateTime;
    li_sys_time.u.HighPart = sys_time.dwHighDateTime;
    ts->tv_sec             = li_sys_time.QuadPart / 10000000UL;
    ts->tv_nsec            = (li_sys_time.QuadPart % 10000000UL) * 100UL;
    return 0;
}

#  else /* ! __MACH__ */
#    error No support for clock_gettime()
#  endif /* __MACH__ */

#endif /* ! HAVE_CLOCK_GETTIME */

static Eina_Spinlock spin;
static Eina_Thread   thread;
static unsigned int  counter;

static void *
_eina_test_lock_thread(void *data, Eina_Thread t)
{
    unsigned int i;

    efl_assert_fail_if(!eina_thread_equal(t, thread));
    efl_assert_fail_if(strcmp("test", data));

    for (i = 0; i < 150; i++)
    {
        efl_assert_fail_if(eina_spinlock_take(&spin) != EINA_LOCK_SUCCEED);
        counter++;
        efl_assert_fail_if(eina_spinlock_release(&spin) != EINA_LOCK_SUCCEED);
    }

    return data;
}

TEST(test_spinlock)
{
    unsigned int i;

    counter = 0;
    efl_assert_fail_if(!eina_spinlock_new(&spin));

    efl_assert_fail_if(!eina_thread_create(&thread,
                                EINA_THREAD_NORMAL,
                                -1,
                                _eina_test_lock_thread,
                                "test"));

    for (i = 0; i < 150; i++)
    {
        efl_assert_fail_if(eina_spinlock_take(&spin) != EINA_LOCK_SUCCEED);
        counter++;
        efl_assert_fail_if(eina_spinlock_release(&spin) != EINA_LOCK_SUCCEED);
    }

    efl_assert_fail_if(strcmp("test", eina_thread_join(thread)));

    efl_assert_fail_if(counter != 300);

    efl_assert_fail_if(eina_spinlock_take_try(&spin) != EINA_LOCK_SUCCEED);
    efl_assert_fail_if(eina_spinlock_release(&spin) != EINA_LOCK_SUCCEED);

    eina_spinlock_free(&spin);
}

static Eina_TLS key;
static int      _eina_tls_free_count = 0;

static void *
_eina_test_tls_alloc(int v)
{
    int *ptr = malloc(sizeof(int));
    *ptr     = v;

    return ptr;
}

static void
_eina_test_tls_free(void *ptr)
{
    _eina_tls_free_count++;
    free(ptr);
}

static void *
_eina_test_tls_thread(void *data EFL_UNUSED, Eina_Thread t EFL_UNUSED)
{
    int *ptr;

    ptr = eina_tls_get(key);
    efl_assert_fail_if(ptr != NULL);

    efl_assert_fail_if(!eina_tls_set(key, _eina_test_tls_alloc(24)));

    ptr = eina_tls_get(key);
    efl_assert_fail_if(ptr == NULL);
    efl_assert_fail_if(*ptr != 24);

    return NULL;
}

TEST(test_tls)
{
    efl_assert_fail_if(!eina_tls_cb_new(&key, _eina_test_tls_free));

    efl_assert_fail_if(!eina_tls_set(key, _eina_test_tls_alloc(42)));

    efl_assert_fail_if(!eina_thread_create(&thread,
                                EINA_THREAD_NORMAL,
                                -1,
                                _eina_test_tls_thread,
                                NULL));

    eina_thread_join(thread);
    efl_assert_fail_if(_eina_tls_free_count != 1);

    int *ptr = eina_tls_get(key);
    efl_assert_fail_if(eina_tls_get(key) == NULL);
    efl_assert_fail_if(*ptr != 42);

    eina_tls_free(key);
}

static Eina_Barrier   barrier;
static Eina_Lock      mtcond;
static Eina_Condition condition;
static Eina_RWLock    mutex;

static void *
_eina_test_rwlock_thread(void *data EFL_UNUSED, Eina_Thread t EFL_UNUSED)
{
    efl_assert_fail_if(!eina_barrier_wait(&barrier));
    efl_assert_fail_if(eina_lock_take(&mtcond) != EINA_LOCK_SUCCEED);
    efl_assert_fail_if(!eina_condition_broadcast(&condition));
    efl_assert_fail_if(eina_lock_release(&mtcond) != EINA_LOCK_SUCCEED);

    efl_assert_fail_if(eina_rwlock_take_write(&mutex) != EINA_LOCK_SUCCEED);
    counter = 7200;
    efl_assert_fail_if(eina_rwlock_release(&mutex) != EINA_LOCK_SUCCEED);

    efl_assert_fail_if(!eina_barrier_wait(&barrier));

    efl_assert_fail_if(eina_lock_take(&mtcond) != EINA_LOCK_SUCCEED);
    efl_assert_fail_if(!eina_condition_broadcast(&condition));
    efl_assert_fail_if(eina_lock_release(&mtcond) != EINA_LOCK_SUCCEED);

    return NULL;
}

TEST(test_rwlock)
{
    struct timespec ts, ts2;
    long            delay;

    efl_assert_fail_if(!eina_rwlock_new(&mutex));
    efl_assert_fail_if(!eina_lock_new(&mtcond));
    efl_assert_fail_if(!eina_condition_new(&condition, &mtcond));
    efl_assert_fail_if(!eina_barrier_new(&barrier, 2));

    counter = 42;

    eina_lock_debug(&mtcond);

    efl_assert_fail_if(eina_rwlock_take_read(&mutex) != EINA_LOCK_SUCCEED);
    efl_assert_fail_if(eina_lock_take(&mtcond) != EINA_LOCK_SUCCEED);

    efl_assert_fail_if(!eina_thread_create(&thread,
                                EINA_THREAD_NORMAL,
                                -1,
                                _eina_test_rwlock_thread,
                                NULL));

    efl_assert_fail_if(!eina_barrier_wait(&barrier));
    efl_assert_fail_if(!eina_condition_wait(&condition));
    efl_assert_fail_if(eina_lock_release(&mtcond) != EINA_LOCK_SUCCEED);

    efl_assert_fail_if(counter != 42);
    efl_assert_fail_if(eina_rwlock_release(&mutex) != EINA_LOCK_SUCCEED);

    efl_assert_fail_if(eina_lock_take(&mtcond) != EINA_LOCK_SUCCEED);
    efl_assert_fail_if(!eina_barrier_wait(&barrier));

    efl_assert_fail_if(!eina_condition_wait(&condition));
    efl_assert_fail_if(eina_lock_release(&mtcond) != EINA_LOCK_SUCCEED);

    efl_assert_fail_if(eina_rwlock_take_read(&mutex) != EINA_LOCK_SUCCEED);
    efl_assert_fail_if(counter != 7200);
    efl_assert_fail_if(eina_rwlock_release(&mutex) != EINA_LOCK_SUCCEED);

    efl_assert_fail_if(eina_lock_take(&mtcond) != EINA_LOCK_SUCCEED);
    clock_gettime(CLOCK_REALTIME, &ts);
    eina_condition_timedwait(&condition, 0.050);
    clock_gettime(CLOCK_REALTIME, &ts2);
    delay = (ts2.tv_sec - ts.tv_sec) * 1000L +
            (ts2.tv_nsec - ts.tv_nsec) / 1000000L;
    efl_assert_fail_if(delay < 50);
    efl_assert_fail_if(delay > 200);
    efl_assert_fail_if(eina_error_get() != ETIMEDOUT);
    efl_assert_fail_if(eina_lock_release(&mtcond) != EINA_LOCK_SUCCEED);

    eina_thread_join(thread);

    eina_condition_free(&condition);
    eina_lock_free(&mtcond);
    eina_rwlock_free(&mutex);
}
