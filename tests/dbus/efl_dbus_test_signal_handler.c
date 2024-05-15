#include <Efl_Dbus.h>
#include <Efl_Core.h>
#include <string.h>
#include <stdio.h>
#include <Efl_Shared.h>

#include "efl_dbus_suite.h"

static const char *bus = "org.freedesktop.DBus";
static const char *path = "/org/freedesktop/DBus";
static const char *interface = "org.freedesktop.DBus";
static const char *signal_name = "NameOwnerChanged";

static int cb_data = 5;

static Eina_Bool is_success_cb = EINA_FALSE;

static Ecore_Timer *timeout = NULL;

/**
* @addtogroup eldbus
* @{
* @defgroup efl_dbus_signal_handler
*
* @precondition
* @step 1 Initialize eldbus with efl_dbus_init()
*/

static Eina_Bool
_ecore_loop_close(void *data EINA_UNUSED)
{
   ecore_main_loop_quit();

   return ECORE_CALLBACK_CANCEL;
}

static void
_signal_name_owner_changed(void *data, const Efl_Dbus_Message *msg)
{
   const char *errname, *errmsg;
   int *user_data = data;

   if (timeout != NULL)
     {
        ecore_timer_del(timeout);
        timeout = NULL;
     }

   if ((user_data) && (*user_data == cb_data))
     {
        if (!efl_dbus_message_error_get(msg, &errname, &errmsg))
          {
             is_success_cb = EINA_TRUE;
          }
     }

   ecore_main_loop_quit();
}

static void
_response_message_cb(void *data EINA_UNUSED, const Efl_Dbus_Message *msg EINA_UNUSED, Efl_Dbus_Pending *pending EINA_UNUSED)
{
}

static void
_signal_handler_free_cb(void *data, const void *deadptr EINA_UNUSED)
{
   int *user_data = data;

   if (timeout != NULL)
     {
        ecore_timer_del(timeout);
        timeout = NULL;
     }

   if ((user_data) && (*user_data == cb_data))
     is_success_cb = EINA_TRUE;

   ecore_main_loop_quit();
}

static Efl_Dbus_Signal_Handler *
_signal_handler_get(Efl_Dbus_Connection *conn)
{
   is_success_cb = EINA_FALSE;

   Efl_Dbus_Signal_Handler *signal_handler = efl_dbus_signal_handler_add(conn, NULL, path, interface,
                                                                     signal_name, _signal_name_owner_changed, &cb_data);

   if (!signal_handler)
     {
        return NULL;
     }

   timeout = ecore_timer_add(0.1, _ecore_loop_close, NULL);
   if (!timeout)
     {
        efl_dbus_signal_handler_unref(signal_handler);
        return NULL;
     }

   return signal_handler;
}

/**
 * @addtogroup efl_dbus_signal_handler
 * @{
 * @defgroup efl_dbus_signal_handler_add efl_dbus_signal_handler_add()
 * @{
 * @objective Positive test case checks that function add a signal handler without error.
 *
 * @n Input Data:
 * @li the conn connection object
 * @li the msg message connection object
 * @li callback function to be called
 * @li data passed to callback
 *
 * @procedure
 * @step 1 Get eldbus connection object and check on NULL
 * @step 2 Call efl_dbus_signal_handler_add with callback function and data
 * to be called when this signal is received.
 * @step 3 Check returned signal handler object on NULL.
 * @step 4 Call efl_dbus_message_signal_new to create a new signal message.
 * @step 5 Check returned message object on NULL.
 * @step 6 Call efl_dbus_connection_send to send a signal a dbus object.
 * @step 7 Check returned pending object on NULL.
 * @step 8 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 9 Start of main loop and wait for tested response in callback executing.
 * @step 10 Check static variables named is_success_cb.
 * If are equals 1, that callback was executed and signal is received without error.
 * In other cases error.
 * @step 11 Call efl_dbus_message_unref function to delete message object
 * @step 12 Call efl_dbus_signal_handler_unref function to delete signal handler object
 * @step 13 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Variables named is_success_cb must equals 1, and there is no segmentation fault.
 * @}
 * @}
 */
