/*
 * Copyright (C) 2020 Expertise Solutions Cons em Inf
 */

#include <check.h>
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_UNISTD
# include <unistd.h>
#endif

#ifdef _WIN32
# include <evil_private.h> /* mkdir */
#endif

#include <Efl_Shared.h>
#include "efl-test-runner.h"

static void
thread_cleanup_fn(void *arg)
{
   *(int *) arg  = 1;
}

static void *
thread_fn_execute(void *arg, Eina_Thread t EINA_UNUSED)
{
   EINA_THREAD_CLEANUP_PUSH(thread_cleanup_fn, arg);
   EINA_THREAD_CLEANUP_POP(1);
   return NULL;
}

static void *
thread_fn_skip(void *arg, Eina_Thread t EINA_UNUSED)
{
   EINA_THREAD_CLEANUP_PUSH(thread_cleanup_fn, arg);
   EINA_THREAD_CLEANUP_POP(0);
   return NULL;
}

static void *
thread_fn_cancel(void *arg, Eina_Thread t EINA_UNUSED)
{
   Eina_Condition *cond = arg;

   ck_assert(eina_thread_cancellable_set(EINA_TRUE, NULL));
   ck_assert(eina_condition_signal(cond));

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
   int flag = 0;
   ck_assert(eina_thread_create(&t, EINA_THREAD_NORMAL, -1, thread_fn_execute, &flag));
   eina_thread_join(t);
   ck_assert_uint_eq(flag, 1);
}

TEST(thread_test_cleanup_skip)
{
   Eina_Thread t;
   int flag = 2;
   ck_assert(eina_thread_create(&t, EINA_THREAD_NORMAL, -1, thread_fn_skip, &flag));
   eina_thread_join(t);
   ck_assert_uint_eq(flag, 2);
}

TEST(thread_test_cancel)
{
   Eina_Thread t;
   Eina_Lock mutex;
   Eina_Condition cond;

   ck_assert(eina_lock_new(&mutex));
   ck_assert(eina_condition_new(&cond, &mutex));

   ck_assert(eina_thread_create(&t, EINA_THREAD_NORMAL, -1, thread_fn_cancel, &cond));
   ck_assert(eina_lock_take(&mutex));
   ck_assert(eina_condition_wait(&cond));
   ck_assert(eina_thread_cancel(t));
   ck_assert_ptr_eq(eina_thread_join(t), EINA_THREAD_JOIN_CANCELED);

   eina_condition_free(&cond);
   eina_lock_free(&mutex);
}

void
eina_test_thread(TCase *tc)
{
   tcase_add_test(tc, eina_thread_test_cleanup_skip);
   tcase_add_test(tc, eina_thread_test_cleanup_execute);
   tcase_add_test(tc, eina_thread_test_cancel);
}
