#ifndef EFL_DBUS_FREEDESKTOP_H
#define EFL_DBUS_FREEDESKTOP_H 1

/**
 * @defgroup Efl_Dbus_Basic Basic FreeDesktop.Org Methods
 * @ingroup Eldbus
 *
 * @{
 */
#define EFL_DBUS_NAME_REQUEST_FLAG_ALLOW_REPLACEMENT 0x1 /**< Allow another service to become the primary owner if requested */
#define EFL_DBUS_NAME_REQUEST_FLAG_REPLACE_EXISTING  0x2 /**< Request to replace the current primary owner */
#define EFL_DBUS_NAME_REQUEST_FLAG_DO_NOT_QUEUE      0x4 /**< If we can not become the primary owner do not place us in the queue */

/* Replies to request for a name */
#define EFL_DBUS_NAME_REQUEST_REPLY_PRIMARY_OWNER    1 /**< Service has become the primary owner of the requested name */
#define EFL_DBUS_NAME_REQUEST_REPLY_IN_QUEUE         2 /**< Service could not become the primary owner and has been placed in the queue */
#define EFL_DBUS_NAME_REQUEST_REPLY_EXISTS           3 /**< Service is already in the queue */
#define EFL_DBUS_NAME_REQUEST_REPLY_ALREADY_OWNER    4 /**< Service is already the primary owner */

/**
 * Send a "RequestName" method call in proxy.
 *
 * @param conn Connection object.
 * @param bus Name of the bus
 * @param flags Parameter of the "RequestName" method.
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending *efl_dbus_name_request(Efl_Dbus_Connection *conn, const char *bus, unsigned int flags, Efl_Dbus_Message_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1, 2);

/* Replies to releasing a name */
#define EFL_DBUS_NAME_RELEASE_REPLY_RELEASED     1    /**< Service was released from the given name */
#define EFL_DBUS_NAME_RELEASE_REPLY_NON_EXISTENT 2    /**< The given name does not exist on the bus */
#define EFL_DBUS_NAME_RELEASE_REPLY_NOT_OWNER    3    /**< Service is not an owner of the given name */

/**
 * Send a "ReleaseName" method call in proxy.
 *
 * @param conn Connection object.
 * @param bus Name of the bus
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending *efl_dbus_name_release(Efl_Dbus_Connection *conn, const char *bus, Efl_Dbus_Message_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1, 2);

/**
 * Send a "GetNameOwner" method call in proxy.
 *
 * @param conn Connection object.
 * @param bus Name of the bus
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending *efl_dbus_name_owner_get(Efl_Dbus_Connection *conn, const char *bus, Efl_Dbus_Message_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1, 2);

/**
 * Send a "NameHasOwner" method call in proxy.
 *
 * @param conn Connection object.
 * @param bus Name of the bus
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending *efl_dbus_name_owner_has(Efl_Dbus_Connection *conn, const char *bus, Efl_Dbus_Message_Cb cb, const void *cb_data);

/**
 * Send a "ListNames" method call in proxy.
 *
 * @param conn Connection object.
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending *efl_dbus_names_list(Efl_Dbus_Connection *conn, Efl_Dbus_Message_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1);

/**
 * Send a "ListActivatableNames" method call in proxy.
 *
 * @param conn Connection object.
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending *efl_dbus_names_activatable_list(Efl_Dbus_Connection *conn, Efl_Dbus_Message_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1);

/**
 * Send a "Hello" method call in proxy.
 *
 * @param conn Connection object.
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending *efl_dbus_hello(Efl_Dbus_Connection *conn, Efl_Dbus_Message_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1);

/* Replies to service starts */
#define EFL_DBUS_NAME_START_REPLY_SUCCESS         1 /**< Service was auto started */
#define EFL_DBUS_NAME_START_REPLY_ALREADY_RUNNING 2 /**< Service was already running */

/**
 * Send a "StartServiceByName" method call in proxy.
 *
 * @param conn Connection object.
 * @param bus Name of the bus.
 * @param flags Parameter of the "StartServiceByName" method.
 * @param cb Callback to call when receiving answer.
 * @param cb_data Data passed to callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_name_start(Efl_Dbus_Connection *conn, const char *bus, unsigned int flags, Efl_Dbus_Message_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1, 2);

/**
 * @typedef Efl_Dbus_Name_Owner_Changed_Cb
 *
 * Callback called when unique id of a bus name changed.
 */
