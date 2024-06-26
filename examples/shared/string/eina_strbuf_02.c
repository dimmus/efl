/* Used:
 * eina_strbuf_new
 * eina_strbuf_free
 * eina_strbuf_reset
 * eina_strbuf_append
 * eina_strbuf_append_strftime
 * eina_strbuf_insert_strftime
 * eina_strbuf_string_get
 * 
 * Compile with:
 * gcc -Wall -o eina_strbuf_02 eina_strbuf_02c `pkg-config --cflags --libs shared`
 */

#include <stdio.h>
#include <Efl_Shared.h>

int main(int argc EFL_UNUSED, char **argv EFL_UNUSED)
{
   Efl_Strbuf *buf;
   time_t curr_time;
   struct tm *info;

   eina_init();

   curr_time = time(NULL);
   info = localtime(&curr_time);

   buf = eina_strbuf_new();
   eina_strbuf_append_strftime(buf, "%I:%M%p", info);
   printf("current time: %s\n", eina_strbuf_string_get(buf));
   eina_strbuf_reset(buf);

   eina_strbuf_append(buf, "Hours: Minutes");
   //insert hour at ^Hours: Minutes where ^ is the position
   eina_strbuf_prepend_strftime(buf, "%I ", info);
   //insert hour at hhhHours: ^Minutes where ^ is the position
   eina_strbuf_insert_strftime(buf, "%M ", info, 10);
   printf("%s\n", eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   eina_shutdown();

   return 0;
}
