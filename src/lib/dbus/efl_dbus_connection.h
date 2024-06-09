#ifndef EFL_DBUS_CONNECTION_H
#define EFL_DBUS_CONNECTION_H 1

/**
 * @defgroup Efl_Dbus_Connection Connection
 * @ingroup Eldbus
 *
 * @{
 */

/**
 * @typedef Efl_Dbus_Connection_Type
 * Enum of several Eldbus connection types.
 */
typedef enum
{
  EFL_DBUS_CONNECTION_TYPE_UNKNOWN = 0, /**< sentinel, not a real type */
  EFL_DBUS_CONNECTION_TYPE_SESSION,
  EFL_DBUS_CONNECTION_TYPE_SYSTEM,
  EFL_DBUS_CONNECTION_TYPE_STARTER,
  EFL_DBUS_CONNECTION_TYPE_ADDRESS,
  EFL_DBUS_CONNECTION_TYPE_LAST /**< sentinel, not a real type */
} Efl_Dbus_Connection_Type;

#define EFL_DBUS_TIMEOUT_INFINITE ((int) 0x7fffffff) /**< Infinite timeout definition */

/**
 * Establish a connection to bus and integrate it with the ecore main
 * loop. If a connection of given type was already created before, its
 * reference counter is incremented and the connection is returned.
 *
 * @param type type of connection e.g EFL_DBUS_CONNECTION_TYPE_SESSION,
 * EFL_DBUS_CONNECTION_TYPE_SYSTEM or EFL_DBUS_CONNECTION_TYPE_STARTER
 *
 * @return connection with bus
 */
EAPI Efl_Dbus_Connection *
efl_dbus_connection_get(Efl_Dbus_Connection_Type type);

/**
 * Always create and establish a new connection to bus and integrate it with
 * the ecore main loop. Differently from efl_dbus_connection_get(), this function
 * guarantees to create a new connection to the D-Bus daemon and the connection
 * is not shared by any means.
 *
 * @param type type of connection e.g EFL_DBUS_CONNECTION_TYPE_SESSION,
 * EFL_DBUS_CONNECTION_TYPE_SYSTEM or EFL_DBUS_CONNECTION_TYPE_STARTER
 *
 * @return connection with bus
 */
EAPI Efl_Dbus_Connection *
efl_dbus_private_connection_get(Efl_Dbus_Connection_Type type);

/**
 * Establish a connection to bus and integrate it with the ecore main
 * loop. If a connection of given type was already created before, its
 * reference counter is incremented and the connection is returned.
 *
 * @param address the address which will be passed to dbus_connection_open()
 *
 * @return connection with bus
 */
EAPI Efl_Dbus_Connection *efl_dbus_address_connection_get(const char *address)
  EINA_ARG_NONNULL(1);

/**
 * Always create and establish a new connection to bus and integrate it with
 * the ecore main loop. Differently from efl_dbus_connection_get(), this function
 * guarantees to create a new connection to the D-Bus daemon and the connection
 * is not shared by any means.
 *
 * @param address the address which will be passed to dbus_connection_open_private()
 *
 * @return connection with bus
 */
EAPI Efl_Dbus_Connection *
efl_dbus_private_address_connection_get(const char *address)
  EINA_ARG_NONNULL(1);

/**
 * @brief Increment connection reference count.
 *
 * @param conn The given Efl_Dbus_Connection object to reference.
 * @return The Efl_Dbus_Connection object given as parameter.
 */
EAPI Efl_Dbus_Connection *efl_dbus_connection_ref(Efl_Dbus_Connection *conn)
  EINA_ARG_NONNULL(1);

/**
 * @brief Decrement connection reference count.
 *
 * If reference count reaches 0, the connection to bus will be dropped and all
 * its children will be invalidated.
 *
 * @param conn The given Efl_Dbus_Connection object to unreference.
 */
EAPI void efl_dbus_connection_unref(Efl_Dbus_Connection *conn)
  EINA_ARG_NONNULL(1);

/**
 * @brief Add a callback function to be called when connection is freed
 *
 * @param conn The connection object to add the callback to.
 * @param cb The callback to be called when the connection is free.
 * @param data The data passed to the callback.
 */
EAPI void efl_dbus_connection_free_cb_add(Efl_Dbus_Connection *conn,
                                          Efl_Dbus_Free_Cb     cb,
                                          const void          *data)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Remove callback registered in efl_dbus_connection_free_cb_add().
 *
 * @param conn The connection object to delete the callback from.
 * @param cb The callback that was called when the connection was free.
 * @param data The data that was passed to the callback.
 */
