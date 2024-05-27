#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>

#include "efl_dbus_suite.h"
#include "test_efl_dbus_fake_server.h"

#define FAKE_SERVER_DATA_KEY "data"

static void     _fake_server_name_request_cb(void *,
                                             const Efl_Dbus_Message *,
                                             Efl_Dbus_Pending *);
static Efl_Bool _fakse_server_property_get(const Efl_Dbus_Service_Interface *,
                                           const char *,
                                           Efl_Dbus_Message_Iter *,
                                           const Efl_Dbus_Message *,
                                           Efl_Dbus_Message **);
static Efl_Dbus_Message *
_fake_server_property_set(const Efl_Dbus_Service_Interface *,
                          const char *,
                          Efl_Dbus_Message_Iter *,
                          const Efl_Dbus_Message *);
static Efl_Dbus_Message *_fake_server_sum(const Efl_Dbus_Service_Interface *,
                                          const Efl_Dbus_Message *);
static Efl_Dbus_Message *_fake_server_ping(const Efl_Dbus_Service_Interface *,
                                           const Efl_Dbus_Message *);
static Efl_Bool          _fake_server_send_pong_signal(void *);

static Efl_Dbus_Connection *conn;

static const Efl_Dbus_Method methods[] = {
    { FAKE_SERVER_SUM_METHOD_NAME,
     EFL_DBUS_ARGS({ "i", "a" }, { "i", "b" }),
     EFL_DBUS_ARGS({ "i", "result" }),
     _fake_server_sum, 0 },
    { FAKE_SERVER_PING_METHOD_NAME,
     EFL_DBUS_ARGS({ "i", "a" }),
     NULL, _fake_server_ping,
     0 },
    { NULL, NULL, NULL, NULL, 0 }
};

enum
{
    FAKE_SERVER_PONG_SIGNAL = 0
};

static const Efl_Dbus_Signal signals[] = {
    [FAKE_SERVER_PONG_SIGNAL] = {FAKE_SERVER_PONG_SIGNAL_NAME,
                                 EFL_DBUS_ARGS({ "i", NULL }),
                                 0                                    },
    {                        NULL, NULL, 0}
};

static const Efl_Dbus_Property properties[] = {
    { FAKE_SERVER_READONLY_PROPERTY,"i", _fakse_server_property_get,NULL, 0                                                                      },
    {FAKE_SERVER_WRITEONLY_PROPERTY,  "i",                       NULL, _fake_server_property_set, 0},
    {FAKE_SERVER_READWRITE_PROPERTY,
     "i", _fakse_server_property_get,
     _fake_server_property_set, 0                                                                  },
    {                          NULL, NULL,                       NULL,                      NULL, 0}
};

static const Efl_Dbus_Service_Interface_Desc test_interface_desc = {
    FAKE_SERVER_INTERFACE, methods, signals, properties, NULL, NULL
};

Efl_Dbus_Service_Interface *
fake_server_start(Fake_Server_Data *data, const char *name)
{
    ck_assert_ptr_ne(NULL, data);

    conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SESSION);

    Efl_Dbus_Service_Interface *interface =
        efl_dbus_service_interface_register(conn,
                                            FAKE_SERVER_PATH,
                                            &test_interface_desc);
    efl_dbus_service_object_data_set(interface, FAKE_SERVER_DATA_KEY, data);

    efl_dbus_name_request(conn,
                          name,
                          EFL_DBUS_NAME_REQUEST_FLAG_DO_NOT_QUEUE,
                          _fake_server_name_request_cb,
                          interface);

    core_main_loop_begin();

    return interface;
}

void
fake_server_stop(Efl_Dbus_Service_Interface *interface)
{
    efl_dbus_service_object_unregister(interface);
    efl_dbus_connection_unref(conn);
}

static Efl_Dbus_Message *
_fake_server_sum(const Efl_Dbus_Service_Interface *iface EFL_UNUSED,
                 const Efl_Dbus_Message                 *msg)
{
    int a, b;
    if (!efl_dbus_message_arguments_get(msg, "ii", &a, &b))
        return efl_dbus_message_error_new(msg,
                                          "Invalid arguments",
                                          "Error getting arguments..");

    int sum = a + b;

    Efl_Dbus_Message *reply = efl_dbus_message_method_return_new(msg);
    efl_dbus_message_arguments_append(reply, "i", sum);
    return reply;
}

