//Compile with:
// gcc -o server server.c `pkg-config --cflags --libs eldbus ecore`

#include "Efl_Dbus.h"
#include <Efl_Core.h>

#define BUS "org.Enlightenment"
#define PATH "/org/enlightenment"
#define PATH_TEST_SON "/org/enlightenment/son"
#define INTERFACE "org.enlightenment.Test"

static Efl_Dbus_Connection *conn;

static Efl_Dbus_Message *
_hello(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *message)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(message);
   efl_dbus_message_arguments_append(reply, "s", "Hello World");
   printf("Hello\n");
   return reply;
}

static Efl_Dbus_Message *
_quit(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *message)
{
   printf("Quit\n");
   core_main_loop_quit();
   return efl_dbus_message_method_return_new(message);
}

enum
{
   TEST_SIGNAL_ALIVE = 0,
   TEST_SIGNAL_HELLO
};

static Efl_Bool
send_signal_alive(void *data)
{
   Efl_Dbus_Service_Interface *iface = data;
   efl_dbus_service_signal_emit(iface, TEST_SIGNAL_ALIVE);
   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
send_signal_hello(void *data)
{
   Efl_Dbus_Service_Interface *iface = data;
   efl_dbus_service_signal_emit(iface, TEST_SIGNAL_HELLO, "Hello World");
   return CORE_CALLBACK_RENEW;
}

static Efl_Dbus_Message *
_send_bool(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   Efl_Bool bool;
   if (!efl_dbus_message_arguments_get(msg, "b", &bool))
     printf("efl_dbus_message_arguments_get() error\n");
   efl_dbus_message_arguments_append(reply, "b", bool);
   return reply;
}

static Efl_Dbus_Message *
_send_byte(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   unsigned char byte;
   if (!efl_dbus_message_arguments_get(msg, "y", &byte))
     printf("efl_dbus_message_arguments_get() error\n");
   efl_dbus_message_arguments_append(reply, "y", byte);
   return reply;
}

static Efl_Dbus_Message *
_send_uint32(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   unsigned int uint32;
   if (!efl_dbus_message_arguments_get(msg, "u", &uint32))
     printf("efl_dbus_message_arguments_get() error\n");
   efl_dbus_message_arguments_append(reply, "u", uint32);
   return reply;
}

static Efl_Dbus_Message *
_send_int32(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   int int32;
   if (!efl_dbus_message_arguments_get(msg, "i", &int32))
     printf("efl_dbus_message_arguments_get() error\n");
   efl_dbus_message_arguments_append(reply, "i", int32);
   return reply;
}

static Efl_Dbus_Message *
_send_int16(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   short int int16;
   if (!efl_dbus_message_arguments_get(msg, "n", &int16))
     printf("efl_dbus_message_arguments_get() error\n");
   efl_dbus_message_arguments_append(reply, "n", int16);
   return reply;
}

static Efl_Dbus_Message *
_send_double(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   double d;
   if (!efl_dbus_message_arguments_get(msg, "d", &d))
     printf("efl_dbus_message_arguments_get() error\n");
   efl_dbus_message_arguments_append(reply, "d", d);
   return reply;
}

static Efl_Dbus_Message *
_send_string(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   const char *txt;
   if (!efl_dbus_message_arguments_get(msg, "s", &txt))
     printf("efl_dbus_message_arguments_get() error\n");
   efl_dbus_message_arguments_append(reply, "s", txt);
   return reply;
}

static Efl_Bool
_resp_async(void *data)
{
   Efl_Dbus_Message *msg = data;
   efl_dbus_message_arguments_append(msg, "s", "Async test ok");
   efl_dbus_connection_send(conn, msg, NULL, NULL, -1);
   return CORE_CALLBACK_CANCEL;
}

static Efl_Dbus_Message *
_async_test(const Efl_Dbus_Service_Interface *iface EFL_UNUSED, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
   printf("Received a call to AsyncTest.\n");
   printf("Response will be send in 5 seconds.\n");
   core_timer_add(5, _resp_async, reply);
   return NULL;
}

static const Efl_Dbus_Signal signals[] = {
   [TEST_SIGNAL_ALIVE] = {"Alive", NULL, 0},
   [TEST_SIGNAL_HELLO] = {"Hello", EFL_DBUS_ARGS({ "s", "message" }), 0},
   { }
};

static const Efl_Dbus_Method methods[] = {
      {
        "Hello", NULL, EFL_DBUS_ARGS({"s", "message"}),
        _hello, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      {
        "Quit", NULL, NULL,
        _quit, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "SendBool", EFL_DBUS_ARGS({"b", "bool"}), EFL_DBUS_ARGS({"b", "bool"}),
        _send_bool, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "SendByte", EFL_DBUS_ARGS({"y", "byte"}), EFL_DBUS_ARGS({"y", "byte"}),
        _send_byte, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "SendUint32", EFL_DBUS_ARGS({"u", "uint32"}), EFL_DBUS_ARGS({"u", "uint32"}),
        _send_uint32, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "SendInt32", EFL_DBUS_ARGS({"i", "int32"}), EFL_DBUS_ARGS({"i", "int32"}),
        _send_int32, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "SendInt16", EFL_DBUS_ARGS({"n", "int16"}), EFL_DBUS_ARGS({"n", "int16"}),
        _send_int16, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "SendDouble", EFL_DBUS_ARGS({"d", "double"}), EFL_DBUS_ARGS({"d", "double"}),
        _send_double, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "SendString", EFL_DBUS_ARGS({"s", "string"}), EFL_DBUS_ARGS({"s", "string"}),
        _send_string, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { "AsyncTest", NULL, EFL_DBUS_ARGS({"s", "text"}),
        _async_test, EFL_DBUS_METHOD_FLAG_DEPRECATED
      },
      { }
};

static const Efl_Dbus_Service_Interface_Desc iface_desc = {
   INTERFACE, methods, signals, NULL, NULL, NULL
};

static void
on_name_request(void *data, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   Efl_Dbus_Service_Interface *iface;
   unsigned int reply;

   iface = data;
   if (efl_dbus_message_error_get(msg, NULL, NULL))
     {
        printf("error on on_name_request\n");
        return;
     }

   if (!efl_dbus_message_arguments_get(msg, "u", &reply))
    {
       printf("error geting arguments on on_name_request\n");
       return;
    }

   if (reply != EFL_DBUS_NAME_REQUEST_REPLY_PRIMARY_OWNER)
     {
        printf("error name already in use\n");
        return;
     }

   core_timer_add(5, send_signal_alive, iface);
   core_timer_add(6, send_signal_hello, iface);
}

int
main(void)
{
   Efl_Dbus_Service_Interface *iface;

   core_init();
   efl_dbus_init();

   conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);

   iface = efl_dbus_service_interface_register(conn, PATH, &iface_desc);
   efl_dbus_name_request(conn, BUS, EFL_DBUS_NAME_REQUEST_FLAG_DO_NOT_QUEUE,
                      on_name_request, iface);

   efl_dbus_service_interface_register(conn, PATH_TEST_SON, &iface_desc);

   core_main_loop_begin();

   efl_dbus_connection_unref(conn);

   efl_dbus_shutdown();
   core_shutdown();
   return 0;
}
