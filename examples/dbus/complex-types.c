//Compile with:
// gcc -o complex-types complex-types.c `pkg-config --cflags --libs eldbus ecore`

#include "Efl_Dbus.h"
#include <Efl_Core.h>

#define BUS "com.profusion"
#define PATH "/com/profusion/Test"
#define IFACE "com.profusion.Test"

Efl_Dbus_Connection *conn;

static Efl_Bool
_timer1_cb(void *data EFL_UNUSED)
{
   printf("\nFishing...\n");
   core_main_loop_quit();
   return CORE_CALLBACK_CANCEL;
}

static void
on_plus_one(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   int num2 = 0;

   if (efl_dbus_message_error_get(msg, NULL, NULL))
     {
        printf("Message error\n\n");
        return;
     }
   if (!efl_dbus_message_arguments_get(msg, "i", &num2))
     {
        printf("Error getting arguments.");
        return;
     }

   printf("on_plus_one() %d\n", num2);
}

static void
set_property_resp2(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   const char *errname;
   const char *errmsg;

   printf("set_property_resp2()\n");
   if (efl_dbus_message_error_get(msg, &errname, &errmsg))
     {
        printf("Message error %s - %s\n\n", errname, errmsg);
        return;
     }
}

static void
get_property_resp2(void *data, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   Efl_Dbus_Proxy *proxy = data;
   Efl_Dbus_Message_Iter *variant = NULL;
   char *type;
   char *resp2;
   const char *errname;
   const char *errmsg;

   printf("get_property_resp2()\n");
   if (efl_dbus_message_error_get(msg, &errname, &errmsg))
     {
        printf("Message error %s - %s\n\n", errname, errmsg);
        return;
     }
   if (!efl_dbus_message_arguments_get(msg, "v", &variant))
     {
        printf("Error getting arguments.");
        return;
     }

   type = efl_dbus_message_iter_signature_get(variant);
   if (type[1])
     {
        printf("It is a complex type, not handle yet.\n\n");
        return;
     }
   if (type[0] != 's')
     {
        printf("Expected type is string.\n\n");
        return;
     }
   if (!efl_dbus_message_iter_arguments_get(variant, "s", &resp2))
     {
        printf("error in efl_dbus_message_iter_arguments_get()\n\n");
        return;
     }
   printf("resp2=%s\n", resp2);
   free(type);

   efl_dbus_proxy_property_set(proxy, "Resp2", "s", &"lalala", set_property_resp2, NULL);
   efl_dbus_proxy_property_set(proxy, "int32", "i",  (void*)(intptr_t)99, set_property_resp2, NULL);
}

static void
on_send_array_int(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   Efl_Dbus_Message_Iter *array = NULL;
   int num;

   printf("on_send_array_int()\n");
   if (efl_dbus_message_error_get(msg, NULL, NULL))
     {
        printf("Message error\n\n");
        return;
     }
   if (!efl_dbus_message_arguments_get(msg, "ai", &array))
     {
        printf("Error getting arguments.");
        return;
     }

   while (efl_dbus_message_iter_get_and_next(array, 'i', &num))
     {
        printf("%d\n", num);
     }
}

static void
on_send_array(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   Efl_Dbus_Message_Iter *array = NULL;
   char *txt = NULL;
   char *string[10];
   int i = 0;
   int z;

   printf("on_send_array()\n");
   if (efl_dbus_message_error_get(msg, NULL, NULL))
     {
        printf("Message error\n\n");
        return;
     }
   if (!efl_dbus_message_arguments_get(msg, "as", &array))
     {
        printf("Error getting arguments.");
        return;
     }

   while (efl_dbus_message_iter_get_and_next(array, 's', &txt))
     {
        string[i] = txt;
        i++;
     }

   for (z = 0; z < i; z++)
     printf("string = %s\n", string[z]);
}

static void
on_receive_array_with_size(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   const char *errname;
   const char *errmsg;

   printf("on_receive_array_with_size()\n");
   if (efl_dbus_message_error_get(msg, &errname, &errmsg))
     {
        fprintf(stderr, "Error: %s %s\n", errname, errmsg);
     }
}

static void
on_send_variant(void *data EFL_UNUSED, const Efl_Dbus_Message *msg EFL_UNUSED, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   printf("on_send_variant()\n\n");
}

static void
on_receive_array(void *data EFL_UNUSED, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EFL_UNUSED)
{
   const char *errname;
   const char *errmsg;

   printf("on_receive_array()\n");
   if (efl_dbus_message_error_get(msg, &errname, &errmsg))
     {
        fprintf(stderr, "Error: %s %s\n", errname, errmsg);
     }
}

