#include <Efl_Dbus.h>
#include <Efl_Core.h>
#include <string.h>
#include <stdio.h>
#include <Efl_Shared.h>

#include "efl_dbus_suite.h"

static Efl_Bool is_success          = EFL_FALSE;
static Efl_Bool is_register_service = EFL_FALSE;
static Efl_Bool is_peer_ping        = EFL_FALSE;

static Core_Timer *timeout = NULL;

static const char *empty_string = "";

static const char *bus         = "org.freedesktop.DBus";
static const char *interface   = "org.freedesktop.DBus";
static const char *path        = "/org/freedesktop/DBus";
static const char *method_name = "GetId";

static int cb_data = 5;

/**
* @addtogroup eldbus
* @{
* @defgroup efl_dbus_object
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
_object_message_cb(void                     *data,
                   const Efl_Dbus_Message   *msg,
                   Efl_Dbus_Pending *pending EFL_UNUSED)
{
  if (timeout != NULL)
  {
    core_timer_del(timeout);
    timeout = NULL;
  }

  const char *errname, *errmsg;
  int        *user_data = data;

  if ((user_data) && (*user_data == cb_data))
  {
    if (!efl_dbus_message_error_get(msg, &errname, &errmsg))
    {
      char *txt;
      if (efl_dbus_message_arguments_get(msg, "s", &txt))
      {
        if ((txt) && strcmp(txt, empty_string)) is_success = EFL_TRUE;
      }
    }
  }

  core_main_loop_quit();
}

static void
_name_request(void *data                EFL_UNUSED,
              const Efl_Dbus_Message   *msg,
              Efl_Dbus_Pending *pending EFL_UNUSED)
{
  unsigned int reply;
  const char  *errname, *errmsg;
  if (!efl_dbus_message_error_get(msg, &errname, &errmsg))
  {
    if (efl_dbus_message_arguments_get(msg, "u", &reply))
    {
      if (reply == EFL_DBUS_NAME_REQUEST_REPLY_PRIMARY_OWNER)
      {
        is_register_service = EFL_TRUE;
      }
    }
  }
}

static const Efl_Dbus_Method methods[] = {
  {         "Ping", NULL,                                 NULL, NULL, 0 },
  { "GetMachineId", NULL, EFL_DBUS_ARGS({ "s", "machine_id" }), NULL, 0 },
  {           NULL, NULL,                                 NULL, NULL, 0 }
};

static const Efl_Dbus_Service_Interface_Desc iface_desc = {
  "org.freedesktop.DBus.Peer", methods, NULL, NULL, NULL, NULL
};

static void
_peer_ping_cb(void                     *data,
              const Efl_Dbus_Message   *msg,
              Efl_Dbus_Pending *pending EFL_UNUSED)
{
  int *user_data = data;

  if (user_data && (*user_data == cb_data))
  {
    if (!efl_dbus_message_error_get(msg, NULL, NULL))
    {
      is_peer_ping = EFL_TRUE;
    }
  }
}

static void
_machine_id_get(void                     *data,
                const Efl_Dbus_Message   *msg,
                Efl_Dbus_Pending *pending EFL_UNUSED)
{
  int *user_data = data;

  if (user_data && (*user_data == cb_data))
  {
    if (!efl_dbus_message_error_get(msg, NULL, NULL))
    {
      char *txt;
      if (efl_dbus_message_arguments_get(msg, "s", &txt))
      {
        if (txt && strcmp(txt, empty_string))
        {
          is_success = EFL_TRUE;
        }
      }
    }
  }
}

/**
 * @addtogroup efl_dbus_object
 * @{
 * @defgroup efl_dbus_object_send_info_get
 * @li efl_dbus_object_bus_name_get()
 * @li efl_dbus_object_connection_get()
 * @li efl_dbus_object_method_call_new()
 * @li efl_dbus_object_path_get()
 * @li efl_dbus_object_ref()
 * @li efl_dbus_object_send()
 * @li efl_dbus_object_get()
 * @{
 * @objective Positive test case checks if functions get expected bus name,
 * get Efl_Dbus_Connection expected associated with Efl_Dbus_Object object,
 * get expected path associated with object,
 * create message object and send a message to DBus service,
 * increase object reference. Without segmentation fault.
 *
 * @n Input Data:
 * @li the conn object connection with bus
 * @li the obj Efl_Dbus_Object object of the given bus and path.
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get connection object.
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_object_get get an object of the given bus and path.
 * @step 4 Check returned object on NULL.
 * @step 5 Call efl_dbus_object_bus_name_get function to get valid string and check on expected string.
 * @step 6 Call efl_dbus_object_path_get function to get valid string and check on expected string.
 * @step 7 Call efl_dbus_object_ref function to increase object reference.
 * @step 8 Check two objects on the same address.
 * @step 9 Call efl_dbus_object_unref function to decrease object reference.
 * @step 10 Call efl_dbus_object_method_call_new function to call a dbus method on the object.
 * @step 11 Check returned message object on NULL.
 * @step 12 Call efl_dbus_object_send function to send a message.
 * @step 13 Check returned pending object on NULL.
 * @step 14 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 15 Start of main loop and wait for tested response in callback executing.
 * @step 16 Check static variable named is_success.
 * If is equal EFL_TRUE, that callback was executed and method send valid message response.
 * @step 17 Call efl_dbus_message_unref function to delete message object
 * @step 18 Call efl_dbus_object_unref function to delete connection dbus object
 * @step 19 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Function should get valid expected object information about service.
 * Variables named is_success must equals EFL_TRUE. Without segmentation fault.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_object_send_info_get_p)
{
  is_success = EFL_FALSE;

  Efl_Dbus_Connection *conn =
    efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
  ck_assert_ptr_ne(NULL, conn);

  Efl_Dbus_Object *obj = efl_dbus_object_get(conn, bus, path);
  ck_assert_ptr_ne(NULL, obj);

  const char *bus_from_object_ptr = efl_dbus_object_bus_name_get(obj);
  ck_assert_msg((strcmp(bus, bus_from_object_ptr) == 0),
                "'%s' != '%s'",
                bus,
                bus_from_object_ptr);

  const char *path_from_object_ptr = efl_dbus_object_path_get(obj);
  ck_assert_msg((strcmp(path, path_from_object_ptr) == 0),
                "'%s' != '%s'",
                path,
                path_from_object_ptr);

  Efl_Dbus_Object *obj_ref = efl_dbus_object_ref(obj);
  ck_assert_ptr_eq(obj_ref, obj);

  efl_dbus_object_unref(obj_ref);

  Efl_Dbus_Connection *connection_obj = efl_dbus_object_connection_get(obj);
  ck_assert_ptr_eq(connection_obj, conn);

  Efl_Dbus_Message *msg =
    efl_dbus_object_method_call_new(obj, interface, method_name);
  ck_assert_ptr_ne(NULL, msg);

  Efl_Dbus_Pending *pending =
    efl_dbus_object_send(obj, msg, _object_message_cb, &cb_data, -1);
  ck_assert_ptr_ne(NULL, pending);

  timeout = core_timer_add(0.1, _core_loop_close, NULL);
  ck_assert_ptr_ne(NULL, timeout);

  core_main_loop_begin();

  ck_assert_msg(is_success, "Method %s is not call", method_name);

  efl_dbus_connection_unref(conn);
}

EFL_END_TEST

/**
 * @addtogroup efl_dbus_object
 * @{
 * @defgroup efl_dbus_introspect efl_dbus_object_introspect()
 * @{
 * @objective Positive test case checks if function call the method "Introspect"
 * on the eldbus object and get valid response in callback function. Without segmentation fault.
 *
 * @n Input Data:
 * @li the conn object connection with bus
 * @li the obj Efl_Dbus_Object object of the given bus and path.
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get connection object.
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_object_get to get an object of the given bus and path.
 * @step 4 Check returned object on NULL.
 * @step 5 Call efl_dbus_object_introspect functiont to call the method "Introspect" on the eldbus object.
 * @step 6 Check returned pending object on NULL.
 * @step 7 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 8 Start of main loop and wait for tested response in callback executing.
 * @step 9 Check static variable named is_success.
 * If is equal EFL_TRUE, that callback was executed and method send valid message response.
 * @step 10 Call efl_dbus_object_unref function to delete connection dbus object.
 * @step 11 Call efl_dbus_connection_unref function to delete connection object.
 *
 * @passcondition Variables named is_success must equals EFL_TRUE, and there is no segmentation fault.
 * @}
 * @}
 */

