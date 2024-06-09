#include "efl_dbus_private.h"
#include "efl_dbus_private_types.h"
#include <dbus/dbus.h>

/* TODO: mempool of Efl_Dbus_Pending */
#define EFL_DBUS_PENDING_CHECK(pending)                         \
    do                                                          \
    {                                                           \
        EINA_SAFETY_ON_NULL_RETURN(pending);                    \
        if (!EINA_MAGIC_CHECK(pending, EFL_DBUS_PENDING_MAGIC)) \
        {                                                       \
            EINA_MAGIC_FAIL(pending, EFL_DBUS_PENDING_MAGIC);   \
            return;                                             \
        }                                                       \
    }                                                           \
    while (0)

#define EFL_DBUS_PENDING_CHECK_RETVAL(pending, retval)          \
    do                                                          \
    {                                                           \
        EINA_SAFETY_ON_NULL_RETURN_VAL(pending, retval);        \
        if (!EINA_MAGIC_CHECK(pending, EFL_DBUS_PENDING_MAGIC)) \
        {                                                       \
            EINA_MAGIC_FAIL(pending, EFL_DBUS_PENDING_MAGIC);   \
            return retval;                                      \
        }                                                       \
    }                                                           \
    while (0)

static void efl_dbus_pending_dispatch(Efl_Dbus_Pending *pending,
                                      Efl_Dbus_Message *msg);

Efl_Bool
efl_dbus_pending_init(void)
{
  return EFL_TRUE;
}

void
efl_dbus_pending_shutdown(void)
{}

static void
cb_pending(DBusPendingCall *dbus_pending, void *user_data)
{
  Efl_Dbus_Message *msg;
  Efl_Dbus_Pending *pending = user_data;

  if (!dbus_pending_call_get_completed(dbus_pending))
  {
    INF("timeout to pending %p", pending);
    dbus_pending_call_cancel(dbus_pending);
    msg = efl_dbus_message_error_new(pending->msg_sent,
                                     EFL_DBUS_ERROR_PENDING_TIMEOUT,
                                     "This call was not completed in time.");
    efl_dbus_pending_dispatch(pending, msg);
    return;
  }

  msg = efl_dbus_message_new(EFL_FALSE);
  EINA_SAFETY_ON_NULL_RETURN(msg);
  msg->dbus_msg = dbus_pending_call_steal_reply(dbus_pending);
  if (!msg->dbus_msg)
  {
    EINA_SAFETY_ON_NULL_GOTO(pending->cb, cleanup);

    msg->dbus_msg =
      dbus_message_new_error(NULL,
                             "org.enlightenment.DBus.NoReply",
                             "There was no reply to this method call.");
    EINA_SAFETY_ON_NULL_GOTO(msg->dbus_msg, cleanup);
  }

  dbus_message_iter_init(msg->dbus_msg, &msg->iterator->dbus_iterator);
  efl_dbus_pending_dispatch(pending, msg);

  return;

cleanup:
  efl_dbus_message_unref(msg);
}

static void
_on_conn_message_cb(void                   *data,
                    const Efl_Dbus_Message *msg,
                    Efl_Dbus_Pending       *pending)
{
  Efl_Dbus_Message_Cb  cb = efl_dbus_pending_data_del(pending, "__user_cb");
  Efl_Dbus_Connection *conn =
    efl_dbus_pending_data_del(pending, "__connection");

  EINA_SAFETY_ON_NULL_RETURN(conn);
  efl_dbus_connection_pending_del(conn, pending);
  cb(data, msg, pending);
}

EAPI Efl_Dbus_Pending *
efl_dbus_connection_send(Efl_Dbus_Connection *conn,
                         Efl_Dbus_Message    *msg,
                         Efl_Dbus_Message_Cb  cb,
                         const void          *cb_data,
                         double               timeout)
{
  Efl_Dbus_Pending *pending;

  EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(msg, NULL);

  if (!cb)
  {
    _efl_dbus_connection_send(conn, msg, NULL, NULL, timeout);
    return NULL;
  }
  pending =
    _efl_dbus_connection_send(conn, msg, _on_conn_message_cb, cb_data, timeout);
  EINA_SAFETY_ON_NULL_RETURN_VAL(pending, NULL);

  efl_dbus_pending_data_set(pending, "__user_cb", cb);
  efl_dbus_pending_data_set(pending, "__connection", conn);
  efl_dbus_connection_pending_add(conn, pending);
  return pending;
}

