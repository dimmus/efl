/* Used:
 * eina_error_set
 * eina_error_get
 * eina_error_msg_static_register
 * eina_error_msg_get
 * 
 * Compile with:
 * gcc -g -Wall eina_error_01.c -o eina_error_01 `pkg-config --cflags --libs etool`
 */

#include <stdlib.h>
#include <stdio.h>

#include <eina_main.h>
#include <eina_error.h>

Eina_Error MY_ERROR_NEGATIVE;
Eina_Error MY_ERROR_NULL;

static void *
data_new(void)
{
   eina_error_set(0);

   eina_error_set(MY_ERROR_NULL);
   return NULL;
}

static int
test(int n)
{
   eina_error_set(0);

   if (n < 0)
   {
      eina_error_set(MY_ERROR_NEGATIVE);
      return 0;
   }

   return 1;
}

int 
main(void)
{
   void *data;

   if (!eina_init())
   {
      printf ("Error during the initialization of Eina_Error module\n");
      return EXIT_FAILURE;
   }

   MY_ERROR_NEGATIVE = eina_error_msg_static_register("Negative number");
   MY_ERROR_NULL     = eina_error_msg_static_register("NULL pointer");

   data = data_new();
   
   if (!data)
   {
      Eina_Error err;

      err = eina_error_get();
      if (err)
         printf("Error during memory allocation: %s\n", eina_error_msg_get(err));
   }

   if (!test(0))
   {
      Eina_Error err;

      err = eina_error_get();
      if (err)
         printf("Error during test function: %s\n", eina_error_msg_get(err));
   }

   if (!test(-1))
   {
      Eina_Error err;

      err = eina_error_get();
      if (err)
         printf("Error during test function: %s\n", eina_error_msg_get(err));
   }

   eina_shutdown();

   return EXIT_SUCCESS;
}