EFL_START_TEST(utc_efl_dbus_introspect_p)
{
  is_success = EFL_FALSE;

  Efl_Dbus_Connection *conn =
    efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
  ck_assert_ptr_ne(NULL, conn);

  Efl_Dbus_Object *obj = efl_dbus_object_get(conn, bus, path);
  ck_assert_ptr_ne(NULL, obj);

  Efl_Dbus_Pending *pending =
    efl_dbus_object_introspect(obj, _object_message_cb, &cb_data);
  ck_assert_ptr_ne(NULL, pending);

  timeout = core_timer_add(0.1, _core_loop_close, NULL);
  ck_assert_ptr_ne(NULL, timeout);

  core_main_loop_begin();

  ck_assert_msg(is_success, "Method Introspect is not call");

  efl_dbus_object_unref(obj);
  efl_dbus_connection_unref(conn);
}

EFL_END_TEST

/**
 * @addtogroup efl_dbus_object
 * @{
 * @defgroup efl_dbus_object_peer
 * @li efl_dbus_object_peer_ping()
 * @li efl_dbus_object_peer_machine_id_get()
 * @li efl_dbus_object_get()
 * @{
 * @objective Positive test case checks if function call the method "Ping",
 * "GetMachineId" and get expected response without error. Without segmentation fault.
 *
 * @n Input Data:
 * @li the conn_server object connection to server
 * @li the conn_client object connection to client
 * @li the iface object register an interface
 *
 * @procedure
 * @step 1 Call efl_dbus_connection_get function to get server connection object.
 * @step 2 Check returned connection object on NULL.
 * @step 3 Call efl_dbus_service_interface_register function to register an interface.
 * in the given path and connection.
 * @step 4 Check returned interface object on NULL.
 * @step 5 Call efl_dbus_name_request function to send a "RequestName" method call in proxy.
 * @step 6 Check returned pending object on NULL.
 * @step 7 Call efl_dbus_connection_get function to get client connection object.
 * @step 8 Check returned connection object on NULL.
 * @step 9 Call efl_dbus_object_get function to get an client object
 * of the given bus and path.
 * @step 10 Check returned object on NULL.
 * @step 11 Call efl_dbus_object_peer_machine_id_get function to call the
 * method "GetMachineId" on the dbus object.
 * @step 12 Check returned pending object on NULL.
 * @step 13 Call efl_dbus_object_peer_ping function to call the
 * method "Ping" on the dbus object.
 * @step 14 Check returned pending object on NULL.
 * @step 15 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 16 Start of main loop and wait for tested response in callback executing.
 * @step 17 Check static variables named is_success, is_register_service, is_peer_ping.
 * If are equals 1, that callbacks were executed and methods send valid message response.
 * In other cases error.
 * @step 18 Call efl_dbus_object_unref function to delete connection dbus object.
 * @step 19 Call efl_dbus_connection_unref function to delete client connection object.
 * @step 20 Call efl_dbus_service_interface_unregister function to unregister a interface.
 * @step 21 Call efl_dbus_connection_unref function to delete server connection object
 *
 * @passcondition Variables named is_success_cb, is_register_service, is_peer_ping must equals 1, and there is no segmentation fault.
 * @}
 * @}
 */
