#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>

#include <Efl_Shared.h>
#include <Efl_Core.h>

#include "efl_core_suite.h"

#define FP_ERR         (0.0000001)
#define CHECK_FP(a, b) ((a - b) < FP_ERR)

static Efl_Bool
_quit_cb(void *data)
{
  Efl_Bool *val = data;
  if (val) *val = EFL_TRUE;
  core_main_loop_quit();
  return EFL_FALSE;
}

Efl_Bool
_poller_handler(void *data)
{
  int *val = data;
  (*val)++;
  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_poller_loop_add(void *data EFL_UNUSED)
{
  core_poller_add(CORE_POLLER_CORE, 1, _quit_cb, NULL);
  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_poller_loop_fail(void *data EFL_UNUSED)
{
  core_main_loop_quit();
  ck_abort();
  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_poller_loop_del(void *data)
{
  static int count = 0;

  if (count++ == 0) core_poller_del(data);
  else core_main_loop_quit();
  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_poller_loop_modify(void *data)
{
  core_poller_poller_interval_set(data, 1);
  return EFL_FALSE;
}

EFL_START_TEST(core_test_core_main_loop_poller_add)
{
   /* Create renewable main poller */
  Core_Poller *poll_ptr =
    core_poller_add(CORE_POLLER_CORE, 1, _poller_loop_add, NULL);

   /* Enter main loop and wait for quit*/
  core_main_loop_begin();

   /* Destroy renewable main poller */
  core_poller_del(poll_ptr);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop_poller_del)
{
   /* Create renewable main poller */
  Core_Poller *poller =
    core_poller_add(CORE_POLLER_CORE, 2, _poller_loop_fail, NULL);
  core_poller_add(CORE_POLLER_CORE, 1, _poller_loop_del, poller);

   /* Enter main loop and wait for quit*/
  core_main_loop_begin();
}

EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop_poller_modify)
{
   /* Create renewable main poller */
  Core_Poller *poller = core_poller_add(CORE_POLLER_CORE, 4, _quit_cb, NULL);
  core_poller_add(CORE_POLLER_CORE, 1, _poller_loop_modify, poller);
  core_poller_add(CORE_POLLER_CORE, 4, _poller_loop_fail, NULL);

   /* Enter main loop and wait for quit*/
  core_main_loop_begin();
}

EFL_END_TEST

Efl_Bool
_poller_cb(void *data)
{
  int *val = data;
  (*val)++;
  return CORE_CALLBACK_RENEW;
}

EFL_START_TEST(core_test_core_main_loop_poller)
{
  Core_Poller *poll1_ptr      = NULL;
  int          poll1_interval = 1;
  int          poll1_counter  = 0;

  Core_Poller *poll2_ptr      = NULL;
  int          poll2_interval = 2;
  int          poll2_counter  = 0;

  Core_Poller *poll3_ptr      = NULL;
  int          poll3_interval = 4;
  int          poll3_counter  = 0;

  Efl_Bool did = EFL_FALSE;

   /* Check CORE_POLLER_CORE poll interval */
  double interval = 0.05;
  core_poller_poll_interval_set(CORE_POLLER_CORE, interval);
  fail_unless(
    CHECK_FP(core_poller_poll_interval_get(CORE_POLLER_CORE), interval));

   /* Create three pollers with different poller interval */
  poll1_ptr = core_poller_add(CORE_POLLER_CORE,
                              poll1_interval,
                              _poller_cb,
                              &poll1_counter);
  poll2_ptr = core_poller_add(CORE_POLLER_CORE,
                              poll2_interval,
                              _poller_cb,
                              &poll2_counter);
  poll3_ptr = core_poller_add(CORE_POLLER_CORE,
                              poll3_interval,
                              _poller_cb,
                              &poll3_counter);

   /* Add one time processed quit poller */
  core_poller_add(CORE_POLLER_CORE, 8, _quit_cb, &did);

   /* Enter main loop and wait 8 intervals for quit */
  core_main_loop_begin();

   /* Check each poller poll interval */
  fail_if(core_poller_poller_interval_get(poll1_ptr) != poll1_interval);
  fail_if(core_poller_poller_interval_get(poll2_ptr) != poll2_interval);
  fail_if(core_poller_poller_interval_get(poll3_ptr) != poll3_interval);

   /* Check each poller call counter */
  ck_assert_int_eq(8, poll1_counter);
  ck_assert_int_eq(4, poll2_counter);
  ck_assert_int_eq(2, poll3_counter);

   /* Destroy renewable pollers */
  core_poller_del(poll3_ptr);
  core_poller_del(poll2_ptr);
  core_poller_del(poll1_ptr);

  fail_if(did == EFL_FALSE);
}

EFL_END_TEST

void
core_test_core_poller(TCase *tc)
{
  tcase_add_test(tc, core_test_core_main_loop_poller);
  tcase_add_test(tc, core_test_core_main_loop_poller_add);
  tcase_add_test(tc, core_test_core_main_loop_poller_del);
  tcase_add_test(tc, core_test_core_main_loop_poller_modify);
}
