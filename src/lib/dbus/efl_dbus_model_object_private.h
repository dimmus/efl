#ifndef _EFL_DBUS_MODEL_OBJECT_PRIVATE_H
#define _EFL_DBUS_MODEL_OBJECT_PRIVATE_H

#include "Efl_Dbus_Model.h"

#include <stdbool.h>

/**
 * efl_dbus_model_object
 */
typedef struct _Efl_Dbus_Model_Object_Data Efl_Dbus_Model_Object_Data;

struct _Efl_Dbus_Model_Object_Data
{
  Eo *obj;

  Eina_List *objects;
  Eina_List *childrens;
  Eina_List *requests;
  Eina_List *pendings;

  Eina_Stringshare *bus;
  Eina_Stringshare *path;

  Efl_Dbus_Introspection_Node *introspection;

  Efl_Bool is_listed : 1;
};

#endif
