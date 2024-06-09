#ifndef EFL_DBUS_PROXY_H
#define EFL_DBUS_PROXY_H 1

/**
 * @defgroup Efl_Dbus_Proxy Proxy
 * @ingroup Eldbus
 *
 * @{
 */

/**
 * @brief Get a proxy of the following interface name in a Efl_Dbus_Object.
 *
 * @param obj The Efl_Dbus_Object on which to do the query.
 * @param interface The interface name of the proxy.
 * @return The corresponding Efl_Dbus_Proxy object.
 */
EAPI Efl_Dbus_Proxy *efl_dbus_proxy_get(Efl_Dbus_Object *obj,
                                        const char      *interface)
  EINA_ARG_NONNULL(1, 2) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Increase proxy reference.
 *
 * @param proxy The Efl_Dbus_Proxy on which to increase the refcount.
 * @return The Efl_Dbus_Proxy with an increased refcount.
 */
EAPI Efl_Dbus_Proxy *efl_dbus_proxy_ref(Efl_Dbus_Proxy *proxy)
  EINA_ARG_NONNULL(1);

/**
 * @brief Decrease proxy reference.
 * If reference == 0 proxy will be freed and all your children.
 *
 * @param proxy The Efl_Dbus_Proxy on which to decrease the refcount.
 */
EAPI void efl_dbus_proxy_unref(Efl_Dbus_Proxy *proxy) EINA_ARG_NONNULL(1);

/**
 * @brief Get the Efl_Dbus_Object associated with a proxy object.
 *
 * @param proxy The Efl_Dbus_Proxy on which to do the query.
 * @return The corresponding Efl_Dbus_Object.
 */
EAPI Efl_Dbus_Object *efl_dbus_proxy_object_get(const Efl_Dbus_Proxy *proxy)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Get the interface name associated with a proxy object.
 *
 * @param proxy The Efl_Dbus_Proxy on which to do the query.
 * @return The string corresponding to the interface name.
 */
EAPI const char *efl_dbus_proxy_interface_get(const Efl_Dbus_Proxy *proxy)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Set data to an Efl_Dbus_Proxy object.
 *
 * @param proxy The Efl_Dbus_Proxy on which to set the data.
 * @param key A string to which the data will be associated.
 * @param data The data to set to the proxy object.
 */
EAPI void efl_dbus_proxy_data_set(Efl_Dbus_Proxy *proxy,
                                  const char     *key,
                                  const void *data) EINA_ARG_NONNULL(1, 2, 3);

/**
 * @brief Get data of an Efl_Dbus_Proxy object.
 *
 * @param proxy The Efl_Dbus_Proxy on which to get the data.
 * @param key The string to which the data is associated.
 * @return The data set to the proxy object associated with the provided key.
 */
EAPI void *efl_dbus_proxy_data_get(const Efl_Dbus_Proxy *proxy, const char *key)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Delete data of an Efl_Dbus_Proxy object.
 *
 * @param proxy The Efl_Dbus_Proxy on which to delete the data.
 * @param key The string to which the data is associated.
 */
EAPI void *efl_dbus_proxy_data_del(Efl_Dbus_Proxy *proxy, const char *key)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Add a callback function to be called when an event of the
 * type passed occurs.
 *
 * @param proxy The Efl_Dbus_Proxy on which to add the callback.
 * @param cb The callback to add.
 * @param data The data to pass to the callback.
 */
