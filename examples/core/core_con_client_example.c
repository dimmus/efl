//Compile with:
// gcc -o core_con_client_example core_con_client_example.c `pkg-config --libs --cflags ecore ecore-con eina`

#ifdef HAVE_CONFIG_H
#include "efl_config.h"
#endif

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>
#include <Efl_Core_Getopt.h>

/* Ecore_Con client example
 * 2010 Mike Blumenkrantz
 */

static Ecore_Con_Server *svr;
static int retval = EXIT_SUCCESS;
static Efl_Bool do_flush = EFL_FALSE;
static Efl_Bool single_message = EFL_FALSE;
static Efl_Bool verify = EFL_TRUE;
static Efl_Bool hostname_verify = EFL_TRUE;
static Efl_Bool do_ssl_upgrade = EFL_FALSE;
static char *starttls_local_command = NULL;

static Efl_Bool
_setup_ssl(void)
{
   Eina_Iterator *it;
   const char *ca;

   if (!(it = eina_file_ls("/etc/ssl/certs")))
     {
        retval = EXIT_FAILURE;
        return EFL_FALSE;
     }

   /* add all the CAs */
   EINA_ITERATOR_FOREACH(it, ca)
     {
        if (!core_con_ssl_server_cafile_add(svr, ca))
          fprintf(stderr, "WARNING: could not load CA: %s!\n", ca);
        eina_stringshare_del(ca);
     }

   eina_iterator_free(it);

   if (verify)
     core_con_ssl_server_verify(svr);

   if (hostname_verify)
     core_con_ssl_server_verify_basic(svr);

   return EFL_TRUE;
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

   if (!svr)
     fputs("WARNING: not connected to server, ignored input.\n", stderr);
   else
     {
        size_t clen = strlen(starttls_local_command);
        if (do_ssl_upgrade && ((size_t)r > clen) &&
            (strncmp(line, starttls_local_command, clen) == 0) &&
            (line[clen] == '\n' || line[clen] == '\r'))
          {
             printf("INFO: starting SSL communication...\n");

             if (!core_con_ssl_server_upgrade(svr, CORE_CON_USE_MIXED | CORE_CON_LOAD_CERT))
               {
                  printf("ERROR: failed to upgrade to SSL!\n");
                  retval = EXIT_FAILURE;
                  core_con_server_del(svr);
                  core_main_loop_quit();
               }

#if 1
             /* This just works since EFL v 1.19.  Prior to this,
              * upgrade couldn't get any extra setup, such as
              * certificate or verification mode as OpenSSL would
              * complain.
              */
             else if (!_setup_ssl())
               {
                  retval = EXIT_FAILURE;
                  core_con_server_del(svr);
                  core_main_loop_quit();
               }
#endif

             svr = NULL; /* it's considered dead until CORE_CON_EVENT_SERVER_UPGRADE */
             goto end;
          }
        core_con_server_send(svr, line, r);
        printf("INFO: sent %zd bytes to server.\n", r);
        if (do_flush) core_con_server_flush(svr);
        if (single_message)
          {
             core_con_server_del(svr);
             svr = NULL;
             core_main_loop_quit();
          }
     }

 end:
   free(line);

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_add(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Add *ev)
{
   printf("Server with ip %s connected!\n", core_con_server_ip_get(ev->server));
   if (do_ssl_upgrade)
     printf("INFO: Not sending 'hello!' in tcp+ssl mode. Use: %s to upgrade the connection\n", starttls_local_command);
   else
     core_con_server_send(ev->server, "hello!", strlen("hello!"));
   if (do_flush) core_con_server_flush(ev->server);

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_del(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Del *ev)
{
   printf("Lost server with ip %s!\n", core_con_server_ip_get(ev->server));
   core_con_server_del(svr);
   svr = NULL;
   core_main_loop_quit();
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_data(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Data *ev)
{
   printf("Received %i bytes from server:\n"
          ">>>>>\n"
          "%.*s\n"
          ">>>>>\n",
          ev->size,
          ev->size, (const char *)ev->data);

   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_write_(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Write *ev)
{
   printf("Sent %d bytes to server\n", ev->size);
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_error(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Error *ev)
{
   printf("Server Error: %s\n", ev->error);
   return CORE_CALLBACK_RENEW;
}

Efl_Bool
_upgrade(void *data EFL_UNUSED, int type EFL_UNUSED, Ecore_Con_Event_Server_Upgrade *ev)
{
   printf("Server upgraded to SSL %p %s\n", ev->server, core_con_server_ip_get(ev->server));
   svr = ev->server;
   return CORE_CALLBACK_RENEW;
}

static const char *types_strs[] = {
  "tcp",
  "udp",
  "ssl",
  "tcp+ssl",
  "local-user",
  "local-system",
  NULL
};

static const Core_Getopt options = {
  "core_con_client_example", /* program name */
  NULL, /* usage line */
  "1", /* version */
  "(C) 2016 Enlightenment Project; 2010 Mike Blumenkrantz", /* copyright */
  "BSD 2-Clause", /* license */
  /* long description, may be multiline and contain \n */
  "Example of core_con_server_connect()\n",
  EFL_FALSE,
  {
    CORE_GETOPT_CHOICE('t', "type", "Server type to use, defaults to 'tcp'", types_strs),
    CORE_GETOPT_STORE_TRUE('P', "no-proxy", "Do not use SOCKS proxy for remote connections."),

    CORE_GETOPT_STORE_TRUE('f', "flush", "Force a flush after every send call."),
    CORE_GETOPT_STORE_TRUE('m', "single-message", "Send a single message and delete the server."),

    CORE_GETOPT_STORE_STR('c', "starttls-local-command", "The string to use as a local command (it's NOT sent to remote peer) to upgrade connections when -t/--type=tcp+ssl. Defaults to STARTTLS, however if you need to send that to the server, change the string with this option."),

    CORE_GETOPT_STORE_FALSE(0, "no-verify", "Do not verify server's certificate"),
    CORE_GETOPT_STORE_FALSE(0, "no-hostname-verify", "Do not Verify server's hostname based on its certificate."),

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
main(int argc, char *argv[])
{
   char *name = NULL;
   char *type_choice = NULL;
   Ecore_Con_Type type;
   int port = -1;
   Efl_Bool no_proxy = EFL_FALSE;
   Efl_Bool quit_option = EFL_FALSE;
   Core_Getopt_Value values[] = {
     CORE_GETOPT_VALUE_STR(type_choice),
     CORE_GETOPT_VALUE_BOOL(no_proxy),

     CORE_GETOPT_VALUE_BOOL(do_flush),
     CORE_GETOPT_VALUE_BOOL(single_message),

     CORE_GETOPT_VALUE_STR(starttls_local_command),

     CORE_GETOPT_VALUE_BOOL(verify),
     CORE_GETOPT_VALUE_BOOL(hostname_verify),

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
   else
     {
        fprintf(stderr, "ERROR: unsupported --type/-t '%s'\n", type_choice);
        retval = EXIT_FAILURE;
        goto end;
     }

   if ((!starttls_local_command) || (starttls_local_command[0] == '\0'))
     starttls_local_command = "STARTTLS";

   if (no_proxy) type |= CORE_CON_NO_PROXY;

   svr = core_con_server_connect(type, name, port, NULL);
   if (!svr) goto end;

   if (strcmp(type_choice, "ssl") == 0)
     {
        if (!_setup_ssl())
          goto no_mainloop;
     }

/* set event handler for server connect */
   core_event_handler_add(CORE_CON_EVENT_SERVER_ADD, (Core_Event_Handler_Cb)_add, NULL);
/* set event handler for server disconnect */
   core_event_handler_add(CORE_CON_EVENT_SERVER_DEL, (Core_Event_Handler_Cb)_del, NULL);
/* set event handler for receiving server data */
   core_event_handler_add(CORE_CON_EVENT_SERVER_DATA, (Core_Event_Handler_Cb)_data, NULL);
/* set event handler that notifies of sent data */
   core_event_handler_add(CORE_CON_EVENT_SERVER_WRITE, (Core_Event_Handler_Cb)_write_, NULL);
/* set event handler that notifies of errors */
   core_event_handler_add(CORE_CON_EVENT_SERVER_ERROR, (Core_Event_Handler_Cb)_error, NULL);
/* set event handler that notifies of upgrades */
   core_event_handler_add(CORE_CON_EVENT_SERVER_UPGRADE, (Core_Event_Handler_Cb)_upgrade, NULL);

   core_main_fd_handler_add(STDIN_FILENO, CORE_FD_READ, _on_stdin, NULL, NULL, NULL);

/* start client */
   core_main_loop_begin();

 no_mainloop:
   if (svr)
     {
        core_con_server_del(svr);
        svr = NULL;
     }

end:
   core_con_shutdown();
   core_shutdown();
   eina_shutdown();

   return retval;
}
