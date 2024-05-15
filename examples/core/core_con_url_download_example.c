//Compile with:
// gcc -o core_con_url_download_example core_con_url_download_example.c `pkg-config --libs --cflags ecore ecore-con`

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <Efl_Core.h>
#include <Ecore_Con.h>

#ifndef O_BINARY
# define O_BINARY 0
#endif

struct _request
{
   long size;
};

static Efl_Bool
_url_progress_cb(void *data EFL_UNUSED, int type EFL_UNUSED, void *event_info)
{
   Ecore_Con_Event_Url_Progress *url_progress = event_info;
   float percent;

   if (url_progress->down.total > 0)
     {
        struct _request *req = core_con_url_data_get(url_progress->url_con);
        req->size = url_progress->down.now;

        percent = (url_progress->down.now / url_progress->down.total) * 100;
        printf("Total of download complete: %0.1f (%0.0f)%%\n",
               percent, url_progress->down.now);
     }

   printf("status: %d\n", core_con_url_status_code_get(url_progress->url_con));

   return EFL_TRUE;
}

static Efl_Bool
_url_complete_cb(void *data EFL_UNUSED, int type EFL_UNUSED, void *event_info)
{
   Ecore_Con_Event_Url_Complete *url_complete = event_info;

   struct _request *req = core_con_url_data_get(url_complete->url_con);
   int nbytes = core_con_url_received_bytes_get(url_complete->url_con);

   printf("\n");
   printf("download completed with status code: %d\n", url_complete->status);
   printf("Total size of downloaded file: %ld bytes\n", req->size);
   printf("Total size of downloaded file: %d bytes "
          "(from received_bytes_get)\n", nbytes);
   core_main_loop_quit();

   return EFL_TRUE;
}

int
main(int argc, const char *argv[])
{
   Ecore_Con_Url *ec_url = NULL;
   struct _request *req;
   int fd;
   const char *filename = "downloadedfile.dat";

   if (argc < 2)
     {
        printf("need one parameter: <url>\n");
        return -1;
     }

   fd = open(filename, O_CREAT | O_BINARY | O_WRONLY | O_TRUNC, 0644);

   if (fd == -1)
     {
        printf("error: could not open file for writing: \"%s\"\n",
               filename);
        return -1;
     }

   core_init();
   core_con_init();
   core_con_url_init();

   ec_url = core_con_url_new(argv[1]);
   if (!ec_url)
     {
        printf("error when creating ecore con url object.\n");
        goto end;
     }

   req = malloc(sizeof(*req));
   req->size = 0;
   core_con_url_data_set(ec_url, req);

   core_con_url_fd_set(ec_url, fd);

   core_event_handler_add(CORE_CON_EVENT_URL_PROGRESS, _url_progress_cb, NULL);
   core_event_handler_add(CORE_CON_EVENT_URL_COMPLETE, _url_complete_cb, NULL);

   if (!core_con_url_get(ec_url))
     {
        printf("could not realize request.\n");
        goto free_ec_url;
     }

   core_main_loop_begin();

free_ec_url:
   free(req);
   core_con_url_free(ec_url);
end:

   close(fd);
   core_con_url_shutdown();
   core_con_shutdown();
   core_shutdown();

   return 0;
}

