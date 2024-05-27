#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>

#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Efl_Dbus.h>

#include "efl_dbus_suite.h"

static Efl_Bool
_quit_cb(void *data EFL_UNUSED)
{
    ck_abort_msg("dbus timeout triggered!");
    core_main_loop_quit();
    return EFL_FALSE;
}

EFL_START_TEST(efl_dbus_test_eldbus)
{}

EFL_END_TEST

EFL_START_TEST(efl_dbus_test_efl_dbus_conn)
{
    Efl_Dbus_Connection *conn;

    conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
    fail_if(conn == NULL);

    efl_dbus_connection_unref(conn);
}

EFL_END_TEST

EFL_START_TEST(efl_dbus_test_efl_dbus_conn_object)
{
    Efl_Dbus_Connection *conn;
    Efl_Dbus_Object     *obj;

    conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
    fail_if(conn == NULL);

    obj = efl_dbus_object_get(conn, "org.buu", "/org/buu");
    fail_if(obj == NULL);

    efl_dbus_object_unref(obj);

    efl_dbus_connection_unref(conn);
}

EFL_END_TEST

void
name_owner_changed_cb(void              *data,
                      const char *bus    EFL_UNUSED,
                      const char *old_id EFL_UNUSED,
                      const char        *new_id)
{
    const char **id = data;
    *id             = new_id;
    core_main_loop_quit();
}

EFL_START_TEST(efl_dbus_test_efl_dbus_name_owner_changed)
{
    Efl_Dbus_Connection *conn;
    const char          *id = NULL;

    conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);
    fail_if(conn == NULL);

    efl_dbus_name_owner_changed_callback_add(conn,
                                             "org.bus.that.not.exist",
                                             name_owner_changed_cb,
                                             &id,
                                             EFL_TRUE);
    core_timer_add(0.1, _quit_cb, NULL);

    core_main_loop_begin();

    fail_if(id == NULL);

    efl_dbus_connection_unref(conn);
}

EFL_END_TEST

void
efl_dbus_test_efl_dbus_init(TCase *tc)
{
    tcase_add_test(tc, efl_dbus_test_eldbus);
    tcase_add_test(tc, efl_dbus_test_efl_dbus_conn);
    tcase_add_test(tc, efl_dbus_test_efl_dbus_conn_object);
    tcase_add_test(tc, efl_dbus_test_efl_dbus_name_owner_changed);
}
