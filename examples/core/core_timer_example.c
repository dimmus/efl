//Compile with:
// gcc -o core_timer_example core_timer_example.c `pkg-config --libs --cflags ecore`

#include <Efl_Core.h>
#include <unistd.h>

#define TIMEOUT_1 1.0 // interval for timer1
#define TIMEOUT_2 3.0 // timer2 - delay timer1
#define TIMEOUT_3 8.2 // timer3 - pause timer1
#define TIMEOUT_4 11.0 // timer4 - resume timer1
#define TIMEOUT_5 14.0 // timer5 - change interval of timer1
#define TIMEOUT_6 18.0 // top timer1 and start timer7 and timer8 with changed precision
#define TIMEOUT_7 1.1 // interval for timer7
#define TIMEOUT_8 1.2 // interval for timer8
#define DELAY_1   3.0 // delay time for timer1 - used by timer2
#define INTERVAL1 2.0 // new interval for timer1 - used by timer5

static double _initial_time = 0;

struct context   // helper struct to give some context to the callbacks
{
   Core_Timer *timer1;
   Core_Timer *timer2;
   Core_Timer *timer3;
   Core_Timer *timer4;
   Core_Timer *timer5;
   Core_Timer *timer6;
   Core_Timer *timer7;
   Core_Timer *timer8;
};

static double
_get_current_time(void)
{
   return core_time_get() - _initial_time;
}

static Efl_Bool
_timer1_cb(void *data EFL_UNUSED)
{
   printf("Timer1 expired after %0.3f seconds.\n", _get_current_time());
   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_timer2_cb(void *data)
{
   struct context *ctxt = data;
   printf("Timer2 expired after %0.3f seconds. "
          "Adding delay of %0.3f seconds to timer1.\n",
          _get_current_time(), DELAY_1);

   core_timer_delay(ctxt->timer1, DELAY_1);

   ctxt->timer2 = NULL;
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_timer3_cb(void *data)
{
   struct context *ctxt = data;
   printf("Timer3 expired after %0.3f seconds. "
          "Freezing timer1.\n", _get_current_time());

   core_timer_freeze(ctxt->timer1);

   ctxt->timer3 = NULL;
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_timer4_cb(void *data)
{
   struct context *ctxt = data;
   printf("Timer4 expired after %0.3f seconds. "
          "Resuming timer1, which has %0.3f seconds left to expire.\n",
          _get_current_time(), core_timer_pending_get(ctxt->timer1));

   core_timer_thaw(ctxt->timer1);

   ctxt->timer4 = NULL;
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_timer5_cb(void *data)
{
   struct context *ctxt = data;
   double interval = core_timer_interval_get(ctxt->timer1);

   printf("Timer5 expired after %0.3f seconds. "
          "Changing interval of timer1 from %0.3f to %0.3f seconds.\n",
          _get_current_time(), interval, INTERVAL1);

   core_timer_interval_set(ctxt->timer1, INTERVAL1);

   ctxt->timer5 = NULL;
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_timer7_cb(void *data)
{
   struct context *ctxt = data;
   printf("Timer7 expired after %0.3f seconds.\n", _get_current_time());

   ctxt->timer7 = NULL;
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_timer8_cb(void *data)
{
   struct context *ctxt = data;
   printf("Timer8 expired after %0.3f seconds.\n", _get_current_time());

   ctxt->timer8 = NULL;
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_timer6_cb(void *data)
{
   struct context *ctxt = data;
   printf("Timer6 expired after %0.3f seconds.\n", _get_current_time());

   printf("Stopping timer1.\n");

   core_timer_del(ctxt->timer1);
   ctxt->timer1 = NULL;

   printf("Starting timer7 (%0.3fs) and timer8 (%0.3fs).\n",
          TIMEOUT_7, TIMEOUT_8);

   ctxt->timer7 = core_timer_add(TIMEOUT_7, _timer7_cb, ctxt);
   ctxt->timer8 = core_timer_add(TIMEOUT_8, _timer8_cb, ctxt);

   core_timer_precision_set(0.2);

   ctxt->timer6 = NULL;
   return CORE_CALLBACK_CANCEL;
}

int
main(void)
{
   struct context ctxt = {0};

   if (!core_init())
     {
        printf("ERROR: Cannot init Ecore!\n");
        return -1;
     }

   _initial_time = core_time_get();

   ctxt.timer1 = core_timer_add(TIMEOUT_1, _timer1_cb, &ctxt);
   ctxt.timer2 = core_timer_add(TIMEOUT_2, _timer2_cb, &ctxt);
   ctxt.timer3 = core_timer_add(TIMEOUT_3, _timer3_cb, &ctxt);
   ctxt.timer4 = core_timer_add(TIMEOUT_4, _timer4_cb, &ctxt);
   ctxt.timer5 = core_timer_add(TIMEOUT_5, _timer5_cb, &ctxt);
   ctxt.timer6 = core_timer_add(TIMEOUT_6, _timer6_cb, &ctxt);

   printf("start the main loop.\n");

   core_main_loop_begin();

   if (ctxt.timer1)
     core_timer_del(ctxt.timer1);
   if (ctxt.timer2)
     core_timer_del(ctxt.timer2);
   if (ctxt.timer3)
     core_timer_del(ctxt.timer3);
   if (ctxt.timer4)
     core_timer_del(ctxt.timer4);
   if (ctxt.timer5)
     core_timer_del(ctxt.timer5);
   if (ctxt.timer6)
     core_timer_del(ctxt.timer6);
   if (ctxt.timer7)
     core_timer_del(ctxt.timer7);
   if (ctxt.timer8)
     core_timer_del(ctxt.timer8);

   core_shutdown();

   return 0;
}