typedef void (*Efl_Dbus_Name_Owner_Changed_Cb)(void *data, const char *bus, const char *old_id, const char *new_id);

/**
 * Add a callback to be called when unique id of a bus name changed.
 *
 * This function implicitly calls efl_dbus_name_owner_get() in order to be able to
 * monitor the name. If the only interest is to receive notifications when the
 * name in fact changes, pass EFL_FALSE to \p allow_initial_call so your
 * callback will not be called on first retrieval of name owner. If the
 * initial state is important, pass EFL_TRUE to this parameter.
 *
 * @param conn connection
 * @param bus name of bus
 * @param cb callback
 * @param cb_data context data
 * @param allow_initial_call allow call callback with actual id of the bus
 */
EAPI void                  efl_dbus_name_owner_changed_callback_add(Efl_Dbus_Connection *conn, const char *bus, Efl_Dbus_Name_Owner_Changed_Cb cb, const void *cb_data, Efl_Bool allow_initial_call) EINA_ARG_NONNULL(1, 2, 3);
/**
 * Remove callback added with efl_dbus_name_owner_changed_callback_add().
 *
 * @param conn connection
 * @param bus name of bus
 * @param cb callback
 * @param cb_data context data
 */
EAPI void                  efl_dbus_name_owner_changed_callback_del(Efl_Dbus_Connection *conn, const char *bus, Efl_Dbus_Name_Owner_Changed_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1, 2, 3);

/**
 * @defgroup Efl_Dbus_FDO_Peer org.freedesktop.DBus.Peer
 *
 * @{
 */

/**
 * Call the method "Ping" on the eldbus object.
 *
 * @param obj Eldbus object.
 * @param cb Callback called when receiving an answer.
 * @param data Data passed to the callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_object_peer_ping(Efl_Dbus_Object *obj, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * Call the method "GetMachineId" on the eldbus object.
 *
 * @param obj Eldbus object.
 * @param cb Callback called when receiving an answer.
 * @param data Data passed to the callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_object_peer_machine_id_get(Efl_Dbus_Object *obj, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * @}
 */

/**
 * @defgroup Efl_Dbus_FDO_Introspectable org.freedesktop.DBus.Introspectable
 *
 * @{
 */

/**
 * Call the method "Introspect" on the eldbus object.
 *
 * @param obj Eldbus object.
 * @param cb Callback called when receiving an answer.
 * @param data Data passed to the callback.
 * @return The Efl_Dbus_Pending corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_object_introspect(Efl_Dbus_Object *obj, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2);
/**
 * @}
 */

/**
 * @defgroup Efl_Dbus_FDO_Properties org.freedesktop.DBus.Properties
 * @{
 */

/**
 * Enable or disable local cache of properties.
 *
 * After enable you can call efl_dbus_proxy_property_local_get() or
 * efl_dbus_proxy_property_local_get_all() to get cached properties.
 *
 * @param proxy bus+path+interface that the properties belong
 * @param enable enable or disable properties monitor
 * @return EFL_TRUE if already have cached properties
 * EFL_FALSE if it will asynchrony get the properties.
 * You should listen for a EFL_DBUS_PROXY_EVENT_PROPERTY_LOADED
 * to know when properties finish to load.
 */
EAPI Efl_Bool efl_dbus_proxy_properties_monitor(Efl_Dbus_Proxy *proxy, Efl_Bool enable) EINA_ARG_NONNULL(1);

/**
 * Get a property.
 *
 * @param proxy The proxy object on which to do the query.
 * @param name The property name to get.
 * @param cb The callback to be called when receiving an answer.
 * @param data Data to be passed to the callback.
 * @return Efl_Dbus_Pending object corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_proxy_property_get(Efl_Dbus_Proxy *proxy, const char *name, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2, 3);

/**
 * Set a property.
 *
 * @param proxy The proxy object on which to do the query.
 * @param name The property name to get.
 * @param sig
 * @param value The value to set.
 * @param cb The callback to be called when receiving an answer.
 * @param data Data to be passed to the callback.
 * @return Efl_Dbus_Pending object corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_proxy_property_set(Efl_Dbus_Proxy *proxy, const char *name, const char *sig, const void *value, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2, 3, 4);

/**
 * Set a property with a Eina_Value.
 *
 * @param proxy The proxy object on which to do the query.
 * @param name The property name to get.
 * @param sig
 * @param value The value to set.
 * @param cb The callback to be called when receiving an answer.
 * @param data Data to be passed to the callback.
 * @return Efl_Dbus_Pending object corresponding to the message sent.
 *
 * @since 1.17
 */
