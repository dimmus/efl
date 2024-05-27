#include <check.h>
#include <Efl_Dbus.h>
#include <Efl_Core.h>
#include <Efl_Shared.h>
#include <string.h>

#include "efl_dbus_suite.h"

static Efl_Bool is_success_cb = EFL_FALSE;

static Core_Timer *timeout = NULL;

static Efl_Dbus_Connection *conn    = NULL;
static Efl_Dbus_Object     *obj     = NULL;
static Efl_Dbus_Message    *message = NULL;

const char *bus       = "org.freedesktop.DBus";
const char *path      = "/org/freedesktop/DBus";
const char *interface = "org.freedesktop.DBus";
const char *member    = "GetId";

/**
* @addtogroup eldbus
* @{
* @defgroup efl_dbus_pending_info_get_cancel
* @li efl_dbus_pending_cancel()
* @li efl_dbus_pending_destination_get()
* @li efl_dbus_pending_interface_get()
* @li efl_dbus_pending_method_get()
* @li efl_dbus_pending_path_get()
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
_response_message_cb(void *data              EFL_UNUSED,
                     const Efl_Dbus_Message *msg,
                     Efl_Dbus_Pending       *pending)
{
    if (timeout != NULL)
    {
        core_timer_del(timeout);
        timeout = NULL;
    }

    if (!pending)
    {
        core_main_loop_quit();
        return;
    }

    const char *pending_path = efl_dbus_pending_path_get(pending);
    if ((!pending_path) || (strcmp(pending_path, path)))
    {
        core_main_loop_quit();
        return;
    }

    const char *pending_method = efl_dbus_pending_method_get(pending);
    if ((!pending_method) || (strcmp(pending_method, member)))
    {
        core_main_loop_quit();
        return;
    }

    const char *pending_interface = efl_dbus_pending_interface_get(pending);
    if ((!pending_interface) || (strcmp(pending_interface, interface)))
    {
        core_main_loop_quit();
        return;
    }

    const char *pending_destination = efl_dbus_pending_destination_get(pending);
    if ((!pending_destination) || (strcmp(pending_destination, bus)))
    {
        core_main_loop_quit();
        return;
    }

    const char *errname, *errmsg;

    if (efl_dbus_message_error_get(msg, &errname, &errmsg))
    {
        is_success_cb = EFL_TRUE;
    }

    core_main_loop_quit();
}

static Efl_Dbus_Pending *
_pending_connection_get(void)
{
    const int send_timeout_ms = 500;

    conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
    if (!conn)
    {
        return NULL;
    }

    obj = efl_dbus_object_get(conn, bus, path);
    if (!obj)
    {
        efl_dbus_connection_unref(conn);
        return NULL;
    }

    message = efl_dbus_object_method_call_new(obj, interface, member);
    if (!message)
    {
        efl_dbus_connection_unref(conn);
        return NULL;
    }

    Efl_Dbus_Pending *pending = efl_dbus_connection_send(conn,
                                                         message,
                                                         _response_message_cb,
                                                         NULL,
                                                         send_timeout_ms);
    if (!pending)
    {
        efl_dbus_connection_unref(conn);
        return NULL;
    }

    return pending;
}

/**
 * @addtogroup efl_dbus_pending_info_get_cancel
 * @{
 * @objective Positive test case checks if pending connection object canceled success.
 * If functions efl_dbus_pending_cancel() efl_dbus_pending_destination_get()
 * efl_dbus_pending_interface_get() efl_dbus_pending_method_get() efl_dbus_pending_path_get()
 * returned expected values
 *
 * @n Input Data:
 * @li the pending connection object
 * @li callback function to be called
 * @li the bus name of bus who listens
 * @li the path Dbus object path
 * @li the interface Dbus interface name
 * @li the member name of the method to be called.
 *
 * @procedure
 * @step 1 Get pendindig connection object and check on NULL
 * @step 2 Call efl_dbus_pending_cancel to cancel the pending message
 * @step 3 Set timer for preschedule termination of main loop if tested callback wasn't executed.
 * @step 4 Start of main loop and wait for tested response in callback executing.
 * @step 5 Check static variable named is_success_cb.
 * If are equals 1, that pending message was canceled and pending object
 * returned expeted information about connection services. In other cases error.
 * @step 6 Call efl_dbus_message_unref function to delete message object
 * @step 7 Call efl_dbus_object_unref function to delete connection dbus object
 * @step 8 Call efl_dbus_connection_unref function to delete connection object
 *
 * @passcondition Variables named is_success_cb must equals 1, and there is no segmentation fault.
 * @}
 */

EFL_START_TEST(utc_efl_dbus_pending_info_get_cancel_p)
{
    Efl_Dbus_Pending *pending = _pending_connection_get();
    ck_assert_ptr_ne(NULL, pending);

    efl_dbus_pending_cancel(pending);

    timeout = core_timer_add(0.1, _core_loop_close, NULL);
    ck_assert_ptr_ne(NULL, timeout);

    core_main_loop_begin();

    ck_assert(is_success_cb == EFL_TRUE);

    efl_dbus_connection_unref(conn);
}

EFL_END_TEST

void
efl_dbus_test_efl_dbus_pending_cancel(TCase *tc)
{
    tcase_add_test(tc, utc_efl_dbus_pending_info_get_cancel_p);
}
