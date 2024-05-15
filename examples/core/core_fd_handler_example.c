/*
 * gcc -o core_fd_handler_example core_fd_handler_example.c `pkg-config --cflags --libs ecore`
 */

#include <Efl_Core.h>
#include <unistd.h>

struct context
{
   Core_Fd_Handler *handler;
   Core_Timer      *timer;
};

static void
_fd_prepare_cb(void *data EFL_UNUSED, Core_Fd_Handler *handler EFL_UNUSED)
{
   printf("prepare_cb called.\n");
}

static Efl_Bool
_fd_handler_cb(void *data, Core_Fd_Handler *handler)
{
   struct context *ctxt = data;
   char buf[1024];
   size_t nbytes;
   int fd;

   if (core_main_fd_handler_active_get(handler, CORE_FD_ERROR))
     {
        printf("An error has occurred. Stop watching this fd and quit.\n");
        core_main_loop_quit();
        ctxt->handler = NULL;
        return CORE_CALLBACK_CANCEL;
     }

   fd = core_main_fd_handler_fd_get(handler);
   nbytes = read(fd, buf, sizeof(buf));
   if (nbytes == 0)
     {
        printf("Nothing to read, exiting...\n");
        core_main_loop_quit();
        ctxt->handler = NULL;
        return CORE_CALLBACK_CANCEL;
     }
   buf[nbytes - 1] = '\0';

   printf("Read %zd bytes from input: \"%s\"\n", nbytes - 1, buf);

   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_timer_cb(void *data EFL_UNUSED)
{
   printf("Timer expired after 5 seconds...\n");

   return CORE_CALLBACK_RENEW;
}

int
main(void)
{
   struct context ctxt = {0};

   if (!core_init())
     {
        printf("ERROR: Cannot init Ecore!\n");
        return -1;
     }

   ctxt.handler = core_main_fd_handler_add(STDIN_FILENO,
                                            CORE_FD_READ | CORE_FD_ERROR,
                                            _fd_handler_cb,
                                            &ctxt, NULL, NULL);
   core_main_fd_handler_prepare_callback_set(ctxt.handler, _fd_prepare_cb, &ctxt);
   ctxt.timer = core_timer_add(5, _timer_cb, &ctxt);

   printf("Starting the main loop. Type anything and hit <enter> to "
          "activate the fd_handler callback, or CTRL+d to shutdown.\n");

   core_main_loop_begin();

   if (ctxt.handler)
     core_main_fd_handler_del(ctxt.handler);

   if (ctxt.timer)
     core_timer_del(ctxt.timer);

   core_shutdown();

   return 0;
}

