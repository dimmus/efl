//Compile with:
//gcc -g -Wall eina_array_02.c -o eina_array_02 `pkg-config --cflags --libs shared`

#include <stdio.h>
#include <string.h>

#include <Efl_Shared.h>

static Efl_Bool
keep(void *data, void *gdata EFL_UNUSED)
{
   if (strlen((const char*)data) <= 5)
      return EFL_TRUE;
   return EFL_FALSE;
}

int
main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
   const char* strs[] = {
      "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
      "ten", "eleven", "twelve", "thirteen", "fourtenn", "fifteen", "sixteen",
      "seventeen", "eighteen", "nineteen", "twenty"
   };
   const char* strings[] = {
      "helo", "hera", "starbuck", "kat", "boomer",
      "hotdog", "longshot", "jammer", "crashdown", "hardball",
      "duck", "racetrack", "apolo", "husker", "freaker",
      "skulls", "bulldog", "flat top", "hammerhead", "gonzo"
   };
   Eina_Array *array;
   Eina_Array_Iterator iterator;
   const char *item;
   unsigned int i;

   eina_init();

   array = eina_array_new(10);

   for (i = 0; i < 20; i++)
     eina_array_push(array, strs[i]);

   eina_array_clean(array);
   for (i = 0; i < 20; i++)
     eina_array_push(array, strings[i]);

   eina_array_data_set(array, 17, "flattop");

   eina_array_remove(array, keep, NULL);
   EINA_ARRAY_ITER_NEXT(array, i, item, iterator)
     printf("item #%u: %s\n", i, item);

   eina_array_free(array);

   eina_shutdown();

   return 0;
}