EFL_START_TEST(utc_efl_dbus_object_peer_p)
{
  const char *dbus_path   = "/org/freedesktop/Test";
  const char *bus_session = "org.freedesktop.Test.utc_efl_dbus_object_peer_p";

  is_success          = EFL_FALSE;
  is_register_service = EFL_FALSE;
  is_peer_ping        = EFL_FALSE;

  Efl_Dbus_Connection *conn_server =
    efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
  ck_assert_ptr_ne(NULL, conn_server);

  Efl_Dbus_Service_Interface *iface =
    efl_dbus_service_interface_register(conn_server, dbus_path, &iface_desc);
  ck_assert_ptr_ne(NULL, iface);

  Efl_Dbus_Pending *pending_name =
    efl_dbus_name_request(conn_server,
                          bus_session,
                          EFL_DBUS_NAME_REQUEST_FLAG_DO_NOT_QUEUE,
                          _name_request,
                          iface);
  ck_assert_ptr_ne(NULL, pending_name);

  Efl_Dbus_Connection *conn_client =
    efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
  ck_assert_ptr_ne(NULL, conn_client);

  Efl_Dbus_Object *obj =
    efl_dbus_object_get(conn_client, bus_session, dbus_path);
  ck_assert_ptr_ne(NULL, obj);

  Efl_Dbus_Pending *pending1 =
    efl_dbus_object_peer_machine_id_get(obj, _machine_id_get, &cb_data);
  ck_assert_ptr_ne(NULL, pending1);

  Efl_Dbus_Pending *pending2 =
    efl_dbus_object_peer_ping(obj, _peer_ping_cb, &cb_data);
  ck_assert_ptr_ne(NULL, pending2);

  timeout = core_timer_add(0.15, _core_loop_close, NULL);
  ck_assert_ptr_ne(NULL, timeout);

  core_main_loop_begin();

  ck_assert_msg(is_register_service, "Can not register interface");
  ck_assert_msg(is_peer_ping, "Method Ping is not call");
  ck_assert_msg(is_success, "Method GetMachineId is not call");

  efl_dbus_object_unref(obj);
  efl_dbus_connection_unref(conn_client);
  efl_dbus_service_interface_unregister(iface);
  efl_dbus_connection_unref(conn_server);
}

EFL_END_TEST

/**
 *@}
 */
void
efl_dbus_test_efl_dbus_object(TCase *tc)
{
  tcase_add_test(tc, utc_efl_dbus_object_send_info_get_p);
  tcase_add_test(tc, utc_efl_dbus_introspect_p);
  tcase_add_test(tc, utc_efl_dbus_object_peer_p);
}
