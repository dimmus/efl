#ifdef HAVE_CONFIG_H
# include <efl_config.h>
#endif

#include "efl_dbus_model_arguments_private.h"
#include "efl_dbus_model_signal_private.h"
#include "efl_dbus_model_private.h"

#include <Efl_Core.h>
#include <Efl_Shared.h>

#define MY_CLASS EFL_DBUS_MODEL_SIGNAL_CLASS
#define MY_CLASS_NAME "Efl_Dbus_Model_Signal"

static void _efl_dbus_model_signal_handler_cb(void *, const Efl_Dbus_Message *);
static void _efl_dbus_model_signal_callback_add(Efl_Dbus_Model_Signal_Data *);
static void _efl_dbus_model_signal_callback_del(Efl_Dbus_Model_Signal_Data *);

static Efl_Object*
_efl_dbus_model_signal_efl_object_constructor(Eo *obj, Efl_Dbus_Model_Signal_Data *pd)
{
   efl_constructor(efl_super(obj, MY_CLASS));

   pd->obj = obj;
   pd->handler = NULL;
   pd->signal = NULL;
   return obj;
}

static void
_efl_dbus_model_signal_signal_constructor(Eo *obj EFL_UNUSED,
                                 Efl_Dbus_Model_Signal_Data *pd,
                                 Efl_Dbus_Proxy *proxy,
                                 const Efl_Dbus_Introspection_Signal *signal)
{
   EINA_SAFETY_ON_NULL_RETURN(proxy);
   EINA_SAFETY_ON_NULL_RETURN(signal);

   efl_dbus_model_arguments_custom_constructor(efl_super(obj, MY_CLASS), proxy, signal->name, signal->arguments);

   pd->signal = signal;
   _efl_dbus_model_signal_callback_add(pd);
}

static void
_efl_dbus_model_signal_efl_object_invalidate(Eo *obj, Efl_Dbus_Model_Signal_Data *pd)
{
   _efl_dbus_model_signal_callback_del(pd);

   efl_invalidate(efl_super(obj, MY_CLASS));
}


static void
_efl_dbus_model_signal_callback_add(Efl_Dbus_Model_Signal_Data *pd)
{
   EINA_SAFETY_ON_NULL_RETURN(pd);
   EINA_SAFETY_ON_FALSE_RETURN(NULL == pd->handler);

   Efl_Dbus_Model_Arguments_Data *args_data = efl_data_scope_get(pd->obj, EFL_DBUS_MODEL_ARGUMENTS_CLASS);
   EINA_SAFETY_ON_NULL_RETURN(args_data);

   pd->handler = efl_dbus_proxy_signal_handler_add(args_data->proxy, pd->signal->name, _efl_dbus_model_signal_handler_cb, pd);
}

static void
_efl_dbus_model_signal_callback_del(Efl_Dbus_Model_Signal_Data *pd)
{
   EINA_SAFETY_ON_NULL_RETURN(pd);

   if (pd->handler)
     {
        efl_dbus_signal_handler_unref(pd->handler);
        pd->handler = NULL;
     }
}

static void
_efl_dbus_model_signal_handler_cb(void *data, const Efl_Dbus_Message *msg)
{
   Efl_Dbus_Model_Signal_Data *pd = (Efl_Dbus_Model_Signal_Data*)data;

   Efl_Dbus_Model_Arguments_Data *args_data = efl_data_scope_get(pd->obj, EFL_DBUS_MODEL_ARGUMENTS_CLASS);

   efl_dbus_model_arguments_process_arguments(args_data, msg, NULL);
}

#include "efl_dbus_model_signal.eo.c"
