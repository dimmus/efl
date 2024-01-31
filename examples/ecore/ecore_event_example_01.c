//Compile with:
// gcc -g -Wall -o ecore_event_example_01 ecore_event_example_01.c `pkg-config --cflags --libs ecore`

#include <Ecore.h>

static Efl_Bool
_quitter(void *data EFL_UNUSED, int ev_type EFL_UNUSED, void *event EFL_UNUSED)
{
   printf("Leaving already?\n");
   ecore_main_loop_quit();
   return ECORE_CALLBACK_DONE;
}

int
main(void)
{
   ecore_init();

   ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, _quitter, NULL);
   ecore_main_loop_begin();

   return 0;
}

