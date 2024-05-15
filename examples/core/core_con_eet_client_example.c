/*
 * This example shows how to register a eet descriptor to a ecore con client
 * and send message to ecore con server. The eet descriptor is defined in
 * core_con_eet_descriptor_example.c.
 */

//Compile with:
// gcc -o core_con_eet_client_example core_con_eet_client_example.c core_con_eet_descriptor_example.c `pkg-config --cflags --libs eet ecore ecore-con eina`

#include <stdio.h>
#include <Efl_Core.h>
#include <Ecore_Con.h>
#include <Efl_Shared.h>
#include "core_con_eet_descriptor_example.h"

Efl_Bool
_eet_svr_discnct_cb(void *data EFL_UNUSED, Ecore_Con_Reply *reply EFL_UNUSED, Ecore_Con_Server *conn EFL_UNUSED)
{
   core_main_loop_quit();
   return EFL_FALSE;
}

static Efl_Bool
_eet_svr_cnct_cb(void *data EFL_UNUSED, Ecore_Con_Reply *reply, Ecore_Con_Server *conn EFL_UNUSED)
{
   /* Only id and message parameter are sent to server. not_sending, parameter
    * is not sent, as it is not added to the eet descriptor. */
   ECE_Example toSend = {1, "Message from Client.",
                         "This is not sent to server"};

   core_con_eet_send(reply, EXAMPLE_STREAM, &toSend);
   return EFL_TRUE;
}

static void
_eet_raw_data_cb(void *data EFL_UNUSED, Ecore_Con_Reply *reply EFL_UNUSED, const char *protocol_name EFL_UNUSED, const char *section, void *value, size_t length EFL_UNUSED)
{
   /* Only a perticular parameter's value is sent by Server. */
   printf("Section: %s\n", section);
   printf("Value: %s\n", (char *) value);
}

int main(int argc, const char *argv[])
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
   ece_example_descriptor_init(); // Initialize a eet descriptor.

   svr = core_con_server_connect(CORE_CON_REMOTE_TCP, argv[1],
                                  atoi(argv[2]), NULL);
   if (!svr)
     {
        printf("could not connect to the server.\n");
        exit(1);
     }

   ec_eet = core_con_eet_client_new(svr); // Associate it with the connection.

   // Register the descriptor to send messages.
   ece_example_descriptor_register_descs(ec_eet);

   // Register call backs.
   core_con_eet_raw_data_callback_add(ec_eet, EXAMPLE_STREAM,
                                       _eet_raw_data_cb, NULL);
   core_con_eet_server_connect_callback_add(ec_eet, _eet_svr_cnct_cb, NULL);
   core_con_eet_server_disconnect_callback_add(ec_eet, _eet_svr_discnct_cb,
                                                NULL);

   core_main_loop_begin();

   // Delete all callbacks before closing connection.
   core_con_eet_server_disconnect_callback_del(ec_eet, _eet_svr_discnct_cb,
                                                NULL);
   core_con_eet_server_connect_callback_del(ec_eet, _eet_svr_discnct_cb,
                                             NULL);
   core_con_eet_raw_data_callback_del(ec_eet, EXAMPLE_STREAM);

   // Free the descriptor.
   ece_example_descriptor_shutdown();

   core_con_server_del(svr);
   core_con_shutdown();
   eet_shutdown();
   eina_shutdown();

   return 0;
}
