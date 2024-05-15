//Compile with:
// gcc -o core_client_bench core_client_bench.c `pkg-config --libs --cflags ecore ecore-con eina`

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>

/* Ecore_Con client example
 * 2010 Mike Blumenkrantz
 */

#define NUM_CLIENTS 30000

static Eina_Counter *counter;
static int add = 0;
static int del = 0;

Efl_Bool
_add(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Add *ev EFL_UNUSED)
{
   ++add;
   printf("Connection #%i!\n", add);
   if (add == NUM_CLIENTS)
     core_main_loop_quit();

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_del(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Add *ev EFL_UNUSED)
{
   ++del;
   printf("Connection lost! #%i!\n", del);

   return CORE_CALLBACK_RENEW;
}

static void
_spawn(void *data EFL_UNUSED)
{
   int x;

   for (x = 0; x < NUM_CLIENTS; x++)
     {
//        printf("Creating connection %i\n", x);
        if (!core_con_server_connect(CORE_CON_REMOTE_NODELAY, "127.0.0.1", 8080, NULL))
          {
             printf("CRITICAL ERROR!\n"
                    "Could not create connection #%i!\n", x);
             exit(1);
          }
     }
   printf("***Job done***\n");
}

int
main(void)
{
   double done;
   eina_init();
   core_init();
   core_con_init();

   eina_log_domain_level_set("core_con", EINA_LOG_LEVEL_ERR);
   eina_log_domain_level_set("eina", EINA_LOG_LEVEL_ERR);
   counter = eina_counter_new("client");
   eina_counter_start(counter);
   done = core_time_get();

   core_job_add(_spawn, NULL);

/* set event handler for server connect */
   core_event_handler_add(CORE_CON_EVENT_SERVER_ADD, (Core_Event_Handler_Cb)_add, NULL);
   core_event_handler_add(CORE_CON_EVENT_SERVER_ADD, (Core_Event_Handler_Cb)_del, NULL);

/* start client */
   core_main_loop_begin();
   eina_counter_stop(counter, 1);
   printf("\nTime elapsed for %i connections: %f seconds\n%s", NUM_CLIENTS, core_time_get() - done, eina_counter_dump(counter));
   return 0;
}