static Efl_Dbus_Message *
_fake_server_ping(const Efl_Dbus_Service_Interface *iface,
                  const Efl_Dbus_Message           *msg)
{
    int a;
    if (!efl_dbus_message_arguments_get(msg, "i", &a))
        return efl_dbus_message_error_new(msg,
                                          "Invalid arguments",
                                          "Error getting arguments..");

    Fake_Server_Data *pd =
        efl_dbus_service_object_data_get(iface, FAKE_SERVER_DATA_KEY);
    ck_assert_ptr_ne(NULL, pd);

    pd->pong_response = a + 1;
    Core_Timer *timer =
        core_timer_add(0.1, _fake_server_send_pong_signal, iface);
    ck_assert_ptr_ne(NULL, timer);

    return NULL;
}

static Efl_Bool
_fake_server_send_pong_signal(void *data)
{
    Efl_Dbus_Service_Interface *iface = (Efl_Dbus_Service_Interface *)data;

    Fake_Server_Data *pd =
        efl_dbus_service_object_data_get(iface, FAKE_SERVER_DATA_KEY);
    ck_assert_ptr_ne(NULL, pd);

    efl_dbus_service_signal_emit(iface,
                                 FAKE_SERVER_PONG_SIGNAL,
                                 pd->pong_response);
    return CORE_CALLBACK_CANCEL;
}

Efl_Bool
_fakse_server_property_get(const Efl_Dbus_Service_Interface   *iface,
                           const char                         *propname,
                           Efl_Dbus_Message_Iter              *iter,
                           const Efl_Dbus_Message *request_msg EFL_UNUSED,
                           Efl_Dbus_Message **error            EFL_UNUSED)
{
    Fake_Server_Data *data =
        efl_dbus_service_object_data_get(iface, FAKE_SERVER_DATA_KEY);
    ck_assert_ptr_ne(NULL, data);

    if (strcmp(propname, FAKE_SERVER_READONLY_PROPERTY) == 0)
        efl_dbus_message_iter_arguments_append(iter,
                                               "i",
                                               data->readonly_property);
    else if (strcmp(propname, FAKE_SERVER_READWRITE_PROPERTY) == 0)
        efl_dbus_message_iter_arguments_append(iter,
                                               "i",
                                               data->readwrite_property);
    else return EFL_FALSE;

    return EFL_TRUE;
}

Efl_Dbus_Message *
_fake_server_property_set(const Efl_Dbus_Service_Interface *iface,
                          const char                       *propname,
                          Efl_Dbus_Message_Iter            *iter,
                          const Efl_Dbus_Message           *msg)
{
    Fake_Server_Data *data =
        efl_dbus_service_object_data_get(iface, FAKE_SERVER_DATA_KEY);
    ck_assert_ptr_ne(NULL, data);

    if (strcmp(propname, FAKE_SERVER_WRITEONLY_PROPERTY) == 0)
    {
        if (!efl_dbus_message_iter_arguments_get(iter,
                                                 "i",
                                                 &data->writeonly_property))
            ck_abort_msg(
                "error getting iter arguments on _fake_server_property_set");
    }
    else
    {
        if (strcmp(propname, FAKE_SERVER_READWRITE_PROPERTY) == 0)
        {
            if (!efl_dbus_message_iter_arguments_get(iter,
                                                     "i",
                                                     &data->readwrite_property))
                ck_abort_msg("error getting iter arguments on "
                             "_fake_server_property_set");
        }
        else
            return efl_dbus_message_error_new(msg,
                                              "Invalid property",
                                              "Invalid property.");
    }

    return efl_dbus_message_method_return_new(msg);
}

static void
_fake_server_name_request_cb(void *data                EFL_UNUSED,
                             const Efl_Dbus_Message   *msg,
                             Efl_Dbus_Pending *pending EFL_UNUSED)
{
    const char *name, *text;
    if (efl_dbus_message_error_get(msg, &name, &text))
        ck_abort_msg("error on _fake_server_name_request_cb: %s %s",
                     name,
                     text);

    unsigned int reply;
    if (!efl_dbus_message_arguments_get(msg, "u", &reply))
        ck_abort_msg("error geting arguments on _fake_server_name_request_cb");

    if (EFL_DBUS_NAME_REQUEST_REPLY_PRIMARY_OWNER != reply)
    {
        const char *errcode = "Unknown reply";
        switch (reply)
        {
            case EFL_DBUS_NAME_REQUEST_REPLY_IN_QUEUE:
                errcode = "Service could not become the primary owner and has "
                          "been placed in the queue";
                break;
            case EFL_DBUS_NAME_REQUEST_REPLY_EXISTS:
                errcode = "Service is already in the queue";
                break;
            case EFL_DBUS_NAME_REQUEST_REPLY_ALREADY_OWNER:
                errcode = "Service is already the primary owner";
                break;
            default:
                break;
        }
        ck_abort_msg("Failed to start fake server: %s (%u)", errcode, reply);
    }

    core_main_loop_quit();
}
