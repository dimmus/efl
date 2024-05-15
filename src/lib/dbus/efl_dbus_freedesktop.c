#include "efl_dbus_private.h"
#include "efl_dbus_private_types.h"
#include <dbus/dbus.h>

EAPI Efl_Dbus_Pending *
efl_dbus_name_request(Efl_Dbus_Connection *conn, const char *name, unsigned int flags, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "RequestName", cb,
                           cb_data, -1, "su", name, flags);
}

EAPI Efl_Dbus_Pending *
efl_dbus_name_release(Efl_Dbus_Connection *conn, const char *name, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "ReleaseName", cb,
                           cb_data, -1, "s", name);
}

EAPI Efl_Dbus_Pending *
efl_dbus_name_owner_get(Efl_Dbus_Connection *conn, const char *name, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "GetNameOwner", cb,
                           cb_data, -1, "s", name);
}

EAPI Efl_Dbus_Pending *
efl_dbus_name_owner_has(Efl_Dbus_Connection *conn, const char *name, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "NameHasOwner", cb,
                           cb_data, -1, "s", name);
}

EAPI Efl_Dbus_Pending *
efl_dbus_names_list(Efl_Dbus_Connection *conn, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "ListNames", cb,
                           cb_data, -1, "");
}

EAPI Efl_Dbus_Pending *
efl_dbus_names_activatable_list(Efl_Dbus_Connection *conn, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "ListActivatableNames", cb,
                           cb_data, -1, "");
}

EAPI Efl_Dbus_Pending *
efl_dbus_name_start(Efl_Dbus_Connection *conn, const char *name, unsigned int flags, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(name, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "StartServiceByName", cb,
                           cb_data, -1, "su", name, flags);
}

EAPI Efl_Dbus_Pending *
efl_dbus_object_managed_objects_get(Efl_Dbus_Object *obj, Efl_Dbus_Message_Cb cb, const void *data)
{
   Efl_Dbus_Message *msg;
   Efl_Dbus_Pending *p;
   msg = efl_dbus_object_method_call_new(obj, EFL_DBUS_FDO_INTERFACE_OBJECT_MANAGER,
                                      "GetManagedObjects");
   p = efl_dbus_object_send(obj, msg, cb, data, -1);
   return p;
}

EAPI Efl_Dbus_Signal_Handler *
efl_dbus_object_manager_interfaces_added(Efl_Dbus_Object *obj, Efl_Dbus_Signal_Cb cb, const void *cb_data)
{
   return efl_dbus_object_signal_handler_add(obj, EFL_DBUS_FDO_INTERFACE_OBJECT_MANAGER,
                                           "InterfacesAdded", cb, cb_data);
}

EAPI Efl_Dbus_Signal_Handler *
efl_dbus_object_manager_interfaces_removed(Efl_Dbus_Object *obj, Efl_Dbus_Signal_Cb cb, const void *cb_data)
{
   return efl_dbus_object_signal_handler_add(obj, EFL_DBUS_FDO_INTERFACE_OBJECT_MANAGER,
                                           "InterfacesRemoved", cb, cb_data);
}

EAPI Efl_Dbus_Pending *
efl_dbus_hello(Efl_Dbus_Connection *conn, Efl_Dbus_Message_Cb cb, const void *cb_data)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);

   return efl_dbus_proxy_call(conn->fdo_proxy, "Hello", cb, cb_data, -1, "");
}