static Efl_Dbus_Message *
_efl_dbus_message_error_get(const Efl_Dbus_Message *msg,
                            const char             *error_name,
                            const char             *error_msg)
{
  int32_t serial;

  serial = dbus_message_get_serial(msg->dbus_msg);
  if (serial == 0)
  {
    return NULL;
  }

  return efl_dbus_message_error_new(msg, error_name, error_msg);
}

/*
 * On success @param msg is unref'd or its ref is stolen by the returned
 * Efl_Dbus_Pending.
 */
Efl_Dbus_Pending *
_efl_dbus_connection_send(Efl_Dbus_Connection *conn,
                          Efl_Dbus_Message    *msg,
                          Efl_Dbus_Message_Cb  cb,
                          const void          *cb_data,
                          double               timeout)
{
  Efl_Dbus_Pending *pending;
  Efl_Dbus_Message *error_msg;
  DBG("conn=%p, msg=%p, cb=%p, cb_data=%p, timeout=%f",
      conn,
      msg,
      cb,
      cb_data,
      timeout);

  if (!cb)
  {
    dbus_connection_send(conn->dbus_conn, msg->dbus_msg, NULL);
    efl_dbus_message_unref(msg);
    return NULL;
  }

  pending = calloc(1, sizeof(Efl_Dbus_Pending));
  EINA_SAFETY_ON_NULL_RETURN_VAL(pending, NULL);

  pending->cb      = cb;
  pending->cb_data = cb_data;
  pending->conn    = conn;
  pending->dest =
    eina_stringshare_add(dbus_message_get_destination(msg->dbus_msg));
  pending->interface =
    eina_stringshare_add(dbus_message_get_interface(msg->dbus_msg));
  pending->method =
    eina_stringshare_add(dbus_message_get_member(msg->dbus_msg));
  pending->path = eina_stringshare_add(dbus_message_get_path(msg->dbus_msg));

   /* Steal the reference */
  pending->msg_sent = msg;

  EINA_MAGIC_SET(pending, EFL_DBUS_PENDING_MAGIC);

  if (!dbus_connection_send_with_reply(conn->dbus_conn,
                                       msg->dbus_msg,
                                       &pending->dbus_pending,
                                       timeout))
  {
    error_msg =
      _efl_dbus_message_error_get(msg,
                                  "org.enlightenment.DBus.NoConnection",
                                  "Efl_Dbus_Connection was closed.");
    efl_dbus_pending_dispatch(pending, error_msg);
    return NULL;
  }
  if (!pending->dbus_pending)
  {
    error_msg = _efl_dbus_message_error_get(msg,
                                            "org.enlightenment.DBus.Error",
                                            "dbus_pending is NULL.");
    efl_dbus_pending_dispatch(pending, error_msg);
    return NULL;
  }
  if (dbus_pending_call_set_notify(pending->dbus_pending,
                                   cb_pending,
                                   pending,
                                   NULL))
    return pending;

  dbus_pending_call_cancel(pending->dbus_pending);
  error_msg =
    _efl_dbus_message_error_get(pending->msg_sent,
                                "org.enlightenment.DBus.Error",
                                "Error when try set callback to message.");
  efl_dbus_pending_dispatch(pending, error_msg);
  return NULL;
}

Efl_Dbus_Message *
_efl_dbus_connection_send_and_block(Efl_Dbus_Connection *conn,
                                    Efl_Dbus_Message    *msg,
                                    double               timeout)
{
  Efl_Dbus_Message *reply = NULL;
  DBusError         err;
  DBusMessage      *dbus_msg;

  if (core_main_loop_nested_get())
    WRN("Calling this function may result in dropped frames because the "
        "main loop is running");

  dbus_error_init(&err);
  dbus_msg = dbus_connection_send_with_reply_and_block(conn->dbus_conn,
                                                       msg->dbus_msg,
                                                       timeout,
                                                       &err);
  EINA_SAFETY_ON_TRUE_GOTO(dbus_error_is_set(&err), dbus_error_set);
  dbus_error_free(&err);

  reply = efl_dbus_message_new(EFL_FALSE);
  EINA_SAFETY_ON_NULL_GOTO(reply, fail);

  reply->dbus_msg = dbus_msg;
  dbus_message_iter_init(reply->dbus_msg, &reply->iterator->dbus_iterator);
  efl_dbus_message_unref(msg);
  return reply;

dbus_error_set:
  reply = efl_dbus_message_error_new(msg, err.name, err.message);
  dbus_error_free(&err);
fail:
  efl_dbus_message_unref(msg);
  return reply;
}

