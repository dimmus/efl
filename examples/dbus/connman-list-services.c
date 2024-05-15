//Compile with:
// gcc -o connman-list-services connman-list-services.c `pkg-config --cflags --libs eldbus ecore`

#include "Efl_Dbus.h"
#include <Efl_Core.h>

static void
on_services_get(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   Efl_Dbus_Message_Iter *array, *entry;
   const char *errname, *errmsg;

   if (efl_dbus_message_error_get(msg, &errname, &errmsg))
     {
        fprintf(stderr, "Error: %s %s\n", errname, errmsg);
        return;
     }

   if (!efl_dbus_message_arguments_get(msg, "a(oa{sv})", &array))
     {
        fprintf(stderr, "Error: could not get array\n");
        return;
     }

   while (efl_dbus_message_iter_get_and_next(array, 'r', &entry))
     {
        Efl_Dbus_Message_Iter *properties, *dict_entry;
        const char *path;

        if (!efl_dbus_message_iter_arguments_get(entry, "oa{sv}", &path, &properties))
          {
             fprintf(stderr, "Error: could not get entry contents\n");
             return;
          }

        printf("service: %s\n", path);

        while (efl_dbus_message_iter_get_and_next(properties, 'e', &dict_entry))
          {
             Efl_Dbus_Message_Iter *variant;
             const char *key;

             if (!efl_dbus_message_iter_arguments_get(dict_entry, "sv", &key,
                                                      &variant))
               {
                  fprintf(stderr,
                          "Error: could not get property contents\n");
                  return;
               }

             printf("\t%s: type %s\n", key,
                    efl_dbus_message_iter_signature_get(variant));

             /* TODO: get the value from variant */
          }
     }
}

int
main(void)
{
   Efl_Dbus_Connection *conn;
   Efl_Dbus_Object *obj;
   Efl_Dbus_Proxy *manager;
   Efl_Dbus_Pending *pending;

   core_init();
   efl_dbus_init();

   conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SYSTEM);
   if (!conn)
     {
        fprintf(stderr, "Error: could not get system bus\n");
        return EXIT_FAILURE;
     }

   obj = efl_dbus_object_get(conn, "net.connman", "/");
   if (!obj)
     {
        fprintf(stderr, "Error: could not get object\n");
        return EXIT_FAILURE;
     }

   manager = efl_dbus_proxy_get(obj, "net.connman.Manager");
   if (!manager)
     {
        fprintf(stderr, "Error: could not get proxy\n");
        return EXIT_FAILURE;
     }

   pending = efl_dbus_proxy_call(manager, "GetServices", on_services_get, NULL,
                              -1, "");

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

