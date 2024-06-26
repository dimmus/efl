//Compile with:
// gcc -o core_con_url_cookies_example core_con_url_cookies_example.c `pkg-config --libs --cflags ecore ecore-con eina`

#include <stdio.h>
#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>

#define COOKIEJAR "cookies.jar"

static Efl_Bool
_url_data_cb(void *data EFL_UNUSED, int type EFL_UNUSED, void *event_info)
{
   Ecore_Con_Event_Url_Data *url_data = event_info;
   int i;

   printf("\nData received from server:\n>>>>>\n");
   for (i = 0; i < url_data->size; i++)
     printf("%c", url_data->data[i]);
   printf("\n>>>>>>\n\n");

   return EFL_TRUE;
}

static Efl_Bool
_url_complete_cb(void *data EFL_UNUSED, int type EFL_UNUSED, void *event_info)
{
   Ecore_Con_Event_Url_Complete *url_complete = event_info;
   const Eina_List *headers, *l;
   char *str;

   printf("\n");
   printf("download completed with status code: %d\n", url_complete->status);

   headers = core_con_url_response_headers_get(url_complete->url_con);

   printf("response headers:\n");
   EINA_LIST_FOREACH(headers, l, str)
     printf("header: %s", str);

   core_con_url_cookies_jar_write(url_complete->url_con);

   core_main_loop_quit();

   return EFL_TRUE;
}

int
main(int argc, const char *argv[])
{
   Ecore_Con_Url *ec_url = NULL;
   char cmd = '\0';
   Efl_Bool r;

   if (argc < 2)
     {
        printf("need at least one parameter: <url> [command]\n");
        return -1;
     }

   if (argc > 2)
     cmd = argv[2][0];

   core_init();
   core_con_init();
   core_con_url_init();

   ec_url = core_con_url_new(argv[1]);
   if (!ec_url)
     {
        printf("error when creating ecore con url object.\n");
        goto end;
     }

   core_event_handler_add(CORE_CON_EVENT_URL_DATA, _url_data_cb, NULL);
   core_event_handler_add(CORE_CON_EVENT_URL_COMPLETE, _url_complete_cb, NULL);

   core_con_url_additional_header_add(ec_url, "User-Agent", "Ecore_Con client");

   core_con_url_cookies_init(ec_url);
   if (cmd != 'c' && cmd != 's')
     core_con_url_cookies_file_add(ec_url, COOKIEJAR);
   core_con_url_cookies_jar_file_set(ec_url, COOKIEJAR);

   switch (cmd)
     {
      case 'c': // clear
        printf("Cleaning previously set cookies.\n");
        core_con_url_cookies_clear(ec_url);
        break;

      case 's': // clear session
        printf("Cleaning previously set session cookies.\n");
        core_con_url_cookies_session_clear(ec_url);
        break;

      case 'i': // ignore session
        printf("Ignoring old session cookies.\n");
        core_con_url_cookies_ignore_old_session_set(ec_url, EFL_TRUE);
     }

   r = core_con_url_get(ec_url);
   if (!r)
     {
        printf("could not realize request.\n");
        goto free_ec_url;
     }

   core_main_loop_begin();

free_ec_url:
   core_con_url_free(ec_url);
end:
   core_con_url_shutdown();
   core_con_shutdown();
   core_shutdown();

   return 0;
}

