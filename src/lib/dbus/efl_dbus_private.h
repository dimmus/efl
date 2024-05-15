#ifndef EFL_DBUS_PRIVATE_H
#define EFL_DBUS_PRIVATE_H

#include <dbus/dbus.h>
#include <Efl_Shared.h>
#include "eina_safety_checks.h"
#include <Efl_Core.h>
#include "Efl_Dbus.h"
#include "efl_dbus_private_types.h"

extern int _efl_dbus_log_dom;
#define DBG(...)      EINA_LOG_DOM_DBG(_efl_dbus_log_dom, __VA_ARGS__)
#define INF(...)      EINA_LOG_DOM_INFO(_efl_dbus_log_dom, __VA_ARGS__)
#define WRN(...)      EINA_LOG_DOM_WARN(_efl_dbus_log_dom, __VA_ARGS__)
#define ERR(...)      EINA_LOG_DOM_ERR(_efl_dbus_log_dom, __VA_ARGS__)
#define CRI(...)      EINA_LOG_DOM_CRIT(_efl_dbus_log_dom, __VA_ARGS__)

#define EFL_DBUS_CONNECTION_MAGIC        (0xdb050001)
#define EFL_DBUS_MESSAGE_MAGIC           (0xdb050002)
#define EFL_DBUS_SIGNAL_HANDLER_MAGIC    (0xdb050003)
#define EFL_DBUS_PENDING_MAGIC           (0xdb050004)
#define EFL_DBUS_OBJECT_MAGIC            (0xdb050005)
#define EFL_DBUS_PROXY_MAGIC             (0xdb050006)
#define EFL_DBUS_MESSAGE_ITERATOR_MAGIC  (0xdb050007)
#define EFL_DBUS_SERVICE_INTERFACE_MAGIC (0xdb050008)

void                   efl_dbus_cbs_free_dispatch(Eina_Inlist **p_lst, const void *dead_pointer);
Eina_Inlist           *efl_dbus_cbs_free_add(Eina_Inlist *lst, Efl_Dbus_Free_Cb cb, const void *data);
Eina_Inlist           *efl_dbus_cbs_free_del(Eina_Inlist *lst, Efl_Dbus_Free_Cb cb, const void *data);

void                   efl_dbus_data_set(Eina_Inlist **p_lst, const char *key, const void *data) EINA_ARG_NONNULL(1, 2, 3);
void                  *efl_dbus_data_get(Eina_Inlist **p_lst, const char *key) EINA_ARG_NONNULL(1, 2);
void                  *efl_dbus_data_del(Eina_Inlist **p_lst, const char *key) EINA_ARG_NONNULL(1, 2);
void                   efl_dbus_data_del_all(Eina_Inlist **p_list) EINA_ARG_NONNULL(1);

Efl_Bool              efl_dbus_message_init(void);
void                   efl_dbus_message_shutdown(void);
Efl_Dbus_Message         *efl_dbus_message_new(Efl_Bool writable);

Efl_Bool              efl_dbus_signal_handler_init(void);
void                   efl_dbus_signal_handler_shutdown(void);

Efl_Bool              efl_dbus_pending_init(void);
void                   efl_dbus_pending_shutdown(void);

Efl_Bool              efl_dbus_object_init(void);
void                   efl_dbus_object_shutdown(void);

Efl_Bool              efl_dbus_proxy_init(void);
void                   efl_dbus_proxy_shutdown(void);

Efl_Bool              efl_dbus_service_init(void);
void                   efl_dbus_service_shutdown(void);

void                   efl_dbus_connection_event_callback_call(Efl_Dbus_Connection *conn, Efl_Dbus_Connection_Event_Type type, const void *event_info) EINA_ARG_NONNULL(1);

Efl_Bool              efl_dbus_object_proxy_del(Efl_Dbus_Object *obj, Efl_Dbus_Proxy *proxy, const char *interface) EINA_ARG_NONNULL(1, 2);

void                   efl_dbus_connection_signal_handler_add(Efl_Dbus_Connection *conn, Efl_Dbus_Signal_Handler *handler) EINA_ARG_NONNULL(1, 2);
void                   efl_dbus_connection_pending_add(Efl_Dbus_Connection *conn, Efl_Dbus_Pending *pending) EINA_ARG_NONNULL(1, 2);
void                   efl_dbus_connection_signal_handler_del(Efl_Dbus_Connection *conn, Efl_Dbus_Signal_Handler *handler) EINA_ARG_NONNULL(1, 2);
void                   efl_dbus_connection_signal_handler_del(Efl_Dbus_Connection *conn, Efl_Dbus_Signal_Handler *handler) EINA_ARG_NONNULL(1, 2);
void                   efl_dbus_connection_pending_del(Efl_Dbus_Connection *conn, Efl_Dbus_Pending *pending) EINA_ARG_NONNULL(1, 2);

Efl_Dbus_Object          *efl_dbus_connection_name_object_get(Efl_Dbus_Connection *conn, const char *name, const char *path);
void                   efl_dbus_connection_name_object_set(Efl_Dbus_Connection *conn, Efl_Dbus_Object *obj);

Efl_Bool              efl_dbus_object_proxy_add(Efl_Dbus_Object *obj, Efl_Dbus_Proxy *proxy) EINA_ARG_NONNULL(1, 2);
Efl_Dbus_Proxy           *efl_dbus_object_proxy_get(Efl_Dbus_Object *obj, const char *interface);

void                   efl_dbus_connection_name_object_del(Efl_Dbus_Connection *conn, const Efl_Dbus_Object *obj);
Efl_Dbus_Connection_Name *efl_dbus_connection_name_get(Efl_Dbus_Connection *conn, const char *name);
void                   efl_dbus_connection_name_owner_monitor(Efl_Dbus_Connection *conn, Efl_Dbus_Connection_Name *cn, Efl_Bool enable);

Efl_Dbus_Pending         *_efl_dbus_connection_send(Efl_Dbus_Connection *conn, Efl_Dbus_Message *msg, Efl_Dbus_Message_Cb cb, const void *cb_data, double timeout);
Efl_Dbus_Message         *_efl_dbus_connection_send_and_block(Efl_Dbus_Connection *conn, Efl_Dbus_Message *msg, double timeout);

Efl_Dbus_Message_Iter    *efl_dbus_message_iter_sub_iter_get(Efl_Dbus_Message_Iter *iter);
Eina_Value             *_message_iter_struct_to_eina_value(Efl_Dbus_Message_Iter *iter);
Efl_Bool              _message_iter_from_eina_value(const char *signature, Efl_Dbus_Message_Iter *iter, const Eina_Value *value);
Efl_Bool              _message_iter_from_eina_value_struct(const char *signature, Efl_Dbus_Message_Iter *iter, const Eina_Value *value);

void                   efl_dbus_connection_name_ref(Efl_Dbus_Connection_Name *cn);
void                   efl_dbus_connection_name_unref(Efl_Dbus_Connection *conn, Efl_Dbus_Connection_Name *cn);
Efl_Dbus_Signal_Handler  *_efl_dbus_signal_handler_add(Efl_Dbus_Connection *conn, const char *sender, const char *path, const char *interface, const char *member, Efl_Dbus_Signal_Cb cb, const void *cb_data);
Efl_Dbus_Message         *efl_dbus_message_signal_new(const char *path, const char *interface, const char *name) EINA_ARG_NONNULL(1, 2, 3) EFL_WARN_UNUSED_RESULT;

const Eina_Value_Type  *_dbus_type_to_eina_value_type(char type);

#endif
