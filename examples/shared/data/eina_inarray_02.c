//Compile with:
//gcc -g -Wall eina_inarray_02.c -o eina_inarray_02 `pkg-config --cflags --libs shared`

#include <Efl_Shared.h>

int
main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
   const char* strings[] = {
      "helo", "hera", "starbuck", "kat", "boomer",
      "hotdog", "longshot", "jammer", "crashdown", "hardball",
      "duck", "racetrack", "apolo", "husker", "freaker",
      "skulls", "bulldog", "flat top", "hammerhead", "gonzo"
   };
   char **str, **str2;
   Eina_Inarray *iarr;
   int i;

   eina_init();
   iarr = eina_inarray_new(sizeof(char *), 0);

   for (i = 0; i < 20; i++){
     str = (char **)(&strings[i]);
      eina_inarray_push(iarr, str);
   }

   printf("Inline array of strings:\n");
   EINA_INARRAY_FOREACH(iarr, str2)
     printf("string: %s(pointer: %p)\n", *str2, str2);

   eina_inarray_free(iarr);
   eina_shutdown();
}
