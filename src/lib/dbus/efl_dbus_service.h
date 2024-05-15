#ifndef EFL_DBUS_SERVICE_H
#define EFL_DBUS_SERVICE_H 1

/**
 * @defgroup Efl_Dbus_Service Service
 * @ingroup Eldbus
 *
 * @{
 */
#define EFL_DBUS_METHOD_FLAG_DEPRECATED 1
#define EFL_DBUS_METHOD_FLAG_NOREPLY (1 << 1)
#define EFL_DBUS_METHOD_FLAG_HAS_DATA (1 << 2) // @since 1.1

#define EFL_DBUS_SIGNAL_FLAG_DEPRECATED 1

#define EFL_DBUS_PROPERTY_FLAG_DEPRECATED 1

#define EFL_DBUS_INTERFACE_DESCRIPTOR_VERSION 2

typedef struct _Efl_Dbus_Arg_Info
{
   const char *signature;
   const char *name;
} Efl_Dbus_Arg_Info;

/**
 * @brief Used to insert complete types to signature of methods or signals.
 *
 * Example: EFL_DBUS_ARGS({"s", "interface"}, {"s", "property"})
 * The signature will be "ss" and each string will have a tag name on
 * introspect XML with the respective name.
 */
#define EFL_DBUS_ARGS(args...) (const Efl_Dbus_Arg_Info[]){ args, { NULL, NULL } }

typedef struct _Efl_Dbus_Service_Interface Efl_Dbus_Service_Interface;
typedef Efl_Dbus_Message * (*Efl_Dbus_Method_Cb)(const Efl_Dbus_Service_Interface *iface, const Efl_Dbus_Message *message);

typedef Efl_Dbus_Message * (*Efl_Dbus_Method_Data_Cb)(void* data, const Efl_Dbus_Service_Interface *iface, const Efl_Dbus_Message *message); // @since 1.11

/**
 * Callback function to append property value to message.
 *
 * @param iface interface of property
 * @param propname name of property
 * @param iter variant iterator in which value must be appended
 * @param request_msg message that request property
 * @param error if a error happen you must set a message error to be send caller
 *
 * @return EFL_TRUE if success
 *
 * @note request_msg and error arguments are only different from NULL when a
 * client request a property with Properties.Get or Properties.GetAll. Upon
 * calls to efl_dbus_service_property_changed(), this callback will also be called.
 * It's a mistake to return an error in this case because if a property changed,
 * it must have a new value set and it should be able to be read.
 */
typedef Efl_Bool (*Efl_Dbus_Property_Get_Cb)(const Efl_Dbus_Service_Interface *iface, const char *propname, Efl_Dbus_Message_Iter *iter, const Efl_Dbus_Message *request_msg, Efl_Dbus_Message **error);

/**
 * Callback function to set property value from message.
 *
 * @param iface interface of property
 * @param propname name of property
 * @param input_msg message call where you have to get value
 *
 * @return Message of response, could be a simple method_return, error or NULL to send response later.
 */
typedef Efl_Dbus_Message *(*Efl_Dbus_Property_Set_Cb)(const Efl_Dbus_Service_Interface *iface, const char *propname, Efl_Dbus_Message_Iter *iter, const Efl_Dbus_Message *input_msg);

typedef struct _Efl_Dbus_Method
{
   const char *member;
   const Efl_Dbus_Arg_Info *in;
   const Efl_Dbus_Arg_Info *out;
   Efl_Dbus_Method_Cb cb;
   unsigned int flags;
} Efl_Dbus_Method;

// @since 1.11
typedef struct _Efl_Dbus_Method2
{
   Efl_Dbus_Method method;
   void* data;
} Efl_Dbus_Method2;

typedef struct _Efl_Dbus_Signal
{
   const char *name;
   const Efl_Dbus_Arg_Info *args;
   unsigned int flags;
} Efl_Dbus_Signal;

typedef struct _Efl_Dbus_Property
{
   const char *name;
   const char *type;
   Efl_Dbus_Property_Get_Cb get_func;
   Efl_Dbus_Property_Set_Cb set_func;
   unsigned int flags;
} Efl_Dbus_Property;

