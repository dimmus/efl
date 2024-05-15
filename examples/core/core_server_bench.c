//Compile with:
// gcc -o core_server_bench core_server_bench.c `pkg-config --libs --cflags ecore eina ecore-con`

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>

/* Ecore_Con server example
 * 2010 Mike Blumenkrantz
 */

static Ecore_Con_Server *svr;
static int add;
static int del;

Efl_Bool
_add(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Client_Add *ev EFL_UNUSED)
{
   ++add;
//   printf ("%s ", core_con_client_ip_get(ev->client));
   printf("Client #%i!\n", add);
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_del(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Client_Del *ev EFL_UNUSED)
{
   ++del;
   printf("Disconnected #%i!\n", del);
   if (add == del)
     core_main_loop_quit();
   return CORE_CALLBACK_RENEW;
}

int
main(int argc, const char *argv[])
{
   core_init();
   core_con_init();
   core_app_args_set(argc, argv);
   eina_log_domain_level_set("core_con", EINA_LOG_LEVEL_ERR);
   eina_log_domain_level_set("eina", EINA_LOG_LEVEL_ERR);

/* to use a PEM certificate with TLS and SSL3, uncomment the lines below */
//   if (!(svr = core_con_server_add(CORE_CON_REMOTE_NODELAY | CORE_CON_USE_MIXED | CORE_CON_LOAD_CERT, "127.0.0.1", 8080, NULL)))

/* to use simple tcp with ssl/tls, use this line */
   svr = core_con_server_add(CORE_CON_REMOTE_NODELAY, "127.0.0.1", 8080, NULL);
   if (!svr)
     exit(1);

   core_event_handler_add(CORE_CON_EVENT_CLIENT_ADD, (Core_Event_Handler_Cb)_add, NULL);
   core_event_handler_add(CORE_CON_EVENT_CLIENT_DEL, (Core_Event_Handler_Cb)_del, NULL);

/* start server */
   core_main_loop_begin();
   if (add && del)
     {
        printf("Restarting server after %i connections\n", add);
        add = del = 0;
        core_con_server_del(svr);
        core_app_restart();
     }
   return 0;
}