int
main(void)
{
   Efl_Dbus_Object *test2_obj;
   Efl_Dbus_Proxy *test2_proxy;
   Efl_Dbus_Pending *pending;
   Efl_Dbus_Message_Iter *iter, *array_of_string, *variant;
   Efl_Dbus_Message_Iter *array_itr, *structure;
   Efl_Dbus_Message *msg;
   int size_of_array = 5;
   const char *array[5] = { "aaaa", "bbbb", "cccc", "dddd", "eeee" };
   int i;
   int plus_one = 24;

   core_init();
   efl_dbus_init();

   conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);

   test2_obj = efl_dbus_object_get(conn, BUS, PATH);
   test2_proxy = efl_dbus_proxy_get(test2_obj, IFACE);

   msg = efl_dbus_proxy_method_call_new(test2_proxy, "ReceiveArray");
   iter = efl_dbus_message_iter_get(msg);
   array_of_string = efl_dbus_message_iter_container_new(iter, 'a',"s");
   if (!array_of_string) printf("array_of_string == NULL\n\n");
   for (i = 0; i < 5; i++)
     efl_dbus_message_iter_basic_append(array_of_string, 's', array[i]);
   efl_dbus_message_iter_container_close(iter, array_of_string);
   pending = efl_dbus_proxy_send(test2_proxy, msg, on_receive_array, NULL, -1);
   if (!pending) printf("Error in efl_dbus_proxy_send()\n\n");

   msg = efl_dbus_proxy_method_call_new(test2_proxy, "ReceiveArrayOfStringIntWithSize");
   iter = efl_dbus_message_iter_get(msg);
   if (!efl_dbus_message_iter_arguments_append(iter, "ia(si)", size_of_array, &array_of_string))
     printf("error on efl_dbus_massage_iterator_arguments_set()\n\n");
   for (i = 0; i < size_of_array; i++)
     {
         Efl_Dbus_Message_Iter *struct_of_si;
         efl_dbus_message_iter_arguments_append(array_of_string, "(si)", &struct_of_si);
         efl_dbus_message_iter_arguments_append(struct_of_si, "si", array[i], i);
         efl_dbus_message_iter_container_close(array_of_string, struct_of_si);
     }
   efl_dbus_message_iter_container_close(iter, array_of_string);
   pending = efl_dbus_proxy_send(test2_proxy, msg, on_receive_array_with_size, NULL, -1);

   msg = efl_dbus_proxy_method_call_new(test2_proxy, "SendVariantData");
   iter = efl_dbus_message_iter_get(msg);
   variant = efl_dbus_message_iter_container_new(iter, 'v', "s");
   efl_dbus_message_iter_basic_append(variant, 's', "test");
   efl_dbus_message_iter_container_close(iter, variant);
   pending = efl_dbus_proxy_send(test2_proxy, msg, on_send_variant, NULL, -1);

   msg = efl_dbus_proxy_method_call_new(test2_proxy, "DoubleContainner");
   iter = efl_dbus_message_iter_get(msg);
   /**
    * efl_dbus_message_iterator_arguments_set(itr, "a(ii)a(ii)", &array_itr, &array_itr2);
    * this will cause a error, we could not open another container until
    * we close the first one
    */
   efl_dbus_message_iter_arguments_append(iter, "a(ii)", &array_itr);
   for (i = 0; i < 5; i++)
     {
        efl_dbus_message_iter_arguments_append(array_itr, "(ii)", &structure);
        efl_dbus_message_iter_arguments_append(structure, "ii", i, i*i);
        efl_dbus_message_iter_container_close(array_itr, structure);
     }
   efl_dbus_message_iter_container_close(iter, array_itr);
   efl_dbus_message_iter_arguments_append(iter, "a(ii)", &array_itr);
   for (i = 0; i < 7; i++)
     {
        efl_dbus_message_iter_arguments_append(array_itr, "(ii)", &structure);
        efl_dbus_message_iter_arguments_append(structure, "ii", i, i*i*i);
        efl_dbus_message_iter_container_close(array_itr, structure);
     }
   efl_dbus_message_iter_container_close(iter, array_itr);
   efl_dbus_proxy_send(test2_proxy, msg, NULL, NULL, -1);

   pending = efl_dbus_proxy_call(test2_proxy, "SendArrayInt", on_send_array_int, NULL,
                                 -1 , "");

   pending = efl_dbus_proxy_call(test2_proxy, "SendArray", on_send_array, NULL,
                              -1 , "");

   pending = efl_dbus_proxy_call(test2_proxy, "PlusOne", on_plus_one, NULL,
                              -1 , "i", plus_one);

   pending = efl_dbus_proxy_property_get(test2_proxy, "Resp2", get_property_resp2, test2_proxy);

   core_timer_add(10, _timer1_cb, NULL);

   core_main_loop_begin();

   efl_dbus_connection_unref(conn);

   efl_dbus_shutdown();
   core_shutdown();
   return 0;
}