typedef struct _Efl_Dbus_Service_Interface_Desc
{
   const char *interface; /**< interface name */
   const Efl_Dbus_Method *methods; /**< array of the methods that should be registered in this interface, the last item of array should be filled with NULL */
   const Efl_Dbus_Signal *signals; /**< array of signal that this interface send, the last item of array should be filled with NULL */
   const Efl_Dbus_Property *properties; /**< array of property that this interface have, the last item of array should be filled with NULL  */
   const Efl_Dbus_Property_Get_Cb default_get; /**< default get function, if a property don't have a get function this will be used */
   const Efl_Dbus_Property_Set_Cb default_set; /**< default set function, if a property don't have a set function this will be used */
} Efl_Dbus_Service_Interface_Desc;

typedef struct _Efl_Dbus_Service_Interface_Desc2
{
  Efl_Dbus_Service_Interface_Desc description;
  int version; /**< version of the interface descriptor. Must be initialized with EFL_DBUS_INTERFACE_DESCRIPTOR_VERSION @since 1.11 >*/
  const Efl_Dbus_Method2 *methods2; /**< array of the methods that should be registered in this interface, the last item of array should be filled with NULL @since 1.11 */
} Efl_Dbus_Service_Interface_Desc2;

/**
 * @brief Register an interface in the given path and connection.
 *
 * @param conn where the interface should listen
 * @param path object path
 * @param desc description of interface
 *
 * @return Interface
 */
EAPI Efl_Dbus_Service_Interface *efl_dbus_service_interface_register(Efl_Dbus_Connection *conn, const char *path, const Efl_Dbus_Service_Interface_Desc *desc) EINA_ARG_NONNULL(1, 2, 3);

/**
 * @brief Register a fallback interface handler for a given subsection of the object hierarchy.
 * Note: Use efl_dbus_service_interface_unregister() to unregister a interface.
 * @param conn where the interface should listen
 * @param path a '/' delimited string of path elements
 * @param desc description of interface
 * @see efl_dbus_service_interface_unregister()
 *
 * @since 1.9
 *
 * @return Interface
 */
EAPI Efl_Dbus_Service_Interface *
efl_dbus_service_interface_fallback_register(Efl_Dbus_Connection *conn, const char *path, const Efl_Dbus_Service_Interface_Desc *desc) EINA_ARG_NONNULL(1, 2, 3);

/**
 * @brief Register an interface in the given path and connection. This
 * extended register function allows the registration of stateful methods, with void* data.
 *
 * Note: Use efl_dbus_service_interface_unregister() to unregister a interface.
 *
 * @param conn where the interface should listen
 * @param path object path
 * @param desc description of interface
 *
 * @since 1.11
 *
 * @return Interface
 */
EAPI Efl_Dbus_Service_Interface *efl_dbus_service_interface_register2(Efl_Dbus_Connection *conn, const char *path, const Efl_Dbus_Service_Interface_Desc2 *desc) EINA_ARG_NONNULL(1, 2, 3);

/**
 * @brief Register a fallback interface handler for a given subsection
 * of the object hierarchy.  This extended register function allows
 * the registration of stateful methods, with void* data.
 *
 * Note: Use efl_dbus_service_interface_unregister() to unregister a interface.
 *
 * @param conn where the interface should listen
 * @param path a '/' delimited string of path elements
 * @param desc description of interface
 * @see efl_dbus_service_interface_unregister()
 *
 * @since 1.11
 *
 * @return Interface
 */
EAPI Efl_Dbus_Service_Interface *
efl_dbus_service_interface_fallback_register2(Efl_Dbus_Connection *conn, const char *path, const Efl_Dbus_Service_Interface_Desc2 *desc) EINA_ARG_NONNULL(1, 2, 3);


/**
 * @brief Unregister a interface.
 * Note: This doesn't unregister the object path if interface count reaches 0.
 * Use efl_dbus_service_object_unregister() to unregister the object.
 *
 * @param iface interface to unregister
 */
EAPI void efl_dbus_service_interface_unregister(Efl_Dbus_Service_Interface *iface) EINA_ARG_NONNULL(1);

/**
 * @brief Unregister all interfaces of the object path that this interface belongs
 * and the object path.
 *
 * @param iface interface to unregister
 */
