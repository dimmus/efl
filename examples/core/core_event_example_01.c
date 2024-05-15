//Compile with:
// gcc -g -Wall -o core_event_example_01 core_event_example_01.c `pkg-config --cflags --libs ecore`

#include <Efl_Core.h>

static Efl_Bool
_quitter(void *data EFL_UNUSED, int ev_type EFL_UNUSED, void *event EFL_UNUSED)
{
   printf("Leaving already?\n");
   core_main_loop_quit();
   return CORE_CALLBACK_DONE;
}

int
main(void)
{
   core_init();

   core_event_handler_add(CORE_EVENT_SIGNAL_EXIT, _quitter, NULL);
   core_main_loop_begin();

   return 0;
}

