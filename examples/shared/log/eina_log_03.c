/* Used:
 * eina_log_print
 * eina_log_print_cb_set
 *
 * EINA_LOG_INFO
 * EINA_LOG_DBG
 * EINA_LOG_ERR
 *
 * Compile with:
 * gcc -Wall -o eina_log_03 eina_log_03.c `pkg-config --cflags --libs shared`
 */

#include <stdlib.h>
#include <stdio.h>

#include <Efl_Shared.h>

#define log(fmt, ...)                                    \
   eina_log_print(EINA_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

typedef struct _Data Data;

struct _Data
{
   int to_stderr;
};

static void
print_cb(const Eina_Log_Domain *domain,
         Eina_Log_Level level EFL_UNUSED,
         const char *file,
         const char *fnc,
         int line,
         const char *fmt,
         void *data,
         va_list args)
{
   Data *d;
   FILE *output;
   char *str;

   d = (Data*)data;
   if (d->to_stderr)
   {
      output = stderr;
      str = "stderr";
   }
   else
   {
      output = stdout;
      str = "stdout";
   }

   fprintf(output, "%s:%s:%s (%d) %s: ",
           domain->domain_str, file, fnc, line, str);
   vfprintf(output, fmt, args);
   putc('\n', output);
}

static void
test(Data *data, int i)
{
   if (i < 0)
      data->to_stderr = 0;
   else
      data->to_stderr = 1;

   EINA_LOG_INFO("Log message...");
}

int
main(void)
{
   Data data;

   if (!eina_init())
   {
      printf("Error during the initialization of Eina_Log module\n");
      return EXIT_FAILURE;
   }
   
   eina_log_level_set(EINA_LOG_LEVEL_INFO);
   eina_log_print_cb_set(print_cb, &data);

   test(&data, -1);
   test(&data, 0);

   eina_shutdown();

   return EXIT_SUCCESS;
}
