#ifndef _EFL_DBUS_MODEL_SIGNAL_PRIVATE_H
#define _EFL_DBUS_MODEL_SIGNAL_PRIVATE_H

#include "Efl_Dbus_Model.h"

typedef struct _Efl_Dbus_Model_Signal_Data Efl_Dbus_Model_Signal_Data;

/**
 * efl_dbus_model_signal
 */
struct _Efl_Dbus_Model_Signal_Data
{
  Eo                                  *obj;
  Efl_Dbus_Signal_Handler             *handler;
  const Efl_Dbus_Introspection_Signal *signal;
};

#endif
