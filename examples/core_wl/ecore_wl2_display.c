//Compile with:
//gcc -g -Wall ecore_wl_display.c -o ecore_wl_display `pkg-config --cflags --libs shared ecore ecore-wl2`
//gcc -g -Wall ecore_wl_display.c -o ecore_wl_display `pkg-config --cflags --libs efl-one`

#ifndef EFL_BETA_API_SUPPORT
# define EFL_BETA_API_SUPPORT
#endif

#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Efl_Core_Wl.h>

static Ecore_Wl_Display *
_display_setup(void)
{
    return ecore_wl_display_create("Test_Display");
}

int
main(int argc, char **argv)
{
   Ecore_Wl_Display *disp;

   eina_log_domain_level_set("core_wl", EINA_LOG_LEVEL_DBG);
   eina_log_level_set(EINA_LOG_LEVEL_DBG);

   disp = _display_setup();

   if (!disp)
   {
      fprintf(stderr, "ERROR: incorrect usage.\nUsage: %s <src> <dst>\n", argv[0]);
      return EXIT_FAILURE;
   }

   return 0;
}