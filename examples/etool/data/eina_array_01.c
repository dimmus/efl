//Compile with:
//gcc -g -Wall eina_array_01.c -o eina_array_01 `pkg-config --cflags --libs etool`

#include <stdio.h>
#include <string.h>

#include <Eina.h>

static Efl_Bool
_print(const void *container EFL_UNUSED, void *data, void *fdata EFL_UNUSED)
{
  printf("%s\n", (char *)data);
   return EFL_TRUE;
}

int
main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
   const char* strings[] = {
      "helo", "hera", "starbuck", "kat", "boomer",
      "hotdog", "longshot", "jammer", "crashdown", "hardball",
      "duck", "racetrack", "apolo", "husker", "freaker",
      "skulls", "bulldog", "flat top", "hammerhead", "gonzo"
   };
   Eina_Array *array;
   unsigned int i;

   eina_init();

   array = eina_array_new(10);
   eina_array_step_set(array, sizeof(*array), 20);

   for (i = 0; i < 20; i++)
     eina_array_push(array, strdup(strings[i]));

   printf("array count: %d\n", eina_array_count(array));
   eina_array_foreach(array, _print, NULL);

   printf("Top gun: %s\n", (char*)eina_array_data_get(array, 2));

   while (eina_array_count(array))
     free(eina_array_pop(array));

   eina_array_free(array);

   eina_shutdown();

   return 0;
}
