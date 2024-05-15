#ifndef _EFL_DBUS_MODEL_CONNECTION_PRIVATE_H
#define _EFL_DBUS_MODEL_CONNECTION_PRIVATE_H

#include "Efl_Dbus_Model.h"

#include <stdbool.h>

typedef struct _Efl_Dbus_Model_Connection_Data Efl_Dbus_Model_Connection_Data;

/**
 * efl_dbus_model_connection
 */
struct _Efl_Dbus_Model_Connection_Data
{
   Eo *obj;

   Efl_Dbus_Connection *connection;
   Efl_Dbus_Pending *pending;
   Efl_Dbus_Connection_Type type;

   Eina_List *childrens;
   Eina_List *requests;

   Eina_Stringshare *address;
   char *unique_name;

   bool private;

   Efl_Bool is_listed : 1;
};

#endif
