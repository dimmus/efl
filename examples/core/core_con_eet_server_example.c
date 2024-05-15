/*
 * An example Eet descriptor is defined in core_con_eet_descriptor.c.
 * This example creates an Ecore Con Eet descriptor and associates it
 * with the Ecore Con Server. The required callbacks are registered.
 * This example demonstrates how to send Eet descriptor messages between
 * client and server.
 * The server runs until all clients associated with it disconnects.
 * You can run a single server and then connect multiple clients to this
 * to check the message passing.
 */

//Compile with:
// gcc -o core_con_eet_server_example core_con_eet_server_example.c core_con_eet_descriptor_example.c `pkg-config --cflags --libs eet ecore ecore-con eina`

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>
#include <Efl_Shared.h>
#include "core_con_eet_descriptor_example.h"

static int _client_count = 0;

Efl_Bool
_eet_client_discnct_cb(void *data EFL_UNUSED, Ecore_Con_Reply *reply EFL_UNUSED, Ecore_Con_Client *conn EFL_UNUSED)
{
   _client_count--;
   if (!_client_count)
     core_main_loop_quit();

   return EFL_FALSE;
}

Efl_Bool
_eet_client_cnct_cb(void *data EFL_UNUSED, Ecore_Con_Reply *reply EFL_UNUSED, Ecore_Con_Client *conn EFL_UNUSED)
{
   _client_count++;
   return EFL_TRUE;
}

static void
_eet_data_cb(void *data EFL_UNUSED, Ecore_Con_Reply *reply, const char *protocol_name, void *value)
{
   char toSend[] = "Received your message.";// The message to send to client.
   ECE_Example *received = value;// The message received from client.

   printf("id: %d\n message: %s\n not_sending: %s\n",
           received->id, received->message, received->not_sending);
   /* The not_sending would be empty as not_sending is not added in the
    * descriptor. So that value is not sent.
    */

   /* Using eet raw send, you can send value of a particular field and
    * not the whole structure.
    */
   core_con_eet_raw_send(reply, protocol_name, "message", toSend,
                          strlen(toSend));
}

int main(int argc, char *argv[])
{
   Ecore_Con_Eet *ec_eet;
   Ecore_Con_Server *svr;

   if (argc < 3)
     {
        printf("Syntax: \n./core_con_eet_server_example <IP> <PORT>\n");
        exit(0);
     }

   eina_init();
   eet_init();
   core_con_init();
   ece_example_descriptor_init();// Initializing the example eet descriptor.

   svr = core_con_server_add(CORE_CON_REMOTE_TCP, argv[1],
                              atoi(argv[2]), NULL);
   if (!svr)
     {
        printf("Failed to bind\n");
        exit(1);
     }

   ec_eet = core_con_eet_server_new(svr);

   // Register the initialized descriptor to the server.
   ece_example_descriptor_register_descs(ec_eet);

   // Registering call backs.
   core_con_eet_data_callback_add(ec_eet, EXAMPLE_STREAM, _eet_data_cb, NULL);
   core_con_eet_client_connect_callback_add(ec_eet, _eet_client_cnct_cb,
                                             NULL);
   core_con_eet_client_disconnect_callback_add(ec_eet, _eet_client_discnct_cb,
                                                NULL);

   core_main_loop_begin();

   printf("Server was up for %0.3f seconds\n",
          core_con_server_uptime_get(svr));

   // Delete the callbacks before closing connection.
   core_con_eet_data_callback_del(ec_eet, EXAMPLE_STREAM);
   core_con_eet_client_connect_callback_del(ec_eet, _eet_client_cnct_cb,
                                             NULL);
   core_con_eet_client_disconnect_callback_del(ec_eet, _eet_client_discnct_cb,
                                                NULL);
   // Delete the eet descriptor.
   ece_example_descriptor_shutdown();

   core_con_server_del(svr);
   core_con_shutdown();
   eet_shutdown();
   eina_shutdown();

   return 0;
}