EAPI void efl_dbus_service_object_unregister(Efl_Dbus_Service_Interface *iface) EINA_ARG_NONNULL(1);
EAPI Efl_Dbus_Connection *efl_dbus_service_connection_get(const Efl_Dbus_Service_Interface *iface) EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;
EAPI const char *efl_dbus_service_object_path_get(const Efl_Dbus_Service_Interface *iface) EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Emit a signal handler of the interface with non-complex types.
 * Each signal handler have a internal id, the first signal handler of
 * interface is = 0 the second = 1 and go on.
 *
 * @param iface interface of the signal
 * @param signal_id id of signal
 * @param ... values that will be send on signal
 *
 * @return EFL_TRUE if success
 */
EAPI Efl_Bool efl_dbus_service_signal_emit(const Efl_Dbus_Service_Interface *iface, unsigned int signal_id, ...) EINA_ARG_NONNULL(1);

/**
 * @brief Create signal message.
 * Each signal handler have a internal id, the first signal handler of
 * interface is = 0 the second = 1 and go on.
 * This function is used when the signal has complex types.
 *
 * @param iface interface of the signal
 * @param signal_id id of signal
 *
 * @return EFL_TRUE if success
 */
EAPI Efl_Dbus_Message *efl_dbus_service_signal_new(const Efl_Dbus_Service_Interface *iface, unsigned int signal_id) EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Send a signal message.
 *
 * On success this will call efl_dbus_message_unref() on the @p signal_msg,
 * which is the intended behavior in 99% of the cases. Remember to increment
 * the refcount if you want to keep it alive.
 *
 * @param iface interface of the signal
 * @param signal_msg message of signal
 *
 * @return EFL_TRUE if success
 */
EAPI Efl_Bool efl_dbus_service_signal_send(const Efl_Dbus_Service_Interface *iface, Efl_Dbus_Message *signal_msg) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Store data at object path, this data can be obtained from all interfaces
 * of the same object.
 *
 * @param iface interface that belong to the object path where data will
 * be stored
 * @param key to identify data
 * @param data
 */
EAPI void efl_dbus_service_object_data_set(Efl_Dbus_Service_Interface *iface, const char *key, const void *data) EINA_ARG_NONNULL(1, 2, 3);

/**
 * @brief Get data stored in object path.
 *
 * @param iface interface that belongs to the object path where data are stored
 * @param key that identify data
 *
 * @return pointer to data if found otherwise NULL
 */
EAPI void *efl_dbus_service_object_data_get(const Efl_Dbus_Service_Interface *iface, const char *key) EINA_ARG_NONNULL(1, 2) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Del data stored in object path.
 *
 * @param iface interface that belongs to the object path where data are stored
 * @param key that identify data
 *
 * @return pointer to data if found otherwise NULL
 */
EAPI void *efl_dbus_service_object_data_del(Efl_Dbus_Service_Interface *iface, const char *key) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Add property to list of changed properties
 * A DBus.PropertiesChanged signal will be sent in an idler with all properties
 * that have changed.
 *
 * @param iface Interface containing the changed property
 * @param name Property name
 *
 * @return EFL_TRUE if success
 */
EAPI Efl_Bool efl_dbus_service_property_changed(const Efl_Dbus_Service_Interface *iface, const char *name) EINA_ARG_NONNULL(1, 2);

EAPI Efl_Bool efl_dbus_service_property_invalidate_set(const Efl_Dbus_Service_Interface *iface, const char *name, Efl_Bool is_invalidate) EINA_ARG_NONNULL(1, 2);

/**
 * Attach ObjectManager interface.
 *
 * @param iface ObjectManager will be attach in object path of this interface.
 * @return EFL_TRUE if success
 */
EAPI Efl_Bool efl_dbus_service_object_manager_attach(Efl_Dbus_Service_Interface *iface) EINA_ARG_NONNULL(1);

/**
 * Detach ObjectManager interface.
 *
 * @param iface ObjectManager of object path of this interface will be detach.
 * @return EFL_TRUE if success
 */
EAPI Efl_Bool efl_dbus_service_object_manager_detach(Efl_Dbus_Service_Interface *iface) EINA_ARG_NONNULL(1);

/**
 * @}
 */
#endif
