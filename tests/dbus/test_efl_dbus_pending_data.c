#include <check.h>
#include <Efl_Dbus.h>
#include <Efl_Core.h>
#include <string.h>
#include <stdio.h>
#include <Efl_Shared.h>

#include "efl_dbus_suite.h"

static Efl_Bool is_response_cb = EFL_FALSE;

static Core_Timer *timeout = NULL;

static int  pending_data_stored = 5;
const char *pending_key_store   = "pending_data";

static Efl_Dbus_Connection *conn    = NULL;
static Efl_Dbus_Object     *obj     = NULL;
static Efl_Dbus_Message    *message = NULL;

/**
* @addtogroup eldbus
* @{
* @defgroup efl_dbus_pending_data
* @li efl_dbus_pending_data_set()
* @li efl_dbus_pending_data_get()
* @li efl_dbus_pending_data_del()
*
* @precondition
* @step 1 Initialize eldbus with efl_dbus_init()
*/

static Efl_Bool
_core_loop_close(void *data EFL_UNUSED)
{
  core_main_loop_quit();

  return CORE_CALLBACK_CANCEL;
}

static void
_response_message_cb(void *data                  EFL_UNUSED,
                     const Efl_Dbus_Message *msg EFL_UNUSED,
                     Efl_Dbus_Pending           *pending)
{
  ck_assert_ptr_ne(NULL, pending);

  int *pending_data_get = efl_dbus_pending_data_get(pending, pending_key_store);

  ck_assert_ptr_ne(NULL, pending_data_get);
  ck_assert_int_eq((*pending_data_get), pending_data_stored);

  int *pending_data_del = efl_dbus_pending_data_del(pending, pending_key_store);

  ck_assert_ptr_ne(NULL, pending_data_del);
  ck_assert_int_eq((*pending_data_del), pending_data_stored);

  is_response_cb = EFL_TRUE;

  if (timeout != NULL)
  {
    core_timer_del(timeout);
    timeout = NULL;
  }

  core_main_loop_quit();
}

/**
 * @addtogroup efl_dbus_pending_data
 * @{
 * @objective Positive test case checks if function correctly set, get and delete the data stored
 * in pending connection object and without segmentation fault.
 *
 * @n Input Data:
 * @li the conn object connection with bus
 * @li the obj object of the given bus and path
 * @li the message of the pointer to message strunct a dbus method
 * @li the pending connection object to store data
 * @li data that will be stored
 * @li key that identifies data
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get connection object
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_object_get function to get an object
 * of the given bus and path.
 * @step 4 Check returned object on NULL.
 * @step 5 Call efl_dbus_object_method_call_new function to call
 * a dbus method on the object.
 * @step 6 Check returned message on NULL.
 * @step 7 Call efl_dbus_connection_send function to send a message a dbus object with timeout.
 * @step 8 Check returned pending on NULL.
 * @step 9 Call efl_dbus_pending_data_set function to set data in pending connection object.
 * @step 10 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 11 Start of main loop and wait for tested response in callback executing.
 * @step 12 Check variable if callback executing.
 * @step 13 Call efl_dbus_message_unref function to delete message object
 * @step 14 Call efl_dbus_object_unref function to delete connection dbus object
 * @step 15 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Function should set data without segmentation fault,
   get and delete valid stored data.
 * @}
 */

EFL_START_TEST(utc_efl_dbus_pending_data_p)
{
  const char *bus             = "org.freedesktop.DBus";
  const char *path            = "/org/freedesktop/DBus";
  const char *interface       = "org.freedesktop.DBus";
  const char *member          = "GetId";
  const int   send_timeout_ms = 500;

  conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
  ck_assert_ptr_ne(NULL, conn);

  obj = efl_dbus_object_get(conn, bus, path);
  ck_assert_ptr_ne(NULL, obj);

  message = efl_dbus_object_method_call_new(obj, interface, member);
  ck_assert_ptr_ne(NULL, message);

  Efl_Dbus_Pending *pending = efl_dbus_connection_send(conn,
                                                       message,
                                                       _response_message_cb,
                                                       NULL,
                                                       send_timeout_ms);
  ck_assert_ptr_ne(NULL, pending);

  efl_dbus_pending_data_set(pending, pending_key_store, &pending_data_stored);

  timeout = core_timer_add(0.1, _core_loop_close, NULL);
  ck_assert_ptr_ne(NULL, timeout);

  core_main_loop_begin();

  ck_assert(is_response_cb == EFL_TRUE);

  efl_dbus_message_unref(message);
  efl_dbus_object_unref(obj);
  efl_dbus_connection_unref(conn);
}

EFL_END_TEST

void
efl_dbus_test_efl_dbus_pending_data(TCase *tc)
{
  tcase_add_test(tc, utc_efl_dbus_pending_data_p);
}
