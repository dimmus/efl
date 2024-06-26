//Compile with:
// gcc -o core_con_server_example core_con_server_example.c `pkg-config --libs --cflags ecore ecore-con eina`

#ifdef HAVE_CONFIG_H
#include "efl_config.h"
#endif

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>
#include <Efl_Core_Getopt.h>

/* Ecore_Con server example
 * 2010 Mike Blumenkrantz
 */

static int retval = EXIT_SUCCESS;
static Efl_Bool echo = EFL_FALSE;
static Efl_Bool do_flush = EFL_FALSE;
static Efl_Bool single_message = EFL_FALSE;
static Efl_Bool do_ssl_upgrade = EFL_FALSE;

Efl_Bool
_add(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Con_Event_Client_Add *ev = event;

   printf("INFO: client added %p: %s\n", ev->client, core_con_client_ip_get(ev->client));

   if (!echo)
     {
        core_con_client_send(ev->client, "Hello World!", strlen("Hello World!"));
        if (do_flush) core_con_client_flush(ev->client);
        if (single_message) core_con_client_del(ev->client);
        else if (do_ssl_upgrade)
          {
             const char upgrade_msg[] = "\nSend \"Upgrade: SSL\" followed by newline (\\n) to do it.\n";
             core_con_client_send(ev->client, upgrade_msg, strlen(upgrade_msg));
             if (do_flush) core_con_client_flush(ev->client);
          }
     }

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_del(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Con_Event_Client_Del *ev = event;

   printf("INFO: client deleted %p: %s!\n", ev->client, core_con_client_ip_get(ev->client));
   core_con_client_del(ev->client);
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_data(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Con_Event_Client_Data *ev = event;
   const char upgrade_ssl[] = "Upgrade: SSL\n";

   printf("INFO: client data %p: %s\n"
          "INFO:  - size: %d\n"
          "-- BEGIN DATA --\n",
          ev->client, core_con_client_ip_get(ev->client),
          ev->size);

   fwrite(ev->data, ev->size, 1, stdout);
   puts("-- END DATA --");

   if (do_ssl_upgrade && ((size_t)ev->size == strlen(upgrade_ssl)) &&
       (memcmp(ev->data, upgrade_ssl, ev->size) == 0))
     {
        if (!core_con_ssl_client_upgrade(ev->client, CORE_CON_USE_MIXED | CORE_CON_LOAD_CERT))
          {
             printf("ERROR: Failed to upgrade client=%p %s to SSL!\n", ev->client, core_con_client_ip_get(ev->client));
             core_con_client_del(ev->client);
          }
        else
          printf("INFO: upgrading client=%p %s to SSL!\n", ev->client, core_con_client_ip_get(ev->client));

        return CORE_CALLBACK_RENEW;
     }

   if (echo)
     {
        core_con_client_send(ev->client, ev->data, ev->size);
        if (do_flush) core_con_client_flush(ev->client);
        if (single_message) core_con_client_del(ev->client);
     }

   return CORE_CALLBACK_RENEW;
}


Efl_Bool
_write_(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Con_Event_Client_Write *ev = event;
   printf("Sent %d bytes to client %s\n", ev->size, core_con_client_ip_get(ev->client));
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_error(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Con_Event_Client_Error *ev = event;
   printf("Client %s Error: %s\n", core_con_client_ip_get(ev->client), ev->error);
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_upgrade(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Con_Event_Client_Upgrade *ev = event;
   printf("Client %s Upgraded to SSL.\n", core_con_client_ip_get(ev->client));
   return CORE_CALLBACK_RENEW;
}

static const char *types_strs[] = {
  "tcp",
  "udp",
  "ssl",
  "tcp+ssl",
  "mcast",
  "local-user",
  "local-system",
  NULL
};

static const Core_Getopt options = {
  "core_con_server_example", /* program name */
  NULL, /* usage line */
  "1", /* version */
  "(C) 2016 Enlightenment Project; 2010 Mike Blumenkrantz", /* copyright */
  "BSD 2-Clause", /* license */
  /* long description, may be multiline and contain \n */
  "Example of core_con_server_add() usage.\n",
  EFL_FALSE,
  {
    CORE_GETOPT_CHOICE('t', "type", "Server type to use, defaults to 'tcp'", types_strs),
    CORE_GETOPT_STORE_TRUE(0, "socket-activated",
                            "Try to use $LISTEN_FDS from systemd, if not do a regular serve"),

    CORE_GETOPT_STORE_INT('l', "clients-limit",
                            "If set will limit number of clients to accept"),
    CORE_GETOPT_STORE_TRUE('r', "clients-reject-excess",
                            "Immediately reject excess clients (over limit)"),

    CORE_GETOPT_STORE_TRUE('e', "echo",
                            "Behave as 'echo' server, send back to client all the data receive"),
    CORE_GETOPT_STORE_TRUE('f', "flush", "Force a flush after every send call."),
    CORE_GETOPT_STORE_TRUE('m', "single-message", "Send a single message and delete the client."),

    CORE_GETOPT_VERSION('V', "version"),
    CORE_GETOPT_COPYRIGHT('C', "copyright"),
    CORE_GETOPT_LICENSE('L', "license"),
    CORE_GETOPT_HELP('h', "help"),

    CORE_GETOPT_STORE_METAVAR_STR(0, NULL, "The server name.", "name"),
    CORE_GETOPT_STORE_METAVAR_INT(0, NULL, "The server port.", "port"),

    CORE_GETOPT_SENTINEL
  }
};

int
main(int argc, char **argv)
{
   Ecore_Con_Type type;
   char *name = NULL;
   char *type_choice = NULL;
   int clients_limit = -1;
   int port = -1;
   Efl_Bool socket_activated = EFL_FALSE;
   Efl_Bool clients_reject_excess = EFL_FALSE;
   Efl_Bool quit_option = EFL_FALSE;
   Core_Getopt_Value values[] = {
     CORE_GETOPT_VALUE_STR(type_choice),
     CORE_GETOPT_VALUE_BOOL(socket_activated),

     CORE_GETOPT_VALUE_INT(clients_limit),
     CORE_GETOPT_VALUE_BOOL(clients_reject_excess),

     CORE_GETOPT_VALUE_BOOL(echo),
     CORE_GETOPT_VALUE_BOOL(do_flush),
     CORE_GETOPT_VALUE_BOOL(single_message),

     /* standard block to provide version, copyright, license and help */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -V/--version quits */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -C/--copyright quits */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -L/--license quits */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -h/--help quits */

     /* positional argument */
     CORE_GETOPT_VALUE_STR(name),
     CORE_GETOPT_VALUE_INT(port),

     CORE_GETOPT_VALUE_NONE /* sentinel */
   };
   int args;
   Ecore_Con_Server *svr;

   eina_init();
   core_init();
   core_con_init();

   args = core_getopt_parse(&options, values, argc, argv);
   if (args < 0)
     {
        fputs("ERROR: Could not parse command line options.\n", stderr);
        retval = EXIT_FAILURE;
        goto end;
     }

   if (quit_option) goto end;

   args = core_getopt_parse_positional(&options, values, argc, argv, args);
   if (args < 0)
     {
        fputs("ERROR: Could not parse positional arguments.\n", stderr);
        retval = EXIT_FAILURE;
        goto end;
     }

   if (!type_choice) type_choice = "tcp";

   if (strcmp(type_choice, "tcp") == 0)
     type = CORE_CON_REMOTE_TCP;
   else if (strcmp(type_choice, "udp") == 0)
     type = CORE_CON_REMOTE_UDP;
   else if (strcmp(type_choice, "ssl") == 0)
     type = CORE_CON_REMOTE_TCP | CORE_CON_USE_MIXED | CORE_CON_LOAD_CERT;
   else if (strcmp(type_choice, "tcp+ssl") == 0)
     {
        type = CORE_CON_REMOTE_TCP;
        do_ssl_upgrade = EFL_TRUE;
     }
   else if (strcmp(type_choice, "local-user") == 0)
     type = CORE_CON_LOCAL_USER;
   else if (strcmp(type_choice, "local-system") == 0)
     type = CORE_CON_LOCAL_SYSTEM;
   else if (strcmp(type_choice, "mcast") == 0)
     type = CORE_CON_REMOTE_MCAST;
   else
     {
        fprintf(stderr, "ERROR: unsupported --type/-t '%s'\n", type_choice);
        retval = EXIT_FAILURE;
        goto end;
     }

   if (socket_activated) type |= CORE_CON_SOCKET_ACTIVATE;

   svr = core_con_server_add(type, name, port, NULL);
   if (!svr) goto end;

   if ((strcmp(type_choice, "ssl") == 0)
#if 1
       /* This just works since EFL v 1.19.  Prior to this, loading a
        * certificate would put the whole server in SSL mode, which is
        * unexpected.
        *
        * There are no core_con_ssl_client_cert_add() to report a new
        * certificate to clients (and it shouldn't, as it would be too
        * expensive to setup for each client, this should be shared).
        *
        * With EFL v1.19 this can be done, but the whole setup should
        * be done at most once before returning to the main loop.
        */
       || (strcmp(type_choice, "tcp+ssl") == 0)
#endif
       )
     {
        if (!core_con_ssl_server_cert_add(svr, "server.pem"))
          {
             fprintf(stderr, "ERROR: could not add cert: server.pem\n");
             goto no_mainloop;
          }
        if (!core_con_ssl_server_privkey_add(svr, "server.pem"))
          {
             fprintf(stderr, "ERROR: could not add privkey: server.pem\n");
             goto no_mainloop;
          }
     }

/* set event handler for client connect */
   core_event_handler_add(CORE_CON_EVENT_CLIENT_ADD, (Core_Event_Handler_Cb)_add, NULL);
/* set event handler for client disconnect */
   core_event_handler_add(CORE_CON_EVENT_CLIENT_DEL, (Core_Event_Handler_Cb)_del, NULL);
/* set event handler for receiving client data */
   core_event_handler_add(CORE_CON_EVENT_CLIENT_DATA, (Core_Event_Handler_Cb)_data, NULL);
/* set event handler that notifies of sent data */
   core_event_handler_add(CORE_CON_EVENT_CLIENT_WRITE, (Core_Event_Handler_Cb)_write_, NULL);
/* set event handler that notifies of errors */
   core_event_handler_add(CORE_CON_EVENT_CLIENT_ERROR, (Core_Event_Handler_Cb)_error, NULL);
/* set event handler that notifies of upgrades */
   core_event_handler_add(CORE_CON_EVENT_CLIENT_UPGRADE, (Core_Event_Handler_Cb)_upgrade, NULL);

/* start server */
   core_main_loop_begin();

 no_mainloop:
   core_con_server_del(svr);
   svr = NULL;

 end:
   core_con_shutdown();
   core_shutdown();
   eina_shutdown();

   return retval;
}
