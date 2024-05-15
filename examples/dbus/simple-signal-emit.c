//Compile with:
// gcc -o simple-signal-emit simple-signal-emit.c `pkg-config --cflags --libs eldbus`

#include <Efl_Dbus.h>

enum {
   TEST_SIGNAL_ALIVE,
   TEST_SIGNAL_PROP,
   TEST_SIGNAL_NAME,
};

static const Efl_Dbus_Signal test_signals[] = {
   [TEST_SIGNAL_ALIVE] = { "Alive" },
   [TEST_SIGNAL_PROP] = { "Properties", EFL_DBUS_ARGS({ "a{ss}", "properties"}) },
   [TEST_SIGNAL_NAME] = { "Name", EFL_DBUS_ARGS({ "s", "name"}) },
   { }
};

/* signal with complex arguments (a dict) */
static void emit_properties(Efl_Dbus_Service_Interface *iface)
{
   Efl_Dbus_Message *alive2;
   Efl_Dbus_Message_Iter *iter, *dict;
   struct keyval {
      const char *key;
      const char *val;
   } keyval[] = {
      { "key1", "val1" },
      { "key2", "val2" },
      { }
   };
   struct keyval *k;

   alive2 = efl_dbus_service_signal_new(iface, TEST_SIGNAL_PROP);
   iter = efl_dbus_message_iter_get(alive2);
   dict = efl_dbus_message_iter_container_new(iter, 'a', "{ss}");

   for (k = keyval; k && k->key; k++)
     {
        Efl_Dbus_Message_Iter *entry = efl_dbus_message_iter_container_new(dict, 'e',
                                                                     NULL);
        efl_dbus_message_iter_arguments_append(entry, "ss", k->key, k->val);
        efl_dbus_message_iter_container_close(dict, entry);
     }

   efl_dbus_message_iter_container_close(iter, dict);
   efl_dbus_service_signal_send(iface, alive2);
}

/* signal with basic args */
static void emit_name(Efl_Dbus_Service_Interface *iface)
{
   efl_dbus_service_signal_emit(iface, TEST_SIGNAL_NAME, "TEST");
}

/* simple signal example */
static void emit_alive(Efl_Dbus_Service_Interface *iface)
{
   efl_dbus_service_signal_emit(iface, TEST_SIGNAL_ALIVE);
}

static const Efl_Dbus_Service_Interface_Desc iface_desc = {
   "org.enlightenment.Test", NULL, test_signals, NULL, NULL, NULL
};

int main(void)
{
   Efl_Dbus_Connection *conn;
   Efl_Dbus_Service_Interface *iface;

   efl_dbus_init();

   conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   iface = efl_dbus_service_interface_register(conn, "/org/enlightenment",
                                            &iface_desc);

   emit_alive(iface);
   emit_name(iface);
   emit_properties(iface);

   efl_dbus_connection_unref(conn);

   efl_dbus_shutdown();

   return 0;
}
