#include <check.h>
#include <Efl_Dbus.h>
#include <Efl_Core.h>
#include <string.h>
#include <Efl_Shared.h>

#include "efl_dbus_suite.h"

static Eina_Bool is_success = EINA_FALSE;

static Ecore_Timer *timeout = NULL;

static const char *empty_string = "";

static const char *bus = "org.freedesktop.DBus";
static const char *interface = "org.freedesktop.DBus";
static const char *path = "/org/freedesktop/DBus";
static const char *method_name = "GetId";

static int cb_data = 5;

#define DATA_KEY "proxy_data"

static int proxy_data_stored = 5;

/**
* @addtogroup eldbus
* @{
* @defgroup efl_dbus_proxy
*
* @preconditio
* @step 1 Initialize eldbus with efl_dbus_init()
*/

static Eina_Bool
_ecore_loop_close(void *data EINA_UNUSED)
{
   ecore_main_loop_quit();

   return ECORE_CALLBACK_CANCEL;
}

static void
_proxy_message_cb(void *data, const Efl_Dbus_Message *msg, Efl_Dbus_Pending *pending EINA_UNUSED)
{
   if (timeout != NULL)
     {
        ecore_timer_del(timeout);
        timeout = NULL;
     }

   const char *errname, *errmsg;
   int *user_data = data;

   if ((user_data) && (*user_data == cb_data))
     {
        if (!efl_dbus_message_error_get(msg, &errname, &errmsg))
          {
             char *txt = NULL;
             if (efl_dbus_message_arguments_get(msg, "s", &txt))
               {
                  if ((txt) && strcmp(txt, empty_string))
                    is_success = EINA_TRUE;
               }
          }
     }

   ecore_main_loop_quit();
}

