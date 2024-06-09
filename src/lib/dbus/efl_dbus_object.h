#ifndef EFL_DBUS_OBJECT_H
#define EFL_DBUS_OBJECT_H 1

/**
 * @defgroup Efl_Dbus_Object_Mapper Object Mapper
 * @ingroup Eldbus
 *
 * @{
 */

/**
 * @brief Get an object of the given bus and path.
 *
 * @param conn connection where object belongs
 * @param bus name of bus or unique-id of who listens for calls of this object
 * @param path object path of this object
 * @return The corresponding Efl_Dbus_Object.
 */
EAPI Efl_Dbus_Object *efl_dbus_object_get(Efl_Dbus_Connection *conn,
                                          const char          *bus,
                                          const char          *path)
  EINA_ARG_NONNULL(1, 2, 3) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Increase object reference.
 *
 * @param obj An Efl_Dbus_Object.
 * @return The same Efl_Dbus_Object with an increased refcount.
 */
EAPI Efl_Dbus_Object *efl_dbus_object_ref(Efl_Dbus_Object *obj)
  EINA_ARG_NONNULL(1);

/**
 * @brief Decrease object reference.
 * If reference == 0 object will be freed and all its children.
 *
 * @param obj An Efl_Dbus_Object.
 */
EAPI void efl_dbus_object_unref(Efl_Dbus_Object *obj) EINA_ARG_NONNULL(1);

/**
 * @brief Add a callback function to be called when object will be freed.
 *
 * @param obj object that you want to monitor
 * @param cb callback that will be executed
 * @param data passed to callback
 */
