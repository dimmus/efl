//Compile with:
// gcc -o core_idler_example core_idler_example.c `pkg-config --libs --cflags ecore eo`

#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <Efl_Core.h>
#include <Efl_Eo.h>
#include <unistd.h>

struct context   // helper struct to give some context to the callbacks
{
   int                  count;
   Core_Idle_Enterer  *enterer;
   Core_Idler         *idler;
   Core_Idle_Exiter   *exiter;
   Core_Event_Handler *handler;
   Core_Timer         *timer;
};

static int _event_type = 0; // a new type of event will be defined and stored here

static Efl_Bool
_enterer_cb(void *data EFL_UNUSED) // the idle enterer callback
{
   printf("IDLE ENTERER: Ecore entering in idle state.\n");

   return CORE_CALLBACK_RENEW; // same as EFL_TRUE
}

static Efl_Bool
_exiter_cb(void *data EFL_UNUSED) // the idle exiter callback
{
   printf("IDLE EXITER: Ecore exiting idle state.\n");

   return CORE_CALLBACK_RENEW; // same as EFL_TRUE
}

static Efl_Bool
_idler_cb(void *data) // the idler callback - ran while the mainloop is idle
{
   struct context *ctxt = data;
   printf("IDLER: executing idler callback while in idle state.\n");

   ctxt->count++;

   /* each 10 times that the callback gets called, generate an event that
    * will wake up the main loop, triggering idle enterers, exiters, etc. */
   if ((ctxt->count % 10) == 0)
     core_event_add(_event_type, NULL, NULL, NULL);

   return CORE_CALLBACK_RENEW; // same as EFL_TRUE
}

static Efl_Bool
_event_handler_cb(void *data, int type EFL_UNUSED, void *event EFL_UNUSED) // event callback
{
   struct context *ctxt = data;

   printf("EVENT: processing callback for the event received.\n");

   if (ctxt->count > 100)
     {
        core_idle_enterer_del(ctxt->enterer);
        core_idle_exiter_del(ctxt->exiter);
//        core_idler_del(ctxt->idler);

        ctxt->enterer = NULL;
        ctxt->exiter = NULL;
        ctxt->idler = NULL;

        if (ctxt->timer)
          {
             core_timer_del(ctxt->timer);
             ctxt->timer = NULL;
          }

        core_main_loop_quit();
     }

   return CORE_CALLBACK_DONE; // same as EFL_FALSE
}

static Efl_Bool
_timer_cb(void *data)
{
   struct context *ctxt = data;
   printf("TIMER: timer callback called.\n");

   if (ctxt->timer)
     ctxt->timer = NULL;

   return CORE_CALLBACK_CANCEL; // same as EFL_FALSE
}

int
main(void)
{
   struct context ctxt = {0};

   if (!core_init())
     {
        printf("ERROR: Cannot init Efl_Core!\n");
        return -1;
     }

   _event_type = core_event_type_new();

   ctxt.enterer = core_idle_enterer_add(_enterer_cb, &ctxt);
   ctxt.exiter = core_idle_exiter_add(_exiter_cb, &ctxt);
   ctxt.idler = core_idler_add(_idler_cb, &ctxt);
//   ctxt.idler = efl_add_ref(CORE_IDLER_CLASS, NULL, core_idler_constructor(efl_added, _idler_cb, &ctxt));
   ctxt.handler = core_event_handler_add(_event_type,
                                          _event_handler_cb,
                                          &ctxt);
   ctxt.timer = core_timer_add(0.0005, _timer_cb, &ctxt);

   core_main_loop_begin();
   core_shutdown();

   return 0;
}

