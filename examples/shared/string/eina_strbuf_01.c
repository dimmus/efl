/* Used:
 * eina_strbuf_new
 * eina_strbuf_remove
 * eina_strbuf_reset
 * eina_strbuf_free
 * eina_strbuf_append_length
 * eina_strbuf_append_char
 * eina_strbuf_tolower
 * eina_strbuf_insert_escaped
 * eina_strbuf_insert_printf
 * eina_strbuf_append_escaped
 * eina_strbuf_append_printf
 * eina_strbuf_append_char
 * eina_strbuf_append_length
 * eina_strbuf_replace_all
 * eina_strbuf_manage_read_only_new_length
 * eina_strbuf_string_get
 * eina_strbuf_substr_get
 * 
 * Compile with:
 * gcc -Wall -o eina_strbuf_01 eina_strbuf_01.c `pkg-config --cflags --libs shared`
 */

#include <stdio.h>
#include <Efl_Shared.h>

int main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
   Efl_Strbuf *buf, *substr;

   eina_init();

   buf = eina_strbuf_new();

   eina_strbuf_append_length(buf, "BUFFE", 5);
   eina_strbuf_append_char(buf, 'R');
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_tolower(buf);
   printf("%s\n", eina_strbuf_string_get(buf));

   substr = eina_strbuf_substr_get(buf, 3, 2);
   printf("%s\n", eina_strbuf_string_get(substr));
   eina_strbuf_free(substr);

   eina_strbuf_insert_escaped(buf, "my ", 0);
   printf("%s\n", eina_strbuf_string_get(buf));
   eina_strbuf_reset(buf);

   eina_strbuf_append_escaped(buf, "my buffer");
   printf("%s\n", eina_strbuf_string_get(buf));
   eina_strbuf_reset(buf);

   eina_strbuf_append_printf(buf, "%s%c", "buffe", 'r');
   eina_strbuf_insert_printf(buf, " %s: %d", 6, "length", (int)eina_strbuf_length_get(buf));
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_remove(buf, 0, 7);
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_replace_all(buf, "length", "size");
   printf("%s\n", eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);

   buf = eina_strbuf_manage_read_only_new_length("Example string", 14);
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_append_char(buf, '.');
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_append_length(buf, " Another string.", 15);
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   
   eina_shutdown();

   return 0;
}