EFL_START_TEST(utc_efl_dbus_signal_handler_add_p)
{
   is_success_cb = EINA_FALSE;

   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Signal_Handler *signal_handler = efl_dbus_signal_handler_add(conn, NULL, path, interface,
                                                                     signal_name, _signal_name_owner_changed, &cb_data);

   ck_assert_ptr_ne(NULL, signal_handler);

   Efl_Dbus_Message *msg = efl_dbus_message_signal_new(path, interface, signal_name);
   ck_assert_ptr_ne(NULL, signal_handler);

   Efl_Dbus_Pending *pending = efl_dbus_connection_send(conn, msg, _response_message_cb, NULL, -1);
   ck_assert_ptr_ne(NULL, pending);

   timeout = ecore_timer_add(0.1, _ecore_loop_close, NULL);
   ck_assert_ptr_ne(NULL, timeout);

   ecore_main_loop_begin();

   ck_assert_msg(is_success_cb, "Signal %s is not call", signal_name);

   efl_dbus_signal_handler_unref(signal_handler);
   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 * @addtogroup efl_dbus_signal_handler
 * @{
 * @defgroup efl_dbus_signal_handler_del efl_dbus_signal_handler_del()
 * @{
 * @objective Positive test case checks that function add and delete a signal handler without error.
 *
 * @n Input Data:
 * @li the conn connection object
 * @li the msg message connection object
 * @li callback function to be called
 * @li data passed to callback
 *
 * @procedure
 * @step 1 Get eldbus connection object and check on NULL
 * @step 2 Call efl_dbus_signal_handler_add with callback function and data
 * to be called when this signal is received.
 * @step 3 Check returned signal handler object on NULL.
 * @step 4 Call efl_dbus_signal_handler_del the signal handler will stop listening to signals.
 * @step 5 Call efl_dbus_message_signal_new to create a new signal message.
 * @step 6 Check returned message object on NULL.
 * @step 7 Call efl_dbus_connection_send to send a signal a dbus object.
 * @step 8 Check returned pending object on NULL.
 * @step 9 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 10 Start of main loop and wait for tested response in callback executing.
 * @step 11 Check static variables named is_success_cb.
 * If is equal 1, that callback was executed and this is error.
 * @step 12 Call efl_dbus_message_unref function to delete message object
 * @step 13 Call efl_dbus_signal_handler_unref function to delete signal handler object
 * @step 14 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Variables named is_success_cb must equals 0
 * and efl_dbus_signal_handler_del must be delete registered callback, and there is no segmentation fault.
 * @}
 * @}
 */
EFL_START_TEST(utc_efl_dbus_signal_handler_del_p)
{
   is_success_cb = EINA_FALSE;

   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Signal_Handler *signal_handler = efl_dbus_signal_handler_add(conn, NULL, path, interface,
                                                                     signal_name, _signal_name_owner_changed, &cb_data);

   ck_assert_ptr_ne(NULL, signal_handler);

   efl_dbus_signal_handler_del(signal_handler);

   Efl_Dbus_Message *msg = efl_dbus_message_signal_new(path, interface, signal_name);
   ck_assert_ptr_ne(NULL, msg);

   Efl_Dbus_Pending *pending = efl_dbus_connection_send(conn, msg, _response_message_cb, NULL, -1);
   ck_assert_ptr_ne(NULL, pending);

   timeout = ecore_timer_add(0.1, _ecore_loop_close, NULL);
   ck_assert_ptr_ne(NULL, timeout);

   ecore_main_loop_begin();

   ck_assert_msg(!is_success_cb, "Signal %s was called", signal_name);

   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 * @addtogroup efl_dbus_signal_handler
 * @{
 * @defgroup efl_dbus_signal_handler_get
 * @li efl_dbus_signal_handler_connection_get()
 * @li efl_dbus_signal_handler_interface_get()
 * @li efl_dbus_signal_handler_member_get()
 * @li efl_dbus_signal_handler_path_get()
 * @li efl_dbus_signal_handler_sender_get()
 * @{
 * @objective Positive test case checks those functions return valid expected value.
 *
 * @n Input Data:
 * @li the conn connection object
 * @li the path to bus interface
 * @li the sender bus name
 * @li the interface bus interface
 * @li the signal_name bus signal name
 *
 * @procedure
 * @step 1 Get eldbus connection object and check on NULL
 * @step 2 Call efl_dbus_signal_handler_add with callback function and data
 * to be called when this signal is received.
 * @step 3 Check returned signal handler object on NULL.
 * @step 4 Call efl_dbus_signal_handler_connection_get and check on expected value.
 * @step 5 Call efl_dbus_signal_handler_path_get and check on expected value.
 * @step 6 Call efl_dbus_signal_handler_sender_get and check on expected value.
 * @step 7 Call efl_dbus_signal_handler_interface_get and check on expected value.
 * @step 8 Call efl_dbus_signal_handler_member_get and check on expected value.
 * @step 9 Call efl_dbus_connection_unref function to delete signal handler object
 * @step 10 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Functions should returns valid expected value and without segmentation fault.
 * @}
 * @}
 */
EFL_START_TEST(utc_efl_dbus_signal_handler_get_p)
{
   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Signal_Handler *signal_handler = efl_dbus_signal_handler_add(conn, bus, path, interface,
                                                                     signal_name, _signal_name_owner_changed, &cb_data);
   ck_assert_ptr_ne(NULL, signal_handler);

   Efl_Dbus_Connection *conn_get = efl_dbus_signal_handler_connection_get(signal_handler);
   ck_assert_msg((conn_get == conn), "Connection objects are different");

   const char *signal_path = efl_dbus_signal_handler_path_get(signal_handler);
   ck_assert_msg(NULL != signal_path, "Signal path is null");
   ck_assert_msg((!strcmp(signal_path, path)), "Signal path is different than original");

   const char *signal_sender = efl_dbus_signal_handler_sender_get(signal_handler);
   ck_assert_msg(NULL != signal_sender, "Signal sender is null");
   ck_assert_msg((!strcmp(signal_sender, bus)), "Signal sender is different than original");

   const char *signal_interface = efl_dbus_signal_handler_interface_get(signal_handler);
   ck_assert_msg(NULL != signal_interface, "Signal interface is null");
   ck_assert_msg((!strcmp(signal_interface, interface)), "Signal interface is different than original");

   const char *signal_handler_name = efl_dbus_signal_handler_member_get(signal_handler);
   ck_assert_msg(NULL != signal_handler_name, "Signal name is null");
   ck_assert_msg((!strcmp(signal_handler_name, signal_name)), "Signal name is different than original");

   efl_dbus_signal_handler_unref(signal_handler);
   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 * @addtogroup efl_dbus_signal_handler
 * @{
 * @defgroup efl_dbus_signal_handler_ref_unref
 * @li efl_dbus_signal_handler_ref()
 * @li efl_dbus_signal_handler_unref()
 * @{
 * @objective Positive test case checks that function increase, Decrease signal handler reference without error.
 *
 * @n Input Data:
 * @li the conn connection object
 * @li the signal_handler signal handler object
 *
 * @procedure
 * @step 1 Get eldbus connection object and check on NULL
 * @step 2 Call efl_dbus_signal_handler_add with callback function and data
 * to be called when this signal is received.
 * @step 3 Check returned signal handler object on NULL.
 * @step 4 Call efl_dbus_signal_handler_ref the increase signal handler reference.
 * @step 5 Check returned signal ref handler object with original signal handler object.
 * @step 6 Call efl_dbus_signal_handler_unref function decrease reference signal handler reference
 * @step 7 Call efl_dbus_signal_handler_unref function decrease original signal handler reference
 * @step 8 Call efl_dbus_message_signal_new to create a new signal message.
 * @step 9 Check returned message object on NULL.
 * @step 10 Call efl_dbus_connection_send to send a signal a dbus object.
 * @step 11 Check returned pending object on NULL.
 * @step 12 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 13 Start of main loop and wait for tested response in callback executing.
 * @step 14 Check static variables named is_success_cb.
 * If is equal 1, that callback was executed and this is error.
 * @step 15 Call efl_dbus_message_unref function to delete message object
 * @step 16 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Variables named is_success_cb must equals 0
 * and efl_dbus_signal_handler_unref must be decrease signal handler reference, and there is no segmentation fault.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_signal_handler_ref_unref_p)
{
   is_success_cb = EINA_FALSE;

   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Signal_Handler *signal_handler = efl_dbus_signal_handler_add(conn, NULL, path, interface,
                                                                     signal_name, _signal_name_owner_changed, &cb_data);
   ck_assert_ptr_ne(NULL, signal_handler);

   Efl_Dbus_Signal_Handler* signal_ref = efl_dbus_signal_handler_ref(signal_handler);
   ck_assert_msg((signal_ref == signal_handler), "Signal references are different");

   efl_dbus_signal_handler_unref(signal_ref);
   efl_dbus_signal_handler_unref(signal_handler);

   Efl_Dbus_Message *msg = efl_dbus_message_signal_new(path, interface, signal_name);
   ck_assert_ptr_ne(NULL, msg);

   Efl_Dbus_Pending *pending = efl_dbus_connection_send(conn, msg, _response_message_cb, NULL, -1);
   ck_assert_ptr_ne(NULL, pending);

   timeout = ecore_timer_add(0.1, _ecore_loop_close, NULL);
   ck_assert_ptr_ne(NULL, timeout);

   ecore_main_loop_begin();

   ck_assert_msg(!is_success_cb, "Signal %s was called", signal_name);

   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 * @addtogroup efl_dbus_signal_handler
 * @{
 * @defgroup efl_dbus_signal_handler_free_cb
 * @li efl_dbus_signal_handler_free_cb_add()
 * @li efl_dbus_signal_handler_free_cb_del()
 * @{
 * @objective Positive test case checks that function add and delete a callback function without error.
 *
 * @n Input Data:
 * @li the conn connection object
 * @li the signal_handler signal handler object
 *
 * @procedure
 * @step 1 Get eldbus connection object and check on NULL
 * @step 2 Get signal handler object and check on NULL.
 * @step 3 Call efl_dbus_signal_handler_free_cb_add to add a callback function
 * to be called when signal handler will be freed.
 * @step 4 Call efl_dbus_signal_handler_unref function decrease reference signal handler reference
 * @step 5 Start of main loop and wait for tested response in callback executing.
 * @step 6 Check static variables named is_success_cb.
 * If is equal 1, that callback was executed ignal handler was freed is received without error.

 * @step 7 Get signal handler object and check on NULL.
 * @step 8 Call efl_dbus_signal_handler_free_cb_add to add a callback function
 * to be called when signal handler will be freed.
 * @step 9 Call efl_dbus_signal_handler_free_cb_del to remove callback registered
 * in efl_dbus_signal_handler_free_cb_add().
 * @step 10 Call efl_dbus_signal_handler_unref function decrease reference signal handler reference
 * @step 11 Check static variables named is_success_cb.
 * If is equal 1, that callback was executed and this is error.
 * @step 12 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition The function should add and delete registered callback function,
 * and there is no segmentation fault.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_signal_handler_free_cb_add_del_p)
{
   Efl_Dbus_Connection *conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
   ck_assert_ptr_ne(NULL, conn);

   Efl_Dbus_Signal_Handler *signal_handler = _signal_handler_get(conn);
   ck_assert_ptr_ne(NULL, signal_handler);

   efl_dbus_signal_handler_free_cb_add(signal_handler, _signal_handler_free_cb, &cb_data);

   efl_dbus_signal_handler_unref(signal_handler);

   ck_assert_msg(is_success_cb, "Callback is not called");

   signal_handler = _signal_handler_get(conn);
   ck_assert_ptr_ne(NULL, signal_handler);

   efl_dbus_signal_handler_free_cb_add(signal_handler, _signal_handler_free_cb, &cb_data);
   efl_dbus_signal_handler_free_cb_del(signal_handler, _signal_handler_free_cb, &cb_data);

   efl_dbus_signal_handler_unref(signal_handler);

   ecore_main_loop_begin();

   ck_assert_msg(!is_success_cb, "Callback was called");

   efl_dbus_connection_unref(conn);
}
EFL_END_TEST

/**
 *@}
 */
void
efl_dbus_test_efl_dbus_signal_handler(TCase *tc)
{
   tcase_add_test(tc, utc_efl_dbus_signal_handler_add_p);
   tcase_add_test(tc, utc_efl_dbus_signal_handler_del_p);
   tcase_add_test(tc, utc_efl_dbus_signal_handler_get_p);
   tcase_add_test(tc, utc_efl_dbus_signal_handler_ref_unref_p);
   tcase_add_test(tc, utc_efl_dbus_signal_handler_free_cb_add_del_p);
}
