//Compile with:
// gcc -o core_file_download_example core_file_download_example.c `pkg-config --libs --cflags ecore eina ecore-file`

#include <stdio.h>
#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Ecore_File.h>

/*
 * core_file_download() example
 *
 * compile with:
 * gcc core_file_download_example.c `pkg-config --libs --cflags ecore-file` \
 *     -o core_file_download_example
 *
 */

#define URL      "http://www.kernel.org/pub/linux/kernel/v1.0/linux-1.0.tar.gz"
#define DST      "linux-1.0.tar.gz"
#define DST_MIME "[x-gzip]linux-1.0.tar.gz"

static Ecore_File_Download_Job *job = NULL;

void
completion_cb(void *data EFL_UNUSED, const char *file EFL_UNUSED, int status)
{
   printf("Done (status: %d)\n", status);
   job = NULL;
   core_main_loop_quit();
}

int
progress_cb(void *data EFL_UNUSED, const char *file EFL_UNUSED,
            long int dltotal, long int dlnow,
            long int ultotal EFL_UNUSED, long int ulnow EFL_UNUSED)
{
   printf("Progress: %ld/%ld\n", dlnow, dltotal);
   return CORE_FILE_PROGRESS_CONTINUE; //  continue the download
}

int
main(void)
{
   double start;
   Eina_Hash *headers = NULL;

   eina_init();
   core_init();
   core_file_init();

   if (core_file_exists(DST))
     core_file_unlink(DST);

   if (core_file_exists(DST_MIME))
     core_file_unlink(DST_MIME);

   start = core_time_get();

   if (core_file_download(URL, DST, completion_cb, progress_cb, NULL, &job))
     {
        printf("Download started successfully:\n  URL: %s\n  DEST: %s\n  JOB: %p\n", URL, DST, job);
        core_main_loop_begin();
        if (job)
          {
             printf("\nMain loop aborted! Abort download!\n");
             core_file_download_abort(job);
             job = NULL;
          }
        else
          {
             printf("\nTime elapsed: %f seconds\n", core_time_get() - start);
             printf("Downloaded %lld bytes\n", core_file_size(DST));
          }
     }
   else
     {
        printf("Error, can't start download\n");
        goto done;
     }

   headers = eina_hash_string_small_new(NULL);
   eina_hash_add(headers, "Content-type", "application/x-gzip");

   if (core_file_download_full(URL, DST_MIME, completion_cb, progress_cb, NULL, &job, headers))
     {
        printf("Download started successfully:\n  URL: %s\n  DEST: %s\n  JOB: %p\n", URL, DST_MIME, job);
        core_main_loop_begin();
        if (job)
          {
             printf("\nMain loop aborted! Abort download!\n");
             core_file_download_abort(job);
             job = NULL;
          }
        else
          {
             printf("\nTime elapsed: %f seconds\n", core_time_get() - start);
             printf("Downloaded %lld bytes\n", core_file_size(DST_MIME));
          }
     }
   else
     {
        printf("Error, can't start download\n");
        goto done;
     }

done:
   if (headers) eina_hash_free(headers);
   core_file_shutdown();
   core_shutdown();
   eina_shutdown();
   return 0;
}

