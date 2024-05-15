#ifndef _EFL_DBUS_MODEL_PROXY_PRIVATE_H
#define _EFL_DBUS_MODEL_PROXY_PRIVATE_H

#include "Efl_Dbus_Model.h"

#include <stdbool.h>

typedef struct _Efl_Dbus_Model_Proxy_Data Efl_Dbus_Model_Proxy_Data;

/**
 * efl_dbus_model_proxy
 */
struct _Efl_Dbus_Model_Proxy_Data
{
   Eo *obj;

   const Efl_Dbus_Introspection_Interface *interface;

   Efl_Dbus_Object *object;
   Efl_Dbus_Proxy *proxy;
   Eina_Hash *properties;
   Eina_List *childrens;
   Eina_List *pendings;
   Eina_List *promises;

   Eina_Stringshare *name;

   Efl_Bool monitoring : 1;
   Efl_Bool is_listed : 1;
   Efl_Bool is_loaded : 1;
};

#endif
