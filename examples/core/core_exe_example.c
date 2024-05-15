/**
   Compile with gcc -o core_exe_example core_exe_example.c `pkg-config --cflags --libs ecore`
 */

#include <stdio.h>
#include <string.h>
#include <Efl_Core.h>

#define BUFFER_SIZE 1024

static Efl_Bool
_msg_from_child_handler(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Core_Exe_Event_Data *dataFromProcess = (Core_Exe_Event_Data *)event;
   char msg[BUFFER_SIZE];

   if (dataFromProcess->size >= (BUFFER_SIZE - 1))
     {
        printf("Data too big for bugger. error\n");
        return CORE_CALLBACK_DONE;
     }

   strncpy(msg, dataFromProcess->data, dataFromProcess->size);
   msg[dataFromProcess->size] = 0;

   if (strcmp(msg, "quit") == 0)
     {
        printf("My child said to me, QUIT!\n");
        core_main_loop_quit();
     }
   else
     printf("I received a message from my child: %s\n", msg);

   return CORE_CALLBACK_DONE;
}

static Efl_Bool
_sendMessage(void *data)
{
   static int numberOfMessages = 0;
   Core_Exe *childHandle = (Core_Exe *)data;
   char msg[BUFFER_SIZE];

   sprintf(msg, " Message: %d\n", numberOfMessages);
   numberOfMessages++;

   if (core_exe_send(childHandle, msg, strlen(msg)) != EFL_TRUE)
     fprintf(stderr, "Could not send my name to the child\n");
   else
     printf(
             "I'm the father and I sent this message to the child: %s\n", msg);

   return CORE_CALLBACK_RENEW;
}

int
main(void)
{
   pid_t childPid;
   Core_Exe *childHandle;

   if (!core_init())
     goto exit;

   childHandle = core_exe_pipe_run("./core_exe_example_child",
                                    CORE_EXE_PIPE_WRITE |
                                    CORE_EXE_PIPE_READ_LINE_BUFFERED |
                                    CORE_EXE_PIPE_READ, NULL);

   if (childHandle == NULL)
     {
        fprintf(stderr, "Could not create a child process!\n");
        goto core_shutdown;
     }

   childPid = core_exe_pid_get(childHandle);

   if (childPid == -1)
     fprintf(stderr, "Could not retrieve the PID!\n");
   else
     printf("The child process has PID:%u\n", (unsigned int)childPid);

   core_event_handler_add(CORE_EXE_EVENT_DATA, _msg_from_child_handler, NULL);
   core_timer_add(1, _sendMessage, childHandle);

   core_main_loop_begin();

   core_exe_free(childHandle); //This will not affect the child process

   core_shutdown();

   return EXIT_SUCCESS;

core_shutdown:
   core_shutdown();

exit:
   return EXIT_FAILURE;
}

