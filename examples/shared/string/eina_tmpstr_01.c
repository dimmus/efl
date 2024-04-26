/* Used:
 * eina_tmpstr_add_length
 * eina_tmpstr_len
 * eina_tmpstr_del
 * eina_tmpstr_manage_new
 * 
 * Compile with:
 * gcc -g -Wall eina_tmpstr_01.c -o eina_tmpstr_01 `pkg-config --cflags --libs shared`
 */

#include <stdio.h>
#include <Efl_Shared.h>

int
main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
   unsigned int len;
   char *str2;
   const char *str;
   const char *str3;
   const char *prologe = "The Cylons were created by man. They rebelled. They "
                         "evolved.";

   eina_init();

   str = eina_tmpstr_add_length(prologe, 31);
   printf("%s\n", str);
   len = eina_tmpstr_len(str);
   printf("length: %d\n", len);
   eina_tmpstr_del(str);

   str2 = (char *)calloc(61, sizeof(char));
   strcpy(str2, prologe);
   str3 = eina_tmpstr_manage_new(str2);
   printf("%s\n", str3);
   len = eina_tmpstr_len(str3);
   printf("length: %d\n", len);
   eina_tmpstr_del(str3);

   eina_shutdown();

   return 0;
}
