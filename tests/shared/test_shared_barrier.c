#include <Efl_Shared.h>

#include "efl-test-runner.h"

static Eina_Thread  wk1, wk2, wk3, wk4, wk5;
static Eina_Barrier barrier;

static void *
wk_func(void *data EFL_UNUSED, Eina_Thread thread EFL_UNUSED)
{
    eina_barrier_wait(&barrier);
    return NULL;
}

static void *
wk1_func(void *data EFL_UNUSED, Eina_Thread thread EFL_UNUSED)
{
    eina_barrier_wait(&barrier);
    return NULL;
}

static void *
wk2_func(void *data EFL_UNUSED, Eina_Thread thread EFL_UNUSED)
{
    eina_barrier_wait(&barrier);
    return NULL;
}

static void *
wk3_func(void *data EFL_UNUSED, Eina_Thread thread EFL_UNUSED)
{
    eina_barrier_wait(&barrier);
    return NULL;
}

TEST(barrier_test_simple)
{
    Efl_Bool r;
    int      i;

    i = eina_threads_init();
    assert(i >= 1);

    r = eina_barrier_new(&barrier, 6);
    assert(r);

    r = eina_thread_create(&wk1, EINA_THREAD_NORMAL, -1, wk_func, NULL);
    assert(r);

    r = eina_thread_create(&wk2, EINA_THREAD_NORMAL, -1, wk_func, NULL);
    assert(r);

    r = eina_thread_create(&wk3, EINA_THREAD_NORMAL, -1, wk1_func, NULL);
    assert(r);

    r = eina_thread_create(&wk4, EINA_THREAD_NORMAL, -1, wk2_func, NULL);
    assert(r);

    r = eina_thread_create(&wk5, EINA_THREAD_NORMAL, -1, wk3_func, NULL);
    assert(r);

    eina_barrier_wait(&barrier);

    eina_thread_join(wk1);

    eina_thread_join(wk2);

    eina_thread_join(wk3);

    eina_thread_join(wk4);

    eina_thread_join(wk5);

    eina_barrier_free(&barrier);

    eina_threads_shutdown();
}
