#ifndef _EFL_DBUS_MODEL_ARGUMENTS_PRIVATE_H
#define _EFL_DBUS_MODEL_ARGUMENTS_PRIVATE_H

#include "Efl_Dbus_Model.h"

#include <stdbool.h>

typedef struct _Efl_Dbus_Model_Arguments_Data Efl_Dbus_Model_Arguments_Data;

/**
 * efl_dbus_model_arguments
 */
struct _Efl_Dbus_Model_Arguments_Data
{
   Eo *obj;
   Efl_Dbus_Proxy *proxy;
   Eina_Hash *properties;
   Eina_Stringshare *name;
   Eina_List *pending_list;
   const Eina_List *arguments;
   Eina_Value tmp_value;
};

Efl_Bool efl_dbus_model_arguments_process_arguments(Efl_Dbus_Model_Arguments_Data *, const Efl_Dbus_Message *, Efl_Dbus_Pending *);

#define ARGUMENT_FORMAT "arg%u"

#endif
