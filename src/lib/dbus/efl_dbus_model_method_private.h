#ifndef _EFL_DBUS_MODEL_METHOD_PRIVATE_H
#define _EFL_DBUS_MODEL_METHOD_PRIVATE_H

#include "Efl_Dbus_Model.h"

typedef struct _Efl_Dbus_Model_Method_Data Efl_Dbus_Model_Method_Data;

/**
 * efl_dbus_model_method
 */
struct _Efl_Dbus_Model_Method_Data
{
  Eo *obj;

  const Efl_Dbus_Introspection_Method *method;

  Efl_Dbus_Proxy *proxy;
};

#endif