EAPI void efl_dbus_proxy_free_cb_add(Efl_Dbus_Proxy  *proxy,
                                     Efl_Dbus_Free_Cb cb,
                                     const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Remove callback registered in efl_dbus_proxy_free_cb_add().
 *
 * @param proxy The Efl_Dbus_Proxy on which to delete the callback.
 * @param cb The callback to delete.
 * @param data The data passed to the callback.
 */
EAPI void efl_dbus_proxy_free_cb_del(Efl_Dbus_Proxy  *proxy,
                                     Efl_Dbus_Free_Cb cb,
                                     const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Constructs a new message to invoke a method on a remote interface.
 *
 * @param proxy The Efl_Dbus_Proxy on which to call the method.
 * @param member The name of the method to invoke.
 * @return An Efl_Dbus_Message object.
 */
EAPI Efl_Dbus_Message *efl_dbus_proxy_method_call_new(Efl_Dbus_Proxy *proxy,
                                                      const char     *member)
  EINA_ARG_NONNULL(1, 2) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Send a message.
 *
 * @param proxy the msg will be send in connection that proxy belongs
 * @param msg message that will be send
 * @param cb if msg is a method call a callback should be passed
 * @param cb_data data passed to callback
 * @param timeout timeout in milliseconds, -1 to default internal value or
 * ELDBUS_TIMEOUT_INFINITE for no timeout
 * @return A Efl_Dbus_Pending object on the sent message.
 */
EAPI Efl_Dbus_Pending *efl_dbus_proxy_send(Efl_Dbus_Proxy     *proxy,
                                           Efl_Dbus_Message   *msg,
                                           Efl_Dbus_Message_Cb cb,
                                           const void         *cb_data,
                                           double              timeout)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Send a message and block while waiting for the reply.
 *
 * @param proxy the msg will be send in connection that proxy belongs
 * @param msg message that will be send
 * @param timeout timeout in milliseconds, -1 to default internal value or
 * ELDBUS_TIMEOUT_INFINITE for no timeout
 *
 * @return The reply message, error message or NULL.
 * The returned Efl_Dbus_Message need to be unref after read.
 * @since 1.13
 */
EAPI Efl_Dbus_Message *efl_dbus_proxy_send_and_block(Efl_Dbus_Proxy   *proxy,
                                                     Efl_Dbus_Message *msg,
                                                     double            timeout)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Call a method in proxy.
 * Send a method call to interface that proxy belong with data.
 *
 * @param proxy
 * @param member method name
 * @param cb if msg is a method call a callback should be passed
 * to be execute when response arrive
 * @param cb_data data passed to callback
 * @param timeout timeout in milliseconds, -1 to default internal value or
 * ELDBUS_TIMEOUT_INFINITE for no timeout
 * @param signature of data that will be send
 * @param ... data value
 * @return A Efl_Dbus_Pending object on the sent message.
 *
 * @note This function only support basic type to complex types use
 * efl_dbus_message_iter_* functions.
 */
EAPI Efl_Dbus_Pending *efl_dbus_proxy_call(Efl_Dbus_Proxy     *proxy,
                                           const char         *member,
                                           Efl_Dbus_Message_Cb cb,
                                           const void         *cb_data,
                                           double              timeout,
                                           const char         *signature,
                                           ...) EINA_ARG_NONNULL(1, 2, 6);

/**
 * @brief Call a method in proxy.
 * Send a method call to interface that proxy belong with data.
 *
 * @param proxy
 * @param member method name
 * @param cb callback that will be called when response arrive.
 * @param cb_data data passed to callback
 * @param timeout timeout in milliseconds, -1 to default internal value or
 * ELDBUS_TIMEOUT_INFINITE for no timeout
 * @param signature of data that will be send
 * @param ap va_list of data value
 * @return A Efl_Dbus_Pending object on the sent message.
 *
 * @note This function only support basic type to complex types use
 * efl_dbus_message_iter_* functions.
 */
EAPI Efl_Dbus_Pending *efl_dbus_proxy_vcall(Efl_Dbus_Proxy     *proxy,
                                            const char         *member,
                                            Efl_Dbus_Message_Cb cb,
                                            const void         *cb_data,
                                            double              timeout,
                                            const char         *signature,
                                            va_list             ap)
  EINA_ARG_NONNULL(1, 2, 6);

/**
 * @brief Add a signal handler.
 *
 * @param proxy interface where the signal is emitted
 * @param member name of the signal
 * @param cb callback that will be called when this signal is received
 * @param cb_data data that will be passed to callback
 * @return A listener to the desired signal.
 */
EAPI Efl_Dbus_Signal_Handler *
efl_dbus_proxy_signal_handler_add(Efl_Dbus_Proxy    *proxy,
                                  const char        *member,
                                  Efl_Dbus_Signal_Cb cb,
                                  const void *cb_data) EINA_ARG_NONNULL(1, 3);

/**
 * @typedef Efl_Dbus_Proxy_Event_Type
 *
 * An enumeration containing proxy event types.
 */
typedef enum
{
  EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED = 0, /**< a property has changed */
  EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED, /**< a property was removed */
  EFL_DBUS_PROXY_EVENT_DEL,
  EFL_DBUS_PROXY_EVENT_PROPERTY_LOADED,
  EFL_DBUS_PROXY_EVENT_LAST    /**< sentinel, not a real event type */
} Efl_Dbus_Proxy_Event_Type;

/**
 * @typedef Efl_Dbus_Proxy_Event_Property_Changed
 *
 * A structure used when EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED event is received.
 */
typedef struct _Efl_Dbus_Proxy_Event_Property_Changed
{
  const char           *name; /**< the name of the property */
  const Efl_Dbus_Proxy *proxy; /**< the proxy object */
  const Eina_Value     *value; /**< the value of the property */
} Efl_Dbus_Proxy_Event_Property_Changed;

/**
 * @typedef Efl_Dbus_Proxy_Event_Property_Loaded
 *
 * A structure used when EFL_DBUS_PROXY_EVENT_PROPERTY_LOADED event is received.
 */
typedef struct _Efl_Dbus_Proxy_Event_Property_Loaded
{
  const Efl_Dbus_Proxy *proxy; /**< the proxy object */
} Efl_Dbus_Proxy_Event_Property_Loaded;

/**
 * @typedef Efl_Dbus_Proxy_Event_Property_Removed
 *
 * A structure used when EFL_DBUS_PROXY_EVENT_PROPERTY_REMOVED event is received.
 */
typedef struct _Efl_Dbus_Proxy_Event_Property_Removed
{
  const char           *interface; /**< interface name */
  const Efl_Dbus_Proxy *proxy; /**< the proxy object */
  const char           *name; /**< the name of the property */
} Efl_Dbus_Proxy_Event_Property_Removed;

/**
 * @typedef Efl_Dbus_Proxy_Event_Cb
 *
 * A callback called when an event occurs.
 * @param data The data passed by the caller.
 * @param proxy The Efl_Dbus_Proxy object.
 * @param event_info Information on the event that triggered the callback.
 */
typedef void (*Efl_Dbus_Proxy_Event_Cb)(void           *data,
                                        Efl_Dbus_Proxy *proxy,
                                        void           *event_info);

/**
 * @brief Add a callback function to be called when occurs a event of the
 * type passed.
 *
 * @param proxy The Efl_Dbus_Proxy object.
 * @param type The type of the event to register an callback on.
 * @param cb The callback function to register.
 * @param cb_data The data to pass to the callback.
 */
EAPI void efl_dbus_proxy_event_callback_add(Efl_Dbus_Proxy           *proxy,
                                            Efl_Dbus_Proxy_Event_Type type,
                                            Efl_Dbus_Proxy_Event_Cb   cb,
                                            const void               *cb_data)
  EINA_ARG_NONNULL(1, 3);

/**
 * @brief Remove callback registered in efl_dbus_proxy_event_callback_add().
 *
 * @param proxy The Efl_Dbus_Proxy object.
 * @param type The type of the event the callback was registered on.
 * @param cb The callback function to delete.
 * @param cb_data The data passed to the callback.
 */
EAPI void efl_dbus_proxy_event_callback_del(Efl_Dbus_Proxy           *proxy,
                                            Efl_Dbus_Proxy_Event_Type type,
                                            Efl_Dbus_Proxy_Event_Cb   cb,
                                            const void               *cb_data)
  EINA_ARG_NONNULL(1, 3);

/**
 * @}
 */
#endif
