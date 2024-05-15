#include <Efl_Core.h>
#include <Ecore_Ipc.h>
#include <Efl_Core_Getopt.h>

static int retval = EXIT_SUCCESS;
static Ecore_Ipc_Server *server = NULL;
static Efl_Bool do_flush = EFL_FALSE;
static Efl_Bool single_message = EFL_FALSE;

static Efl_Bool
_server_add(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Ipc_Event_Server_Add *ev = event;

   printf("INFO: server added %p: %s\n", ev->server, core_ipc_server_ip_get(ev->server));

   puts("INFO: start typing some lines of text to send to server...");

   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_server_del(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Ipc_Event_Server_Del *ev = event;

   printf("INFO: server deleted %p: %s\n", ev->server, core_ipc_server_ip_get(ev->server));

   core_ipc_server_del(server);
   server = NULL;
   core_main_loop_quit();
   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_server_data(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Ipc_Event_Server_Data *ev = event;

   printf("INFO: server data %p: %s\n"
          "INFO:  - major: %d, minor: %d, ref: %d (to: %d)\n"
          "INFO:  - response?: %d\n"
          "INFO:  - size: %d\n"
          "-- BEGIN DATA --\n",
          ev->server, core_ipc_server_ip_get(ev->server),
          ev->major, ev->minor, ev->ref, ev->ref_to,
          ev->response,
          ev->size);

   fwrite(ev->data, ev->size, 1, stdout);
   puts("-- END DATA --");

   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_on_stdin(void *data EFL_UNUSED, Core_Fd_Handler *fdh EFL_UNUSED)
{
   char *line = NULL;
#ifdef _WIN32
   char lbuf[4096] = "";
   ssize_t r;
   if (fgets(lbuf, sizeof(lbuf), stdin) == NULL)
     r = -1;
   else
     {
        line = strdup(lbuf);
        r = strlen(line);
     }
#else
   size_t len = 0;
   ssize_t r = getline(&line, &len, stdin);
#endif

   if (r < 0)
     {
        fprintf(stderr, "ERROR: could not read from stdin: %s\n", strerror(errno));
        return CORE_CALLBACK_CANCEL;
     }

   if (!server)
     fputs("WARNING: not connected to server, ignored input.\n", stderr);
   else
     {
        core_ipc_server_send(server, 1, 2, 0, 0, 0, line, r);
        printf("INFO: sent %zd bytes to server.\n", r);
        if (do_flush) core_ipc_server_flush(server);
        if (single_message)
          {
             core_ipc_server_del(server);
             server = NULL;
             core_main_loop_quit();
          }
     }

   free(line);

   return CORE_CALLBACK_RENEW;
}

static const char *types_strs[] = {
  "user",
  "system",
  "remote",
  NULL
};

static const Core_Getopt options = {
  "core_ipc_client_example", /* program name */
  NULL, /* usage line */
  "1", /* version */
  "(C) 2016 Enlightenment Project", /* copyright */
  "BSD 2-Clause", /* license */
  /* long description, may be multiline and contain \n */
  "Example of core_ipc_server_connect() usage.\n",
  EFL_FALSE,
  {
    CORE_GETOPT_CHOICE('t', "type", "Server type to use, defaults to 'user'", types_strs),
    CORE_GETOPT_STORE_TRUE('S', "ssl", "Use SSL"),
    CORE_GETOPT_STORE_TRUE('P', "no-proxy", "Do not use SOCKS proxy for remote connections."),

    CORE_GETOPT_STORE_INT('s', "max-size", "Maximum size (in bytes) for messages."),

    CORE_GETOPT_STORE_TRUE('f', "flush", "Force a flush after every send call."),
    CORE_GETOPT_STORE_TRUE('m', "single-message", "Send a single message and delete the server."),

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
   int port = -1;
   int max_size = -1;
   Efl_Bool use_ssl = EFL_FALSE;
   Efl_Bool no_proxy = EFL_FALSE;
   Efl_Bool quit_option = EFL_FALSE;
   Core_Getopt_Value values[] = {
     CORE_GETOPT_VALUE_STR(type_choice),
     CORE_GETOPT_VALUE_BOOL(use_ssl),
     CORE_GETOPT_VALUE_BOOL(no_proxy),

     CORE_GETOPT_VALUE_INT(max_size),

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

   handlers[0] = core_event_handler_add(CORE_IPC_EVENT_SERVER_ADD, _server_add, NULL);
   handlers[1] = core_event_handler_add(CORE_IPC_EVENT_SERVER_DEL, _server_del, NULL);
   handlers[2] = core_event_handler_add(CORE_IPC_EVENT_SERVER_DATA, _server_data, NULL);

   core_main_fd_handler_add(STDIN_FILENO, CORE_FD_READ, _on_stdin, NULL, NULL, NULL);

   if (use_ssl) type |= CORE_IPC_USE_SSL;
   if (no_proxy) type |= CORE_IPC_NO_PROXY;

   server = core_ipc_server_connect(type, name, port, NULL);
   if (!server)
     {
        fprintf(stderr, "ERROR: Could not connect to server type=%#x, name=%s, port=%d\n", type, name, port);
        goto end;
     }

   core_ipc_server_data_size_max_set(server, max_size);

   core_main_loop_begin();

   if (server)
     {
        core_ipc_server_del(server);
        server = NULL;
     }

   core_event_handler_del(handlers[0]);
   core_event_handler_del(handlers[1]);
   core_event_handler_del(handlers[2]);

 end:
   core_ipc_shutdown();
   core_shutdown();

   return retval;
}