EAPI void
efl_dbus_pending_data_set(Efl_Dbus_Pending *pending,
                          const char       *key,
                          const void       *data)
{
  EFL_DBUS_PENDING_CHECK(pending);
  EINA_SAFETY_ON_NULL_RETURN(key);
  EINA_SAFETY_ON_NULL_RETURN(data);
  efl_dbus_data_set(&(pending->data), key, data);
}

EAPI void *
efl_dbus_pending_data_get(const Efl_Dbus_Pending *pending, const char *key)
{
  EFL_DBUS_PENDING_CHECK_RETVAL(pending, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(key, NULL);
  return efl_dbus_data_get(&(((Efl_Dbus_Pending *)pending)->data), key);
}

EAPI void *
efl_dbus_pending_data_del(Efl_Dbus_Pending *pending, const char *key)
{
  EFL_DBUS_PENDING_CHECK_RETVAL(pending, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(key, NULL);
  return efl_dbus_data_del(&(((Efl_Dbus_Pending *)pending)->data), key);
}

static void
efl_dbus_pending_dispatch(Efl_Dbus_Pending *pending, Efl_Dbus_Message *msg)
{
  DBG("pending=%p msg=%p", pending, msg);
  if (pending->cb) pending->cb((void *)pending->cb_data, msg, pending);

  efl_dbus_cbs_free_dispatch(&(pending->cbs_free), pending);
  efl_dbus_data_del_all(&(pending->data));

  if (msg) efl_dbus_message_unref(msg);
  efl_dbus_message_unref(pending->msg_sent);
  if (pending->dbus_pending) dbus_pending_call_unref(pending->dbus_pending);

  pending->cb           = NULL;
  pending->dbus_pending = NULL;
  eina_stringshare_del(pending->dest);
  eina_stringshare_del(pending->path);
  eina_stringshare_del(pending->interface);
  eina_stringshare_del(pending->method);
  EINA_MAGIC_SET(pending, EINA_MAGIC_NONE);
  free(pending);
}

EAPI void
efl_dbus_pending_cancel(Efl_Dbus_Pending *pending)
{
  Efl_Dbus_Message *error_message;
  EFL_DBUS_PENDING_CHECK(pending);
  EINA_SAFETY_ON_NULL_RETURN(pending->dbus_pending);

  DBG("pending=%p", pending);
  dbus_pending_call_cancel(pending->dbus_pending);

  error_message = efl_dbus_message_error_new(pending->msg_sent,
                                             EFL_DBUS_ERROR_PENDING_CANCELED,
                                             "Canceled by user.");
  efl_dbus_pending_dispatch(pending, error_message);
}

EAPI void
efl_dbus_pending_free_cb_add(Efl_Dbus_Pending *pending,
                             Efl_Dbus_Free_Cb  cb,
                             const void       *data)
{
  EFL_DBUS_PENDING_CHECK(pending);
  EINA_SAFETY_ON_NULL_RETURN(cb);
  pending->cbs_free = efl_dbus_cbs_free_add(pending->cbs_free, cb, data);
}

EAPI void
efl_dbus_pending_free_cb_del(Efl_Dbus_Pending *pending,
                             Efl_Dbus_Free_Cb  cb,
                             const void       *data)
{
  EFL_DBUS_PENDING_CHECK(pending);
  EINA_SAFETY_ON_NULL_RETURN(cb);
  pending->cbs_free = efl_dbus_cbs_free_del(pending->cbs_free, cb, data);
}

EAPI const char *
efl_dbus_pending_destination_get(const Efl_Dbus_Pending *pending)
{
  EFL_DBUS_PENDING_CHECK_RETVAL(pending, NULL);
  return pending->dest;
}

EAPI const char *
efl_dbus_pending_path_get(const Efl_Dbus_Pending *pending)
{
  EFL_DBUS_PENDING_CHECK_RETVAL(pending, NULL);
  return pending->path;
}

EAPI const char *
efl_dbus_pending_interface_get(const Efl_Dbus_Pending *pending)
{
  EFL_DBUS_PENDING_CHECK_RETVAL(pending, NULL);
  return pending->interface;
}

EAPI const char *
efl_dbus_pending_method_get(const Efl_Dbus_Pending *pending)
{
  EFL_DBUS_PENDING_CHECK_RETVAL(pending, NULL);
  return pending->method;
}
