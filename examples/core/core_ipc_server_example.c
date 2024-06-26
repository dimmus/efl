#include <Efl_Core.h>
#include <Ecore_Ipc.h>
#include <Efl_Core_Getopt.h>

static int retval = EXIT_SUCCESS;
static int max_size = -1;
static Efl_Bool echo = EFL_FALSE;
static Efl_Bool do_flush = EFL_FALSE;
static Efl_Bool single_message = EFL_FALSE;

static Efl_Bool
_client_add(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Ipc_Event_Client_Add *ev = event;

   core_ipc_client_data_size_max_set(ev->client, max_size);

   printf("INFO: client added %p: %s\n", ev->client, core_ipc_client_ip_get(ev->client));

   if (!echo)
     {
        core_ipc_client_send(ev->client, 1, 2, 3, 0, EFL_TRUE,
                              "Hello World!", strlen("Hello World!"));
        if (do_flush) core_ipc_client_flush(ev->client);
        if (single_message) core_ipc_client_del(ev->client);
     }

   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_client_del(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Ipc_Event_Client_Del *ev = event;

   printf("INFO: client deleted %p: %s\n", ev->client, core_ipc_client_ip_get(ev->client));
   core_ipc_client_del(ev->client);
   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_client_data(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Ipc_Event_Client_Data *ev = event;

   printf("INFO: client data %p: %s\n"
          "INFO:  - major: %d, minor: %d, ref: %d (to: %d)\n"
          "INFO:  - response?: %d\n"
          "INFO:  - size: %d\n"
          "-- BEGIN DATA --\n",
          ev->client, core_ipc_client_ip_get(ev->client),
          ev->major, ev->minor, ev->ref, ev->ref_to,
          ev->response,
          ev->size);

   fwrite(ev->data, ev->size, 1, stdout);
   puts("-- END DATA --");

   if (echo)
     {
        core_ipc_client_send(ev->client, ev->major, ev->minor,
                              ev->ref + 1,
                              ev->ref, 0, ev->data, ev->size);
        if (do_flush) core_ipc_client_flush(ev->client);
        if (single_message) core_ipc_client_del(ev->client);
     }

   return CORE_CALLBACK_RENEW;
}

static const char *types_strs[] = {
  "user",
  "system",
  "remote",
  NULL
};

static const Core_Getopt options = {
  "core_ipc_server_example", /* program name */
  NULL, /* usage line */
  "1", /* version */
  "(C) 2016 Enlightenment Project", /* copyright */
  "BSD 2-Clause", /* license */
  /* long description, may be multiline and contain \n */
  "Example of core_ipc_server_add() usage.\n",
  EFL_FALSE,
  {
    CORE_GETOPT_CHOICE('t', "type", "Server type to use, defaults to 'user'", types_strs),
    CORE_GETOPT_STORE_TRUE('S', "ssl", "Use SSL"),

    CORE_GETOPT_STORE_INT('s', "max-size", "Maximum size (in bytes) for messages."),

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
   Ecore_Ipc_Type type;
   char *name = NULL;
   char *type_choice = NULL;
   int clients_limit = -1;
   int port = -1;
   Efl_Bool use_ssl = EFL_FALSE;
   Efl_Bool clients_reject_excess = EFL_FALSE;
   Efl_Bool quit_option = EFL_FALSE;
   Core_Getopt_Value values[] = {
     CORE_GETOPT_VALUE_STR(type_choice),
     CORE_GETOPT_VALUE_BOOL(use_ssl),

     CORE_GETOPT_VALUE_INT(max_size),

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
   Ecore_Ipc_Server *server;
   Core_Event_Handler *handlers[3];

   core_init();
   core_ipc_init();

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

   if (!type_choice) type_choice = "user";

   if (strcmp(type_choice, "user") == 0)
     type = CORE_IPC_LOCAL_USER;
   else if (strcmp(type_choice, "system") == 0)
     type = CORE_IPC_LOCAL_SYSTEM;
   else if (strcmp(type_choice, "remote") == 0)
     type = CORE_IPC_REMOTE_SYSTEM;
   else
     {
        fprintf(stderr, "ERROR: unsupported --type/-t '%s'\n", type_choice);
        retval = EXIT_FAILURE;
        goto end;
     }

   handlers[0] = core_event_handler_add(CORE_IPC_EVENT_CLIENT_ADD, _client_add, NULL);
   handlers[1] = core_event_handler_add(CORE_IPC_EVENT_CLIENT_DEL, _client_del, NULL);
   handlers[2] = core_event_handler_add(CORE_IPC_EVENT_CLIENT_DATA, _client_data, NULL);

   if (use_ssl) type |= CORE_IPC_USE_SSL;

   server = core_ipc_server_add(type, name, port, NULL);
   if (!server)
     {
        fprintf(stderr, "ERROR: Could not create server type=%#x, name=%s, port=%d\n", type, name, port);
        goto end;
     }

   core_ipc_server_client_limit_set(server, clients_limit, clients_reject_excess);

   core_main_loop_begin();

   core_ipc_server_del(server);
   server = NULL;

   core_event_handler_del(handlers[0]);
   core_event_handler_del(handlers[1]);
   core_event_handler_del(handlers[2]);

 end:
   core_ipc_shutdown();
   core_shutdown();

   return retval;
}
