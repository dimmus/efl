/**
   Compile with gcc -o core_exe_example_child core_exe_example_child.c `pkg-config --cflags --libs ecore`
 */

#include <stdio.h>
#include <unistd.h>
#include <Efl_Core.h>

#define BUFFER_SIZE 1024

#ifdef _WIN32
#define HANDLER_TYPE Ecore_Win32_Handler
#else
#define HANDLER_TYPE Core_Fd_Handler
#endif

static Efl_Bool
_fd_handler_cb(void *data EFL_UNUSED, HANDLER_TYPE *fd_handler EFL_UNUSED)
{
   static int numberOfMessages = 0;
   char message[BUFFER_SIZE];

   if (!fgets(message, BUFFER_SIZE, stdin))
     return CORE_CALLBACK_RENEW;

   numberOfMessages++;

   if (numberOfMessages < 3)
     {
        printf("My father sent this message to me:%s\n", message);
        fflush(stdout);
        return CORE_CALLBACK_RENEW;
     }
   else
     {
        printf("quit\n");
        fflush(stdout);
        core_main_loop_quit();
        return CORE_CALLBACK_DONE;
     }
}

int
main(void)
{
   if (!core_init())
     goto error;

#ifdef _WIN32
   /* note that stdin fd's on windows don't work the same
    * as on unixes. this uses stdin just as a quick
    * example that's simple instead of a more complex
    * one, so this won't actually work on windows unless
    * you use a fd that comes from somewhere that is
    * select()able. */
   core_main_win32_handler_add(GetStdHandle(STD_INPUT_HANDLE),
                                _fd_handler_cb,
                                NULL);
#else
   core_main_fd_handler_add(STDIN_FILENO,
                             CORE_FD_READ,
                             _fd_handler_cb,
                             NULL, NULL, NULL);
#endif
   core_main_loop_begin();

   core_shutdown();

   return EXIT_SUCCESS;

error:
   return EXIT_FAILURE;
}

