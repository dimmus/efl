//Compile with:
// gcc -o core_con_url_ftp_example core_con_url_ftp_example.c `pkg-config --libs --cflags ecore ecore-con eina`

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

   printf("\n");
   printf("upload completed with status code: %d\n", url_complete->status);

   core_main_loop_quit();

   return EFL_TRUE;
}

int
main(int argc, const char *argv[])
{
   Ecore_Con_Url *ec_url = NULL;
   const char *file, *user, *passwd, *dir;

   if (argc < 5)
     {
        printf("./core_con_url_ftp <ftp_server_address> <username> <password> <file> <directory(optional)>\n");
        return -1;
     }

   core_con_init();
   core_con_url_init();

   ec_url = core_con_url_new(argv[1]);
   if (!ec_url)
     {
        printf("error when creating ecore con url object.\n");
        goto end;
     }

   user = argv[2];
   passwd = argv[3];
   file = argv[4];
   dir = argv[5];

   core_event_handler_add(CORE_CON_EVENT_URL_DATA, _url_data_cb, NULL);
   core_event_handler_add(CORE_CON_EVENT_URL_COMPLETE, _url_complete_cb, NULL);

   core_con_url_verbose_set(ec_url, EFL_TRUE);
   core_con_url_ftp_use_epsv_set(ec_url, EFL_TRUE);

   if( !core_con_url_ftp_upload(ec_url, file, user, passwd, dir))
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

   return 0;
}

