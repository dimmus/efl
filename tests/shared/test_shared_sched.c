/*
 * Copyright (C) 2008 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef __linux__
#include <errno.h>
#include <sys/resource.h>
#endif

#include <Efl_Shared.h>

#include "efl-test-runner.h"

#ifdef __linux__

/*
 * TODO: Test if RT priorities are right. However, make check should be run as
 * root.
 */

static void *
_thread_run(void *arg EINA_UNUSED, Eina_Thread tid EINA_UNUSED)
{
    int niceval = getpriority(PRIO_PROCESS, 0);
    int niceval2;
    eina_sched_prio_drop();

    niceval2 = getpriority(PRIO_PROCESS, 0);
    if (niceval + 5 >= 19)
      ck_assert_int_eq(niceval2, 19);
    else
      ck_assert_int_eq(niceval2, (niceval + 5));

    return NULL;
}

TEST(test_sched_prio_drop)
{
    int niceval = getpriority(PRIO_PROCESS, 0);
    int niceval2;
    Eina_Thread tid;
    Eina_Bool r;


    r = eina_thread_create(&tid, EINA_THREAD_NORMAL, -1, _thread_run, NULL);
    fail_unless(r);

    niceval2 = getpriority(PRIO_PROCESS, 0);
    /* niceness of main thread should not have changed */
    ck_assert_int_eq(niceval2, niceval);

    eina_thread_join(tid);
    /* niceness of main thread should not have changed */
    ck_assert_int_eq(niceval2, niceval);

}
#else
TEST(test_sched_prio_drop)
{
   fprintf(stderr, "scheduler priority is not supported by your configuration.\n");
}
#endif

void
eina_test_sched(TCase *tc)
{
   tcase_add_test(tc, eina_test_sched_prio_drop);
}
