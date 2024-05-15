//Compile with:
// gcc -o core_con_server_http_example core_con_server_http_example.c `pkg-config --libs --cflags ecore ecore-con eina`

#include <stdio.h>
#include <sys/time.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>

static const char response_template[] =
  "HTTP/1.0 200 OK\r\n"
  "Server: Ecore_Con custom server\r\n"
  "Content-Length: %zd\r\n"
  "Content-Type: text/html; charset=UTF-8\r\n"
  "Set-Cookie: MYCOOKIE=1; path=/; expires=%s\r\n"
  "Set-Cookie: SESSIONCOOKIE=1; path=/\r\n"
  "\r\n"
  "%s";

struct _Client
{
   int sdata;
};

Efl_Bool
_add(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Client_Add *ev)
{
   struct _Client *client = malloc(sizeof(*client));
   client->sdata = 0;
   static char buf[4096];
   char welcome[] = "Welcome to Ecore_Con server!";
   time_t t;

   printf("Client with ip %s, port %d, connected = %d!\n",
          core_con_client_ip_get(ev->client),
          core_con_client_port_get(ev->client),
          core_con_client_connected_get(ev->client));

   core_con_client_data_set(ev->client, client);

   t = time(NULL);
   t += 60 * 60 * 24;
   snprintf(buf, sizeof(buf), response_template, sizeof(welcome) - 1, ctime(&t), welcome);

   core_con_client_send(ev->client, buf, strlen(buf));
   core_con_client_flush(ev->client);

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_del(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Client_Del *ev)
{
   struct _Client *client;

   if (!ev->client)
     return CORE_CALLBACK_RENEW;

   client = core_con_client_data_get(ev->client);

   printf("Lost client with ip %s!\n", core_con_client_ip_get(ev->client));
   if (client)
     {
        printf("Total data received from this client: %d\n", client->sdata);
        free(client);
     }
   printf("Client was connected for %0.3f seconds.\n",
          core_con_client_uptime_get(ev->client));

   core_con_client_del(ev->client);

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_data(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Client_Data *ev)
{
   char fmt[128];
   struct _Client *client = core_con_client_data_get(ev->client);

   snprintf(fmt, sizeof(fmt),
            "\nReceived %i bytes from client %s port %d:\n"
            ">>>>>\n"
            "%%.%is\n"
            ">>>>>\n\n",
            ev->size, core_con_client_ip_get(ev->client),
            core_con_client_port_get(ev->client), ev->size);

   printf(fmt, ev->data);

   client->sdata += ev->size;

   return CORE_CALLBACK_RENEW;
}

int
main(void)
{
   Ecore_Con_Server *svr;
   Ecore_Con_Client *cl;
   const Eina_List *clients, *l;

   eina_init();
   core_init();
   core_con_init();

   if (!(svr = core_con_server_add(CORE_CON_REMOTE_TCP, "127.0.0.1", 8080, NULL)))
     exit(1);

   core_event_handler_add(CORE_CON_EVENT_CLIENT_ADD, (Core_Event_Handler_Cb)_add, NULL);
   core_event_handler_add(CORE_CON_EVENT_CLIENT_DEL, (Core_Event_Handler_Cb)_del, NULL);
   core_event_handler_add(CORE_CON_EVENT_CLIENT_DATA, (Core_Event_Handler_Cb)_data, NULL);
   core_con_server_client_limit_set(svr, 3, 0);

   core_main_loop_begin();

   clients = core_con_server_clients_get(svr);
   printf("Clients still connected to this server when exiting: %d\n",
          eina_list_count(clients));
   EINA_LIST_FOREACH(clients, l, cl)
     {
        printf("%s\n", core_con_client_ip_get(cl));
        free(core_con_client_data_get(cl));
     }

   printf("Server was up for %0.3f seconds\n",
          core_con_server_uptime_get(svr));

   core_con_shutdown();
   core_shutdown();
   eina_shutdown();

   return 0;
}

