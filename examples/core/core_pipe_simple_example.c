//Compile with:
//gcc -g -Wall -o core_pipe_simple_example core_pipe_simple_example.c `pkg-config --cflags --libs ecore`

#include <unistd.h>
#include <Efl_Core.h>

static void
do_lengthy_task(Core_Pipe *pipe)
{
   int i, j;
   char *buffer;
   for (i = 0; i < 20; i++)
     {
        sleep(1);
        buffer = malloc(sizeof(char) * i);
        for (j = 0; j < i; j++)
          buffer[j] = 'a' + j;
        core_pipe_write(pipe, buffer, i);
        free(buffer);
     }
   core_pipe_write(pipe, "close", 5);
}

static void
handler(void *data EFL_UNUSED, void *buf, unsigned int len)
{
   char *str = malloc(sizeof(char) * len + 1);
   memcpy(str, buf, len);
   str[len] = '\0';
   printf("received %u bytes\n", len);
   printf("content: %s\n", (const char *)str);
   free(str);
   if (len && !strncmp(buf, "close", len < 5 ? len : 5))
     {
        printf("close requested\n");
        core_main_loop_quit();
     }
}

int
main(void)
{
   Core_Pipe *pipe;
   pid_t child_pid;

   core_init();

   pipe = core_pipe_add(handler, NULL);

   child_pid = fork();
   if (!child_pid)
     {
        core_pipe_read_close(pipe);
        do_lengthy_task(pipe);
     }
   else
     {
        core_pipe_write_close(pipe);
        core_main_loop_begin();
     }

   core_pipe_del(pipe);
   core_shutdown();

   return 0;
}

