//Compile with:
// gcc -o core_con_client_simple_example core_con_client_simple_example.c `pkg-config --libs --cflags ecore ecore-con eina`

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>

struct _Server
{
   int sdata;
};

Efl_Bool
_add(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Add *ev)
{
   char welcome[] = "hello! - sent from the client";
   struct _Server *server = malloc(sizeof(*server));
   server->sdata = 0;

   core_con_server_data_set(ev->server, server);
   printf("Server with ip %s, name %s, port %d, connected = %d!\n",
          core_con_server_ip_get(ev->server),
          core_con_server_name_get(ev->server),
          core_con_server_port_get(ev->server),
          core_con_server_connected_get(ev->server));
   core_con_server_send(ev->server, welcome, sizeof(welcome));
   core_con_server_flush(ev->server);

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_del(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Del *ev)
{
   if (!ev->server)
     {
        printf("Failed to establish connection to the server.\nExiting.\n");
        core_main_loop_quit();
        return CORE_CALLBACK_RENEW;
     }

   struct _Server *server = core_con_server_data_get(ev->server);

   printf("Lost server with ip %s!\n", core_con_server_ip_get(ev->server));

   if (server)
     {
        printf("Total data received from this server: %d\n", server->sdata);
        free(server);
     }

   core_con_server_del(ev->server);

   core_main_loop_quit();
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_data(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Data *ev)
{
   char fmt[128];
   struct _Server *server = core_con_server_data_get(ev->server);

   snprintf(fmt, sizeof(fmt),
            "Received %i bytes from server:\n"
            ">>>>>\n"
            "%%.%is\n"
            ">>>>>\n",
            ev->size, ev->size);

   printf(fmt, ev->data);

   server->sdata += ev->size;
   return CORE_CALLBACK_RENEW;
}

int
main(int argc, const char *argv[])
{
   Ecore_Con_Server *svr;
   const char *address;
   int port = 8080;

   if (argc < 2)
     {
        printf("wrong usage. Command syntax is:\n");
        printf("\tcore_con_client_simple_example <address> [port]\n");
        exit(1);
     }

   address = argv[1];

   if (argc > 2)
     port = atoi(argv[2]);

   eina_init();
   core_init();
   core_con_init();

   if (!(svr = core_con_server_connect(CORE_CON_REMOTE_TCP, address, port, NULL)))
     {
        printf("could not connect to the server: %s, port %d.\n",
               address, port);
        exit(2);
     }

   /* set event handler for server connect */
   core_event_handler_add(CORE_CON_EVENT_SERVER_ADD, (Core_Event_Handler_Cb)_add, NULL);
   /* set event handler for server disconnect */
   core_event_handler_add(CORE_CON_EVENT_SERVER_DEL, (Core_Event_Handler_Cb)_del, NULL);
   /* set event handler for receiving server data */
   core_event_handler_add(CORE_CON_EVENT_SERVER_DATA, (Core_Event_Handler_Cb)_data, NULL);

   /* start client */
   core_main_loop_begin();

   core_con_shutdown();
   core_shutdown();
   eina_shutdown();

   return 0;
}

