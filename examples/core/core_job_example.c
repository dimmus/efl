//Compile with:
// gcc -o core_job_example core_job_example.c `pkg-config --libs --cflags ecore`

#include <Efl_Core.h>
#include <unistd.h>

static void
_job_print_cb(void *data)
{
   char *str = data;

   printf("%s\n", str);
}

static void
_job_quit_cb(void *data EFL_UNUSED)
{
   core_main_loop_quit();
}

int
main(void)
{
   Core_Job *job1, *job2, *job3, *job_quit;
   char *str1 = "Job 1 started.";
   char *str2 = "Job 2 started.";
   char *str3 = "Job 3 started.";

   if (!core_init())
     {
        printf("ERROR: Cannot init Efl_Core!\n");
        return -1;
     }

   job1 = core_job_add(_job_print_cb, str1);
   job2 = core_job_add(_job_print_cb, str2);
   job3 = core_job_add(_job_print_cb, str3);

   job_quit = core_job_add(_job_quit_cb, NULL);

   (void)job1;
   (void)job3;
   (void)job_quit;

   printf("Created jobs 1, 2, 3 and quit.\n");

   if (job2)
     {
        char *str;
        str = core_job_del(job2);
        job2 = NULL;
        printf("Deleted job 2. Its data was: \"%s\"\n", str);
     }

   core_main_loop_begin();
   core_shutdown();

   return 0;
}