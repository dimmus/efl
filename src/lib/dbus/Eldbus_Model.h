#ifndef _ELDBUS_MODEL_H
#define _ELDBUS_MODEL_H

#include <Efl_Dbus.h>
#include <Efl_Eo.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef EFL_BETA_API_SUPPORT
#include <Efl_Eo_Interfaces.h>
#include <eldbus_model_connection.eo.h>
#include <eldbus_model_object.eo.h>
#include <eldbus_model_proxy.eo.h>
#include <eldbus_model_arguments.eo.h>
#include <eldbus_model_method.eo.h>
#include <eldbus_model_signal.eo.h>

#endif

#ifdef __cplusplus
}
#endif

#endif
