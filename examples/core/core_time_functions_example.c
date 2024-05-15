//Compile with:
// gcc -o core_time_functions_example core_time_functions_example.c `pkg-config --libs --cflags ecore`

#include <Efl_Core.h>
#include <unistd.h>

static Efl_Bool
_timer_cb(void *data EFL_UNUSED)
{
   printf("ecore time: %0.3f\n", core_time_get());
   printf("loop time: %0.3f\n", core_loop_time_get());
   printf("unix time: %0.3f\n", core_time_unix_get());
   printf("\nSleep for 1 second...\n\n");
   sleep(1);
   printf("ecore time: %0.3f\n", core_time_get());
   printf("loop time: %0.3f\n", core_loop_time_get());
   printf("unix time: %0.3f\n", core_time_unix_get());

   core_main_loop_quit();

   return EFL_FALSE;
}

int
main(void)
{
   if (!core_init())
     {
        printf("ERROR: Cannot init Ecore!\n");
        return -1;
     }

   core_timer_add(0.1, _timer_cb, NULL);
   core_main_loop_begin();
   core_shutdown();
}

