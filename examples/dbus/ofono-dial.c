//Compile with:
// gcc -o ofono-dial ofono-dial.c `pkg-config --cflags --libs eldbus ecore`

#include "Efl_Dbus.h"
#include <Efl_Core.h>

static void
on_dial(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   const char *errname, *errmsg;
   const char *call_path;

   if (efl_dbus_message_error_get(msg, &errname, &errmsg))
     {
        fprintf(stderr, "Error: %s %s\n", errname, errmsg);
        return;
     }

   if (!efl_dbus_message_arguments_get(msg, "o", &call_path))
     {
        fprintf(stderr, "Error: could not get call path\n");
        return;
     }

   printf("dialed! call path: %s\n", call_path);
}

int
main(int argc, char *argv[])
{
   Efl_Dbus_Connection *conn;
   Efl_Dbus_Object *obj;
   Efl_Dbus_Proxy *manager;
   Efl_Dbus_Pending *pending;
   const char *number, *hide_callerid;

   if (argc < 2)
     {
        fprintf(stderr, "Usage:\n\t%s <number> [hide_callerid]\n", argv[0]);
        return EXIT_FAILURE;
     }

   number = argv[1];
   hide_callerid = (argc > 2) ? argv[2] : "";

   core_init();
   efl_dbus_init();

   conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SYSTEM);
   if (!conn)
     {
        fprintf(stderr, "Error: could not get system bus\n");
        return EXIT_FAILURE;
     }

   obj = efl_dbus_object_get(conn, "org.ofono", "/");
   if (!obj)
     {
        fprintf(stderr, "Error: could not get object\n");
        return EXIT_FAILURE;
     }

   manager = efl_dbus_proxy_get(obj, "org.ofono.Manager");
   if (!manager)
     {
        fprintf(stderr, "Error: could not get proxy\n");
        return EXIT_FAILURE;
     }

   pending = efl_dbus_proxy_call(manager, "Dial", on_dial, NULL,
                              -1, "ss", number, hide_callerid);
   if (!pending)
     {
        fprintf(stderr, "Error: could not call\n");
        return EXIT_FAILURE;
     }

   core_main_loop_begin();

   efl_dbus_proxy_unref(manager);
   efl_dbus_object_unref(obj);
   efl_dbus_connection_unref(conn);

   efl_dbus_shutdown();
   core_shutdown();
   return 0;
}

