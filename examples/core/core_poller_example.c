//Compile with:
// gcc -o core_poller_example core_poller_example.c `pkg-config --libs --cflags ecore eo`

#include <Efl_Core.h>
//#include <Efl_Eo.h>
#include <unistd.h>

static double _initial_time = 0;

static Efl_Bool
_poller_print_cb(void *data)
{
   char *str = data;
   printf("Ecore Poller '%s' callback called after %0.3f seconds.\n",
          str, core_time_get() - _initial_time);

   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_poller_quit_cb(void *data EFL_UNUSED)
{

   core_main_loop_quit();
   return EFL_TRUE;
}

int
main(void)
{
   double interval = 0.3; // tick each 0.3 seconds
   Core_Poller *poller1, *poller2, *poller3;
   char *str1 = "poller1";
   char *str2 = "poller2";
   char *str3 = "poller3";

   if (!core_init())
     {
        printf("ERROR: Cannot init Ecore!\n");
        return -1;
     }

   _initial_time = core_time_get();

   core_poller_poll_interval_set(CORE_POLLER_CORE, interval);

   poller1 = core_poller_add(CORE_POLLER_CORE, 4, _poller_print_cb, str1);
   poller2 = core_poller_add(CORE_POLLER_CORE, 8, _poller_print_cb, str2);
   poller3 = core_poller_add(CORE_POLLER_CORE, 30, _poller_quit_cb, str3);

//   poller1 = efl_add_ref(CORE_POLLER_CLASS, NULL, //         core_poller_constructor(efl_added, CORE_POLLER_CORE, 4, _poller_print_cb, str1));
//   poller2 = efl_add_ref(CORE_POLLER_CLASS, NULL, //         core_poller_constructor(efl_added, CORE_POLLER_CORE, 8, _poller_print_cb, str2));
//   poller3 = efl_add_ref(CORE_POLLER_CLASS, NULL, //         core_poller_constructor(efl_added, CORE_POLLER_CORE, 20, _poller_quit_cb, str3));


   core_main_loop_begin();

   printf("changing poller2 interval to 16\n");

   core_poller_poller_interval_set(poller2, 16);
//   core_poller_interval_set(poller2, 16, NULL);
   core_main_loop_begin();

//   efl_unref(poller1);
//   efl_unref(poller2);
//   efl_unref(poller3);
   core_poller_del(poller1);
   core_poller_del(poller2);
   core_poller_del(poller3);

   core_shutdown();
}