/**
 * @addtogroup efl_dbus_proxy
 * @{
 * @defgroup efl_dbus_proxy_info_get_call
 * @li efl_dbus_proxy_get()
 * @li efl_dbus_proxy_interface_get()
 * @li efl_dbus_proxy_object_get()
 * @li efl_dbus_proxy_call()
 * @li efl_dbus_proxy_ref()
 * @li efl_dbus_proxy_unref()
 * @{
 * @objective Positive test case checks if function get a proxy of the
 * following interface name in a Efl_Dbus_Object, get the interface name associated with a proxy object.
 * Get the DBus object associated with a proxy object. Call the method and get valid response in callback function.
 * Without segmentation fault.
 *
 * @n Input Data:
 * @li the conn object connection with bus
 * @li the obj Efl_Dbus_Object object of the given bus and path.
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get connection object.
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_object_get function to get an object of the given bus and path.
 * @step 4 Check returned object on NULL.
 * @step 5 Call efl_dbus_proxy_get function to get a proxy of the following interface name.
 * @step 6 Check returned proxy object on NULL.
 * @step 7 Call efl_dbus_proxy_object_get function to get the dbus object associated with a proxy object.
 * @step 8 Check if returned object is equal to obj Efl_Dbus_Object.
 * @step 9 Call efl_dbus_proxy_interface_get function to get valid string and check on expected string.
 * @step 10 Call efl_dbus_proxy_ref function to increase proxy reference.
 * @step 11 Check two objects on the same adress.
 * @step 12 Call efl_dbus_object_unref function to decrease proxy reference.
 * @step 13 Call efl_dbus_proxy_interface_get function to check the proxy object is still correct.
 * @step 14 Call efl_dbus_proxy_call function to call a method in proxy.
 * @step 15 Check returned pending object on NULL.
 * @step 16 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 17 Start of main loop and wait for tested response in callback executing.
 * @step 18 Check static variable named is_success.
 * If is equal EINA_TRUE, that callback was executed and method send valid message response.
 * @step 19 Call efl_dbus_proxy_unref function to delete proxy object
 * @step 20 Call efl_dbus_object_unref function to delete connection dbus object
 * @step 21 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Function should get valid expected proxy information about connection dbus object.
 * Variables named is_success must equals EINA_TRUE. Without segmentation fault.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_proxy_info_get_call_p)
{
   is_success = EINA_FALSE;

   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Object *obj = efl_dbus_object_get(conn, bus, path);
   ck_assert_ptr_ne(NULL, obj);

   Efl_Dbus_Proxy *proxy = efl_dbus_proxy_get(obj, interface);
   ck_assert_ptr_ne(NULL, proxy);

   Efl_Dbus_Object *proxy_obj = efl_dbus_proxy_object_get(proxy);
   ck_assert_ptr_eq(proxy_obj, obj);

   ck_assert_str_eq(interface, efl_dbus_proxy_interface_get(proxy)); 

   Efl_Dbus_Proxy *proxy_ref = efl_dbus_proxy_ref(proxy);
   ck_assert_ptr_eq(proxy_ref, proxy);

   efl_dbus_proxy_unref(proxy_ref);

   ck_assert(efl_dbus_proxy_interface_get(proxy) != NULL);

   Efl_Dbus_Pending *pending = efl_dbus_proxy_call(proxy, method_name, _proxy_message_cb, &cb_data, -1, empty_string);
   ck_assert_ptr_ne(NULL, pending);

   timeout = ecore_timer_add(0.1, _ecore_loop_close, NULL);
   ck_assert_ptr_ne(NULL, timeout);

   ecore_main_loop_begin();

   ck_assert_msg(is_success, "Method GetId is not call");

   efl_dbus_proxy_unref(proxy);
   efl_dbus_object_unref(obj);
   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 * @addtogroup efl_dbus_proxy
 * @{
 * @defgroup efl_dbus_proxy_send_call
 * @li efl_dbus_proxy_send()
 * @li efl_dbus_proxy_method_call_new()
 * @{
 * @objective Positive test case checks if function call the method
 * on the proxy object and get valid response in callback function. Without segmentation fault.
 *
 * @n Input Data:
 * @li the conn object connection with bus
 * @li the obj Efl_Dbus_Object object of the given bus and path.
 * @li the proxy object associated with dbus object.
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get connection object.
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_object_get function to get an object of the given bus and path.
 * @step 4 Check returned object on NULL.
 * @step 5 Call efl_dbus_proxy_get function to get a proxy of the following interface name.
 * @step 6 Check returned proxy object on NULL.
 * @step 7 Call efl_dbus_proxy_method_call_new function to constructs a new message
 * to invoke a method on a remote interface.
 * @step 8 Check returned message object on NULL.
 * @step 9 Call efl_dbus_proxy_send function to send a message.
 * @step 10 Check returned pending object on NULL.
 * @step 11 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 12 Start of main loop and wait for tested response in callback executing.
 * @step 13 Check static variable named is_success.
 * If is equal EINA_TRUE, that callback was executed and method send valid message response.
 * @step 14 Call efl_dbus_message_unref function to delete message object
 * @step 15 Call efl_dbus_proxy_unref function to delete proxy object
 * @step 16 Call efl_dbus_object_unref function to delete connection dbus object
 * @step 17 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Variables named is_success must equals EINA_TRUE. Without segmentation fault.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_proxy_send_call_p)
{
   is_success = EINA_FALSE;

   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Object *obj = efl_dbus_object_get(conn, bus, path);
   ck_assert_ptr_ne(NULL, obj);

   Efl_Dbus_Proxy *proxy = efl_dbus_proxy_get(obj, interface);
   ck_assert_ptr_ne(NULL, proxy);

   Efl_Dbus_Message *msg = efl_dbus_proxy_method_call_new(proxy, method_name);
   ck_assert_ptr_ne(NULL, msg);

   Efl_Dbus_Pending *pending = efl_dbus_proxy_send(proxy, msg, _proxy_message_cb, &cb_data, -1);
   ck_assert_ptr_ne(NULL, pending);

   timeout = ecore_timer_add(0.1, _ecore_loop_close, NULL);
   ck_assert_ptr_ne(NULL, timeout);

   ecore_main_loop_begin();

   ck_assert_msg(is_success, "Method GetId is not call");

   efl_dbus_proxy_unref(proxy);
   efl_dbus_object_unref(obj);
   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 * @addtogroup efl_dbus_proxy
 * @{
 * @defgroup efl_dbus_proxy_send_and_block efl_dbus_proxy_send_and_block()
 * @{
 * @objective Positive test case checks if function send a message and block while waiting for the reply
 * and get valid response in return message object. Without segmentation fault.
 *
 * @n Input Data:
 * @li the conn object connection with bus
 * @li the obj Efl_Dbus_Object object of the given bus and path.
 * @li the proxy object associated with dbus object.
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get connection object.
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_object_get function to get an object of the given bus and path.
 * @step 4 Check returned object on NULL.
 * @step 5 Call efl_dbus_proxy_get function to get a proxy of the following interface name.
 * @step 6 Check returned proxy object on NULL.
 * @step 7 Call efl_dbus_proxy_method_call_new function to constructs a new message
 * to invoke a method on a remote interface.
 * @step 8 Check returned message object on NULL.
 * @step 9 Call efl_dbus_proxy_send_and_block function to send a message
 * and block while waiting for the reply.
 * @step 10 Check returned reply message object on expected value.
 * @step 11 Call efl_dbus_message_unref function to delete reply message object
 * @step 12 Call efl_dbus_message_unref function to delete message object
 * @step 13 Call efl_dbus_proxy_unref function to delete proxy object
 * @step 14 Call efl_dbus_object_unref function to delete connection dbus object
 * @step 15 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Function should send a message and block while waiting
 * for returned expected reply message object. Without segmentation fault.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_proxy_send_and_block_p)
{
   const int timeout = 1000;
   is_success = EINA_FALSE;
   const char *errname, *errmsg;
   char *text_reply = NULL;

   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Object *obj = efl_dbus_object_get(conn, bus, path);
   ck_assert_ptr_ne(NULL, obj);

   Efl_Dbus_Proxy *proxy = efl_dbus_proxy_get(obj, interface);
   ck_assert_ptr_ne(NULL, proxy);

   Efl_Dbus_Message *msg = efl_dbus_proxy_method_call_new(proxy, method_name);
   ck_assert_ptr_ne(NULL, msg);

   Efl_Dbus_Message *message_reply = efl_dbus_proxy_send_and_block(proxy, msg, timeout);
   ck_assert_ptr_ne(NULL, message_reply);

   //efl_dbus_message_error_get(message_reply, &errname, &errmsg)
   ck_assert(efl_dbus_message_error_get(message_reply, &errname, &errmsg) == EINA_FALSE);

   ck_assert(efl_dbus_message_arguments_get(message_reply, "s", &text_reply) == EINA_TRUE);

   if (text_reply)
     printf("is reply message is not null\n");
   else
     printf("is reply message is null\n");

   printf("message %s\n", text_reply);

   ck_assert_ptr_ne(NULL, text_reply);

   ck_assert_str_ne(text_reply, empty_string);

   efl_dbus_message_unref(message_reply);
   efl_dbus_proxy_unref(proxy);
   efl_dbus_object_unref(obj);
   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 * @addtogroup efl_dbus_proxy
 * @{
 * @defgroup efl_dbus_proxy_data
 * @li efl_dbus_proxy_data_set()
 * @li efl_dbus_proxy_data_get()
 * @li efl_dbus_proxy_data_del()
 * @{
 * @objective Positive test case checks if function correctly set, get and delete the data stored
 * in proxy object and without segmentation fault.
 *
 * @n Input Data:
 * @li the conn object connection with bus
 * @li the obj Efl_Dbus_Object object of the given bus and path.
 * @li the proxy object associated with dbus object.
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get connection object.
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_object_get function to get an object of the given bus and path.
 * @step 4 Check returned object on NULL.
 * @step 5 Call efl_dbus_proxy_get function to get a proxy of the following interface name.
 * @step 6 Check returned proxy object on NULL.
 * @step 7 Call efl_dbus_proxy_data_set function to set data in proxy object.
 * @step 8 Call efl_dbus_proxy_data_get function to get data stored.
 * @step 9 Check returned data on NULL and correct value.
 * @step 10 Call efl_dbus_proxy_data_del function to delete data stored.
 * @step 11 Check returned data on NULL and correct value.
 * @step 12 Call efl_dbus_proxy_data_get function to get data stored.
 * @step 13 Check returned data on not equal NULL.
 * @step 14 Call efl_dbus_proxy_unref function to delete proxy object
 * @step 15 Call efl_dbus_object_unref function to delete connection dbus object
 * @step 16 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Function should set data without segmentation fault,
 * get and delete valid stored data.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_proxy_data_p)
{
   is_success = EINA_FALSE;

   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Object *obj = efl_dbus_object_get(conn, bus, path);
   ck_assert_ptr_ne(NULL, obj);

   Efl_Dbus_Proxy *proxy = efl_dbus_proxy_get(obj, interface);
   ck_assert_ptr_ne(NULL, proxy);

   efl_dbus_proxy_data_set(proxy, DATA_KEY, &proxy_data_stored);

   int *proxy_data_get = efl_dbus_proxy_data_get(proxy, DATA_KEY);
   ck_assert_ptr_ne(NULL, proxy_data_get);
   ck_assert_int_eq((*proxy_data_get), proxy_data_stored);

   int *proxy_data_del = efl_dbus_proxy_data_del(proxy, DATA_KEY);
   ck_assert_ptr_ne(NULL, proxy_data_del);
   ck_assert_int_eq((*proxy_data_del), proxy_data_stored);

   int *proxy_data_null = efl_dbus_proxy_data_get(proxy, DATA_KEY);
   ck_assert_ptr_eq(NULL, proxy_data_null);

   efl_dbus_proxy_unref(proxy);
   efl_dbus_object_unref(obj);
   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 *@}
 */
void
efl_dbus_test_efl_dbus_proxy(TCase *tc)
{
   tcase_add_test(tc, utc_efl_dbus_proxy_info_get_call_p);
   tcase_add_test(tc, utc_efl_dbus_proxy_send_call_p);
   tcase_add_test(tc, utc_efl_dbus_proxy_send_and_block_p);
   tcase_add_test(tc, utc_efl_dbus_proxy_data_p);
}
