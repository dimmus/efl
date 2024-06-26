#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>

#include <math.h>

#include "efl_core_suite.h"

#define TIMEOUT_1 0.01 // interval for timer1
#define TIMEOUT_2 0.02 // timer2
#define TIMEOUT_3 0.06 // timer3
#define TIMEOUT_4 0.1  // timer4
#define TIMEOUT_5 0.5  // timer5 - end core_main_loop_begin()
#define SIZE      3

static int freeze_thaw = 0;   // 1 - freeze timer, 0 - thaw

struct _timers           // timer struct
{
  Core_Timer *timer1; // timer 1
  Core_Timer *timer2;
  Core_Timer *timer3;
  Core_Timer *timer4;
  Core_Timer *timer5;
  int         count_timer1;
  int         count_timer3;    // check count timer 3
  int         add_timer2;      // flag add timer 2
  int         check_freeze_thaw_timer3;
  int         num_elem;
  double      delay_1[3];
  double      interval_1[3];
  double      precision[3];
};

static Efl_Bool
_timer1_cb(void *data)
{
  struct _timers *timer = data;
  timer->count_timer1++;

  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_timer2_cb(void *data)
{
  struct _timers *timer = (struct _timers *)data;

   // choose next settings (delay, interval, precision) for timer 1
  if (++timer->num_elem > 2) timer->num_elem = 0;

   // check add/delay timer 2
  ck_assert_msg(timer->add_timer2 == 1, "Error add/delay timer");

   // check set new delay for timer 1
  core_timer_delay(timer->timer1, timer->delay_1[timer->num_elem]);

   // check set new interval for timer 1
  core_timer_interval_set(timer->timer1, timer->interval_1[timer->num_elem]);
  ck_assert_msg(EINA_DBL_EQ(timer->interval_1[timer->num_elem],
                            core_timer_interval_get(timer->timer1)),
                "Error set new interval");

   // check set new precision
  core_timer_precision_set(timer->precision[timer->num_elem]);
  ck_assert_msg(
    EINA_DBL_EQ(timer->precision[timer->num_elem], core_timer_precision_get()),
    "Error set new precision");

   // check removal timer 2
  if (core_timer_del(timer->timer2))
  {
    timer->add_timer2 = 0;
    timer->timer2     = NULL;
  }
  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_timer3_cb(void *data)
{
  struct _timers *timer = (struct _timers *)data;

  timer->count_timer3++; // number of starts timer 3

   // check add timer 2
  if (!timer->timer2)
  {
    timer->add_timer2++;   // amount of added timer
    timer->timer2 = core_timer_add(TIMEOUT_2, _timer2_cb, timer);
  }

  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_timer4_cb(void *data)
{
  struct _timers *timer = (struct _timers *)data;

   // check frezze/thaw timer 3
  if (freeze_thaw)
  {
    ck_assert_msg(timer->check_freeze_thaw_timer3 == timer->count_timer3,
                  "Error frezze/thaw timer");

    core_timer_thaw(timer->timer3);
    freeze_thaw = 0;
  }
  else
  {
    core_timer_freeze(timer->timer3);
    freeze_thaw                     = 1;
    timer->check_freeze_thaw_timer3 = timer->count_timer3;
  }

  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_timer5_cb(void *data)
{
  struct _timers *timer = (struct _timers *)data;
  if (timer->timer2) timer->timer2 = NULL;
  core_main_loop_quit();
  return CORE_CALLBACK_RENEW;
}

/*
  Timer2 sets new parameters (delay, interval and precision) for Timer1 and checks them.
  Timer3 creates and runs Timer2 and increases parameter add_timer2 to 1. Timer2 removes himself
  and sets parameter add_timer2 to 0. Inside callback of Timer2 parameter add_timer2 should be
  equal to 1 otherwise "Error add/delay timer". 
  Timer4 controls 'freezing' or 'thaw' for Timer3. Timer4 sets flag freeze_thaw to state: 'freeze' or 'thaw'.
  When Timer3 is frozen counter of Timer3 saved in the parameter check_freeze_thaw_timer3.
  Otherwise (thaw mode) check that parameter check_freeze_thaw_timer3 equals to counter Timer3.
  If not equal then "Error frezze/thaw timer".
  Timer 5 finishes testing.
*/

EFL_START_TEST(core_test_timers)
{
  struct _timers timer = {
    .num_elem   = 0,
    .delay_1    = { 0.01, 0.05,  0.1 },
    .interval_1 = { 0.01, 0.05,  0.1 },
    .precision  = { 0.01, 0.02, 0.03 }
  };

  timer.timer1 = core_timer_add(TIMEOUT_1, _timer1_cb, &timer);
  timer.timer2 = core_timer_add(TIMEOUT_2, _timer2_cb, &timer);
  timer.add_timer2++;
  timer.timer3 = core_timer_add(TIMEOUT_3, _timer3_cb, &timer);
  timer.timer4 = core_timer_add(TIMEOUT_4, _timer4_cb, &timer);
  timer.timer5 = core_timer_add(TIMEOUT_5, _timer5_cb, &timer);

  ck_assert_msg((timer.timer1 || timer.timer2 || timer.timer3 || timer.timer4 ||
                 timer.timer5),
                "Error add timer\n");

  core_main_loop_begin();

  if (timer.timer1) core_timer_del(timer.timer1);
  if (timer.timer2) core_timer_del(timer.timer2);
  if (timer.timer3) core_timer_del(timer.timer3);
  if (timer.timer4) core_timer_del(timer.timer4);
  if (timer.timer5) core_timer_del(timer.timer5);
}

EFL_END_TEST

typedef struct _Test_Inside_Call
{
  Core_Timer *t;
  int         it;
} Test_Inside_Call;

static Efl_Bool
_timeri_cb(void *data)
{
  Test_Inside_Call *c = data;

  core_timer_reset(c->t);

  c->it--;

  if (c->it > 0) return EFL_TRUE;
  free(c);
  core_main_loop_quit();
  return EFL_FALSE;
}

static Efl_Bool
timeout_timer_cb(void *)
{
  ck_abort();
  return EFL_FALSE;
}

EFL_START_TEST(core_test_timer_inside_call)
{
  Test_Inside_Call *c;

  c     = malloc(sizeof(Test_Inside_Call));
  c->it = 5;
  c->t  = core_timer_add(0.01, _timeri_cb, c);
  core_timer_add(1.0, timeout_timer_cb, NULL);

  ck_assert_msg(c->t, "Error add timer\n");

  core_main_loop_begin();
}

EFL_END_TEST

EFL_START_TEST(core_test_timer_valid_callbackfunc)
{
  Core_Timer *t = NULL;
  ck_assert_msg(!(t = core_timer_add(0.5, NULL, NULL)),
                "ERROR: Invalid callback func!\n");
}

EFL_END_TEST

static Efl_Bool
_quit_cb(void *data)
{
  Efl_Bool *val = data;
  if (val) *val = EFL_TRUE;
  core_main_loop_quit();
  return EFL_FALSE;
}

EFL_START_TEST(core_test_core_main_loop_timer)
{
  Efl_Bool    did = EFL_FALSE;
  Core_Timer *timer;
  double      start, end, elapsed;

  timer = core_timer_add(0.1, _quit_cb, &did);
  fail_if(timer == NULL);

  start = core_time_get();
  core_main_loop_begin();
  end     = core_time_get();
  elapsed = end - start;

  fail_if(did == EFL_FALSE);
  fail_if(elapsed < 0.05);
  fail_if(elapsed > 0.15); /* .05 second "error margin" */
}

EFL_END_TEST

static int count = 0;

static Efl_Bool
_timer_cb(void *data)
{
  count++;
  int num = (intptr_t)data;
  ck_assert_msg(num == count, "Error timer is called out of order");
  if (count == 8) core_main_loop_quit();
  return CORE_CALLBACK_CANCEL;
}

EFL_START_TEST(core_test_timer_in_order)
{
  Core_Timer *timer;
  timer = core_timer_add(0.001, _timer_cb, (void *)1);
  fail_if(timer == NULL);
  timer = core_timer_add(0.001, _timer_cb, (void *)2);
  fail_if(timer == NULL);
  timer = core_timer_add(0.001, _timer_cb, (void *)3);
  fail_if(timer == NULL);
  timer = core_timer_add(0.001, _timer_cb, (void *)4);
  fail_if(timer == NULL);
  timer = core_timer_add(0.001, _timer_cb, (void *)5);
  fail_if(timer == NULL);
  timer = core_timer_add(0.001, _timer_cb, (void *)6);
  fail_if(timer == NULL);
  timer = core_timer_add(0.001, _timer_cb, (void *)7);
  fail_if(timer == NULL);
  timer = core_timer_add(0.001, _timer_cb, (void *)8);
  fail_if(timer == NULL);

  core_main_loop_begin();
}

EFL_END_TEST

EFL_START_TEST(core_test_timer_iteration)
{
  Core_Timer *timer;
  count = 0;
   /* verify that timers expire after exactly one loop iteration */
  core_timer_add(0, _timer_cb, (void *)1);
  core_main_loop_iterate();
   /* timers should not expire for one loop iteration */
  ck_assert_int_eq(count, 0);
  core_main_loop_iterate();
   /* timers should expire after one loop iteration */
  ck_assert_int_eq(count, 1);

   /* verify multiple timer expiration in single mainloop iteration */
  core_timer_add(0, _timer_cb, (void *)2);
  core_timer_add(0, _timer_cb, (void *)3);
  core_main_loop_iterate();
   /* timers should not expire for one loop iteration */
  ck_assert_int_eq(count, 1);
  core_timer_add(0, _timer_cb, (void *)4);
  core_main_loop_iterate();
  /* all pending and instantiated timers should expire after exactly one loop iteration */
  ck_assert_int_eq(count, 3);
  core_main_loop_iterate();
  /* this should not interfere with successive timer processing */
  ck_assert_int_eq(count, 4);

  /* verify out-of-order timer processing solely based on timer times */
  timer = core_timer_add(1, _timer_cb, (void *)6);
  core_main_loop_iterate();
  ck_assert_int_eq(count, 4);
  core_timer_add(0, _timer_cb, (void *)5);
  core_main_loop_iterate();
  ck_assert_int_eq(count, 4);
  /* timer should expire after exactly 2 iterations */
  core_main_loop_iterate();
  ck_assert_int_eq(count, 5);
  core_timer_interval_set(timer, 0);
  core_timer_reset(timer);
  /* timer should expire after exactly 2 iterations since it becomes un-instantiated now */
  core_main_loop_iterate();
  core_main_loop_iterate();
  ck_assert_int_eq(count, 6);
}

EFL_END_TEST

static Efl_Bool
_recursion(void *)
{
  static unsigned int recurse = 0;
  static Core_Timer  *timer;

  switch (recurse++)
  {
    case 0:
      /* verify multiple timer expiration in single mainloop iteration */
      core_timer_add(0, _timer_cb, (void *)2);
      core_timer_add(0, _timer_cb, (void *)3);
      core_main_loop_iterate();
      core_main_loop_iterate();
      ck_assert_int_eq(count, 6);
      core_main_loop_quit();
      break;
    case 1:
      /* timers should not expire for one loop iteration */
      ck_assert_int_eq(count, 1);
      core_timer_add(0, _timer_cb, (void *)4);
      core_main_loop_iterate();
      core_main_loop_iterate();
      break;
    case 2:
      /* all pending and instantiated timers should expire after exactly one loop iteration */
      ck_assert_int_eq(count, 3);
      core_main_loop_iterate();
      core_main_loop_iterate();
      break;
    case 3:
      /* this should not interfere with successive timer processing */
      ck_assert_int_eq(count, 4);

      /* verify out-of-order timer processing solely based on timer times */
      timer = core_timer_add(1, _timer_cb, (void *)6);
      core_main_loop_iterate();
      core_main_loop_iterate();
      break;
    case 4:
      ck_assert_int_eq(count, 4);
      core_timer_add(0, _timer_cb, (void *)5);
      core_main_loop_iterate();
      core_main_loop_iterate();
      break;
    case 5:
      ck_assert_int_eq(count, 4);
      /* timer should expire after exactly 2 iterations */
      core_main_loop_iterate();
      core_main_loop_iterate();
      break;
    case 6:
      ck_assert_int_eq(count, 5);
      core_timer_interval_set(timer, 0);
      core_timer_reset(timer);
      /* timer should expire after exactly 2 iterations since it becomes un-instantiated now */
      core_main_loop_iterate();
      core_main_loop_iterate();
      break;
    case 7:
      core_main_loop_iterate();
      core_main_loop_iterate();
      break;
  }
  return EFL_FALSE;
}

EFL_START_TEST(core_test_timer_recursion)
{
  Efl_Bool crit      = eina_log_abort_on_critical_get();
  int      critlevel = eina_log_abort_on_critical_level_get();
  count              = 1;
  eina_log_abort_on_critical_set(1);
  eina_log_abort_on_critical_level_set(1);
  core_timer_add(0, (Core_Task_Cb)_recursion, NULL);
  core_main_loop_begin();
  ck_assert_int_eq(count, 6);
  eina_log_abort_on_critical_set(crit);
  eina_log_abort_on_critical_level_set(critlevel);
}

EFL_END_TEST

void
core_test_timer(TCase *tc)
{
  tcase_add_test(tc, core_test_timers);
  tcase_add_test(tc, core_test_timer_inside_call);
  tcase_add_test(tc, core_test_timer_valid_callbackfunc);
  tcase_add_test(tc, core_test_core_main_loop_timer);
  tcase_add_test(tc, core_test_timer_in_order);
  tcase_add_test(tc, core_test_timer_iteration);
  tcase_add_test(tc, core_test_timer_recursion);
}
