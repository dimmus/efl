//Compile with:
// gcc -o ecore_con_url_headers_example ecore_con_url_headers_example.c `pkg-config --libs --cflags ecore ecore-con eina`

#include <stdio.h>
#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>

static Efl_Bool
_url_data_cb(void *data EFL_UNUSED, int type EFL_UNUSED, void *event_info)
{
   Ecore_Con_Event_Url_Data *url_data = event_info;
   int i;

   for (i = 0; i < url_data->size; i++)
     printf("%c", url_data->data[i]);

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

   headers = ecore_con_url_response_headers_get(url_complete->url_con);

   EINA_LIST_FOREACH(headers, l, str)
     printf("header: %s\n", str);

   ecore_main_loop_quit();

   return EFL_TRUE;
}

int
main(int argc, const char *argv[])
{
   Ecore_Con_Url *ec_url = NULL;
   const char *type, *post_data = "";
   Efl_Bool r;

   if (argc < 3)
     {
        printf("need at least two parameters: < POST|GET >  <url1> [post-data]\n");
        return -1;
     }

   type = argv[1];

   if (strcmp(type, "POST") && (strcmp(type, "GET")))
     {
        printf("only POST or GET are supported by this example.\n");
        return -1;
     }

   if (argc > 3) post_data = argv[3];

   ecore_init();
   ecore_con_init();
   ecore_con_url_init();

   // check if requests are being pipelined, and set them if not:
   if (!ecore_con_url_pipeline_get())
     ecore_con_url_pipeline_set(EFL_TRUE);

   ec_url = ecore_con_url_custom_new(argv[2], type);
   if (!ec_url)
     {
        printf("error when creating ecore con url object.\n");
        goto end;
     }

   ecore_event_handler_add(ECORE_CON_EVENT_URL_DATA, _url_data_cb, NULL);
   ecore_event_handler_add(ECORE_CON_EVENT_URL_COMPLETE, _url_complete_cb, NULL);

   ecore_con_url_additional_header_add(ec_url, "User-Agent", "blablabla");
   ecore_con_url_verbose_set(ec_url, EFL_TRUE);

   ecore_con_url_httpauth_set(ec_url, "user", "password", EFL_FALSE);

   ecore_con_url_time(ec_url, ECORE_CON_URL_TIME_IFMODSINCE, 0);

   if (!strcmp(type, "GET"))
     r = ecore_con_url_get(ec_url);
   else
     r = ecore_con_url_post(ec_url, post_data, strlen(post_data), NULL);

   if (!r)
     {
        printf("could not realize request.\n");
        goto free_ec_url;
     }

   ecore_main_loop_begin();

free_ec_url:
   ecore_con_url_free(ec_url);
end:
   ecore_con_url_shutdown();
   ecore_con_shutdown();
   ecore_shutdown();

   return 0;
}

