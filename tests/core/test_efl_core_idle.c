#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>

#include <Efl_Shared.h>
#include <Efl_Core.h>

#include "efl_core_suite.h"


static Efl_Bool
_quit_cb(void *data)
{
   Efl_Bool *val = data;
   if (val) *val = EFL_TRUE;
   core_main_loop_quit();
   return EFL_FALSE;
}

static Efl_Bool
_dummy_cb(void *data)
{
   return !!data;
}

EFL_START_TEST(core_test_core_main_loop_idler)
{
   Efl_Bool did = EFL_FALSE;
   Core_Idler *idler;

   idler = core_idler_add(_quit_cb, &did);
   fail_if(idler == NULL);

   core_main_loop_begin();

   fail_if(did == EFL_FALSE);
}
EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop_idle_enterer)
{
   Efl_Bool did = EFL_FALSE;
   Core_Idle_Enterer *idle_enterer;


   idle_enterer = core_idle_enterer_add(_quit_cb, &did);
   fail_if(idle_enterer == NULL);

   core_main_loop_begin();

   fail_if(did == EFL_FALSE);

}
EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop_idle_before_enterer)
{
   Efl_Bool did = EFL_FALSE;
   Core_Idle_Enterer *idle_enterer;


   idle_enterer = core_idle_enterer_before_add(_quit_cb, &did);
   fail_if(idle_enterer == NULL);

   core_main_loop_begin();

   fail_if(did == EFL_FALSE);

}
EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop_idle_exiter)
{
   Efl_Bool did = EFL_FALSE;
   Core_Timer *timer;
   Core_Idle_Exiter *idle_exiter;


   /* make system exit idle */
   timer = core_timer_add(0.0, _dummy_cb, (void *)(long)0);
   fail_if(timer == NULL);

   idle_exiter = core_idle_exiter_add(_quit_cb, &did);
   fail_if(idle_exiter == NULL);

   core_main_loop_begin();

   fail_if(did == EFL_FALSE);

}
EFL_END_TEST


void core_test_core_idle(TCase *tc)
{
   tcase_add_test(tc, core_test_core_main_loop_idler);
   tcase_add_test(tc, core_test_core_main_loop_idle_enterer);
   tcase_add_test(tc, core_test_core_main_loop_idle_before_enterer);
   tcase_add_test(tc, core_test_core_main_loop_idle_exiter);
}
