/* Used:
 * eina_log_level_set
 *
 * EINA_LOG_WARN
 *
 * Compile with:
 * gcc -Wall -o eina_log_01 eina_log_01.c `pkg-config --cflags --libs shared`
 */

#include <stdlib.h>
#include <stdio.h>

#include <Efl_Shared.h>

static void
test_warn(void)
{
   EINA_LOG_WARN("Here is a warning message");
}

int
main(void)
{
   if (!eina_init())
   {
      printf("Error during the initialization of Eina_Log module\n");
      return EXIT_FAILURE;
   }

   eina_log_level_set(EINA_LOG_LEVEL_WARN);

   test_warn();

   eina_shutdown();

   return EXIT_SUCCESS;
}