EAPI void efl_dbus_object_free_cb_add(Efl_Dbus_Object *obj,
                                      Efl_Dbus_Free_Cb cb,
                                      const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Remove callback registered in efl_dbus_object_free_cb_add().
 *
 * @param obj Object monitored.
 * @param cb Callback that was registered.
 * @param data Data that was passed to callback.
 */
EAPI void efl_dbus_object_free_cb_del(Efl_Dbus_Object *obj,
                                      Efl_Dbus_Free_Cb cb,
                                      const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * @typedef Efl_Dbus_Object_Event_Type
 *
 * An enumeration containing several Efl_Dbus_Object event types.
 */
typedef enum
{
  EFL_DBUS_OBJECT_EVENT_IFACE_ADDED =
    0, /**< a parent path must have a ObjectManager interface */
  EFL_DBUS_OBJECT_EVENT_IFACE_REMOVED, /**< a parent path must have a ObjectManager interface */
  EFL_DBUS_OBJECT_EVENT_PROPERTY_CHANGED, /**< a property has changes */
  EFL_DBUS_OBJECT_EVENT_PROPERTY_REMOVED, /**< a property was removed */
  EFL_DBUS_OBJECT_EVENT_DEL,
  EFL_DBUS_OBJECT_EVENT_LAST /**< sentinel, not a real event type */
} Efl_Dbus_Object_Event_Type;

/**
 * @typedef Efl_Dbus_Object_Event_Interface_Added
 *
 * Structure used with the EFL_DBUS_OBJECT_EVENT_IFACE_ADDED event.
 */
typedef struct _Efl_Dbus_Object_Event_Interface_Added
{
  const char     *interface; /**< The interface name */
  Efl_Dbus_Proxy *proxy; /**< The proxy object */
} Efl_Dbus_Object_Event_Interface_Added;

/**
 * @typedef Efl_Dbus_Object_Event_Interface_Removed
 *
 * Structure used with the EFL_DBUS_OBJECT_EVENT_IFACE_REMOVED event.
 */
typedef struct _Efl_Dbus_Object_Event_Interface_Removed
{
  const char *interface; /**< The interface name */
} Efl_Dbus_Object_Event_Interface_Removed;

/**
 * @typedef Efl_Dbus_Object_Event_Property_Changed
 *
 * Structure used with the EFL_DBUS_OBJECT_EVENT_PROPERTY_CHANGED event.
 */
typedef struct _Efl_Dbus_Object_Event_Property_Changed
{
  const char       *interface; /**< The interface name */
  Efl_Dbus_Proxy   *proxy; /**< The proxy object */
  const char       *name; /**< The name of the property */
  const Eina_Value *value; /**< The value of the property */
} Efl_Dbus_Object_Event_Property_Changed;

/**
 * @typedef Efl_Dbus_Object_Event_Property_Removed
 *
 * Structure used with the EFL_DBUS_OBJECT_EVENT_PROPERTY_REMOVED event.
 */
typedef struct _Efl_Dbus_Object_Event_Property_Removed
{
  const char     *interface; /**< The interface name */
  Efl_Dbus_Proxy *proxy; /**< The proxy object */
  const char     *name; /**< The name of the property */
} Efl_Dbus_Object_Event_Property_Removed;

/**
 * @typedef Efl_Dbus_Object_Event_Cb
 *
 * Callback that will be called when an Efl_Dbus_Object event happens.
 *
 * @param data Context data.
 * @param obj The Efl_Dbus_Object.
 * @param event_info Information about the event that triggered the callback.
 */
typedef void (*Efl_Dbus_Object_Event_Cb)(void            *data,
                                         Efl_Dbus_Object *obj,
                                         void            *event_info);

/**
 * @brief Add a callback function to be called when an event of the specified
 * type occurs.
 *
 * @param obj The Efl_Dbus_Object on which to register a callback.
 * @param type The type of the event.
 * @param cb The callback to call.
 * @param cb_data The data to pass to the callback.
 */
EAPI void efl_dbus_object_event_callback_add(Efl_Dbus_Object           *obj,
                                             Efl_Dbus_Object_Event_Type type,
                                             Efl_Dbus_Object_Event_Cb   cb,
                                             const void                *cb_data)
  EINA_ARG_NONNULL(1, 3);

/**
 * @brief Remove callback registered in efl_dbus_object_event_callback_add().
 *
 * @param obj The Efl_Dbus_Object.
 * @param type The type of the event.
 * @param cb The callback to call.
 * @param cb_data The data to pass to the callback.
 */
EAPI void efl_dbus_object_event_callback_del(Efl_Dbus_Object           *obj,
                                             Efl_Dbus_Object_Event_Type type,
                                             Efl_Dbus_Object_Event_Cb   cb,
                                             const void                *cb_data)
  EINA_ARG_NONNULL(1, 3);

/**
 * Get the Efl_Dbus_Connection object associated with a Efl_Dbus_Object.
 *
 * @param obj The Efl_Dbus_Object.
 * @return The corresponding Efl_Dbus_Connection object.
 */
EAPI Efl_Dbus_Connection *
efl_dbus_object_connection_get(const Efl_Dbus_Object *obj)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Get the name associated with a Efl_Dbus_Object.
 *
 * @param obj The Efl_Dbus_Object.
 * @return A string corresponding to the Efl_Dbus_Object name.
 */
EAPI const char *efl_dbus_object_bus_name_get(const Efl_Dbus_Object *obj)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Get the path associated with a Efl_Dbus_Object.
 *
 * @param obj The Efl_Dbus_Object.
 * @return A string corresponding to the Efl_Dbus_Object path.
 */
EAPI const char *efl_dbus_object_path_get(const Efl_Dbus_Object *obj)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Send a message.
 *
 * @param obj the msg will be sent in connection to this object
 * @param msg message that will be sent
 * @param cb if msg is a method call a callback should be passed
 * to be executed when a response arrives
 * @param cb_data data passed to callback
 * @param timeout timeout in milliseconds, -1 to default internal value or
 * ELDBUS_TIMEOUT_INFINITE for no timeout
 * @return A Efl_Dbus_Pending object.
 */
EAPI Efl_Dbus_Pending *efl_dbus_object_send(Efl_Dbus_Object    *obj,
                                            Efl_Dbus_Message   *msg,
                                            Efl_Dbus_Message_Cb cb,
                                            const void         *cb_data,
                                            double              timeout)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Add a signal handler.
 *
 * @param obj where the signal is emitted
 * @param interface of the signal
 * @param member name of the signal
 * @param cb callback that will be called when this signal is received
 * @param cb_data data that will be passed to callback
 * @return A listener to the desired signal.
 */
EAPI Efl_Dbus_Signal_Handler *
efl_dbus_object_signal_handler_add(Efl_Dbus_Object   *obj,
                                   const char        *interface,
                                   const char        *member,
                                   Efl_Dbus_Signal_Cb cb,
                                   const void *cb_data) EINA_ARG_NONNULL(1, 4);

/**
 * @brief Call a dbus method on the Efl_Dbus_Object.
 *
 * @param obj The Efl_Dbus_Object on which to call the method.
 * @param interface Interface name.
 * @param member Name of the method to be called.
 *
 * @return a new Efl_Dbus_Message, free with efl_dbus_message_unref()
 */
EAPI Efl_Dbus_Message *efl_dbus_object_method_call_new(Efl_Dbus_Object *obj,
                                                       const char *interface,
                                                       const char *member)
  EINA_ARG_NONNULL(1, 2, 3) EFL_WARN_UNUSED_RESULT;

/**
 * @}
 */
#endif