EAPI void efl_dbus_connection_free_cb_del(Efl_Dbus_Connection *conn,
                                          Efl_Dbus_Free_Cb     cb,
                                          const void          *data)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Set an attached data pointer to an object with a given string key.
 *
 * @param conn The connection object to store data to
 * @param key to identify data
 * @param data data that will be stored
 */
EAPI void efl_dbus_connection_data_set(Efl_Dbus_Connection *conn,
                                       const char          *key,
                                       const void          *data)
  EINA_ARG_NONNULL(1, 2, 3);

/**
 * @brief Get data stored in connection.
 *
 * @param conn connection where data is stored
 * @param key key that identifies data
 *
 * @return pointer to data if found otherwise NULL
 */
EAPI void *efl_dbus_connection_data_get(const Efl_Dbus_Connection *conn,
                                        const char *key) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Del data stored in connection.
 *
 * @param conn connection where data is stored
 * @param key that identifies data
 *
 * @return pointer to data if found otherwise NULL
 */
EAPI void *efl_dbus_connection_data_del(Efl_Dbus_Connection *conn,
                                        const char *key) EINA_ARG_NONNULL(1, 2);

/**
 * @typedef Efl_Dbus_Connection_Event_Type
 * Enum of several Eldbus connection events.
 */
typedef enum
{
  EFL_DBUS_CONNECTION_EVENT_DEL,
  EFL_DBUS_CONNECTION_EVENT_DISCONNECTED,
  EFL_DBUS_CONNECTION_EVENT_LAST /**< sentinel, not a real event type */
} Efl_Dbus_Connection_Event_Type;

/**
 * @typedef Efl_Dbus_Connection_Event_Cb
 *
 * Callback called when we receive an event.
 */
typedef void (*Efl_Dbus_Connection_Event_Cb)(void                *data,
                                             Efl_Dbus_Connection *conn,
                                             void                *event_info);

/**
 * @brief Add a callback function to be called when an event occurs of the
 * type passed.
 *
 * @param conn The connection object to add the callback to.
 * @param type The type of event that will trigger the callback.
 * @param cb The callback to be called when the event is triggered.
 * @param cb_data The data passed to the callback.
 */
EAPI void
efl_dbus_connection_event_callback_add(Efl_Dbus_Connection           *conn,
                                       Efl_Dbus_Connection_Event_Type type,
                                       Efl_Dbus_Connection_Event_Cb   cb,
                                       const void                    *cb_data)
  EINA_ARG_NONNULL(1, 3);

/**
 * @brief Remove callback registered in efl_dbus_connection_event_callback_add().
 *
 * @param conn The connection object to delete the callback from.
 * @param type The type of event that was triggered the callback.
 * @param cb The callback that was called when the event was triggered.
 * @param cb_data The data that was passed to the callback.
 */
EAPI void
efl_dbus_connection_event_callback_del(Efl_Dbus_Connection           *conn,
                                       Efl_Dbus_Connection_Event_Type type,
                                       Efl_Dbus_Connection_Event_Cb   cb,
                                       const void                    *cb_data)
  EINA_ARG_NONNULL(1, 3);

/**
 * @brief Send a message.
 *
 * @param conn the connection where the message will be sent
 * @param msg message that will be sent
 * @param cb if msg is a method call a callback should be passed
 * to be executed when a response arrives
 * @param cb_data data passed to callback
 * @param timeout timeout in milliseconds, -1 to use default internal value or
 * ELDBUS_TIMEOUT_INFINITE for no timeout
 * @return A Efl_Dbus_Pending struct.
 */
EAPI Efl_Dbus_Pending *efl_dbus_connection_send(Efl_Dbus_Connection *conn,
                                                Efl_Dbus_Message    *msg,
                                                Efl_Dbus_Message_Cb  cb,
                                                const void          *cb_data,
                                                double               timeout)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Gets unique name assigned by the message bus.
 *
 * @param conn connection object to get unique name from.
 *
 * @return pointer to unique name string or NULL or error. Returned value
 * remains valid until connection is free.
 */
EAPI const char *efl_dbus_connection_unique_name_get(Efl_Dbus_Connection *conn)
  EINA_ARG_NONNULL(1);
/**
 * @}
 */
#endif