EAPI Efl_Dbus_Pending        *efl_dbus_proxy_property_value_set(Efl_Dbus_Proxy *proxy, const char *name, const char *sig, const Eina_Value *value, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2, 3, 4);

/**
 * Get all properties.
 *
 * @param proxy The proxy object on which to do the query.
 * @param cb The callback to be called when receiving an answer.
 * @param data Data to be passed to the callback.
 * @return Efl_Dbus_Pending object corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_proxy_property_get_all(Efl_Dbus_Proxy *proxy, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * Register a callback on "PropertiesChanged" signal.
 *
 * @param proxy The proxy object on which to register the callback.
 * @param cb The callback to be called when receiving the signal.
 * @param data Data to be passed to the callback.
 * @return Efl_Dbus_Signal_Handler object representing a listener for "PropertiesChanged" signal.
 */
EAPI Efl_Dbus_Signal_Handler *efl_dbus_proxy_properties_changed_callback_add(Efl_Dbus_Proxy *proxy, Efl_Dbus_Signal_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * Return the cached value of property.
 *
 * This only work if you have enable efl_dbus_proxy_properties_monitor or
 * if you have call efl_dbus_proxy_event_callback_add of type
 * EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED and the property you want had changed.
 *
 * @param proxy The proxy object on which to do the query.
 * @param name The property name to get.
 * @return Cached value of property.
 */
EAPI Eina_Value           *efl_dbus_proxy_property_local_get(Efl_Dbus_Proxy *proxy, const char *name) EINA_ARG_NONNULL(1, 2);

/**
 * Get all cached properties.
 *
 * This only work if you have enable efl_dbus_proxy_properties_monitor or
 * if you have call efl_dbus_proxy_event_callback_add of type
 * EFL_DBUS_PROXY_EVENT_PROPERTY_CHANGED.
 *
 * @param proxy The proxy object on which to do the query.
 * @return A Eina_Hash with all cached properties
 */
EAPI const Eina_Hash      *efl_dbus_proxy_property_local_get_all(Efl_Dbus_Proxy *proxy) EINA_ARG_NONNULL(1);

/**
 * @}
 */

/**
 * @defgroup Efl_Dbus_FDO_ObjectManager org.freedesktop.DBus.ObjectManager *
 * @{
 */

/**
 * Call the method "GetManagedObjects" on the eldbus object.
 *
 * @param obj A Eldbus object.
 * @param cb The callback to call when receiving an answer.
 * @param data The data to pass to the callback.
 *
 * @return A Efl_Dbus_Pending object corresponding to the message sent.
 */
EAPI Efl_Dbus_Pending        *efl_dbus_object_managed_objects_get(Efl_Dbus_Object *obj, Efl_Dbus_Message_Cb cb, const void *data) EINA_ARG_NONNULL(1, 2);

/**
 * Register a callback on "InterfacesAdded" signal.
 *
 * @param obj The Eldbus object on which to register the callback.
 * @param cb The callback to be called when receiving the signal.
 * @param cb_data Data to be passed to the callback.
 * @return Efl_Dbus_Signal_Handler object representing a listener for "InterfacesAdded" signal.
 */
EAPI Efl_Dbus_Signal_Handler *efl_dbus_object_manager_interfaces_added(Efl_Dbus_Object *obj, Efl_Dbus_Signal_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1);

/**
 * Register a callback on "InterfacesRemoved" signal.
 *
 * @param obj The Eldbus object on which to register the callback.
 * @param cb The callback to be called when receiving the signal.
 * @param cb_data Data to be passed to the callback.
 * @return Efl_Dbus_Signal_Handler object representing a listener for "InterfacesRemoved" signal.
 */
EAPI Efl_Dbus_Signal_Handler *efl_dbus_object_manager_interfaces_removed(Efl_Dbus_Object *obj, Efl_Dbus_Signal_Cb cb, const void *cb_data) EINA_ARG_NONNULL(1);

/**
 * @}
 */

/**
 * @}
 */
#endif
