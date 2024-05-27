#ifdef HAVE_CONFIG_H
#  include <efl_config.h>
#endif

#include "efl_dbus_model_arguments_private.h"
#include "efl_dbus_model_method_private.h"
#include "efl_dbus_model_private.h"
#include "efl_dbus_proxy.h"

#include <Efl_Shared.h>

#include <stdbool.h>

#define MY_CLASS      EFL_DBUS_MODEL_METHOD_CLASS
#define MY_CLASS_NAME "Efl_Dbus_Model_Method"

static void _efl_dbus_model_method_call_cb(void *,
                                           const Efl_Dbus_Message *,
                                           Efl_Dbus_Pending *);

static Efl_Object *
_efl_dbus_model_method_efl_object_constructor(Eo                         *obj,
                                              Efl_Dbus_Model_Method_Data *pd)
{
    obj = efl_constructor(efl_super(obj, MY_CLASS));

    pd->obj    = obj;
    pd->method = NULL;
    return obj;
}

static Efl_Object *
_efl_dbus_model_method_efl_object_finalize(Eo                         *obj,
                                           Efl_Dbus_Model_Method_Data *pd)
{
    if (!pd->proxy || !pd->method) return NULL;

    efl_dbus_model_arguments_custom_constructor(obj,
                                                pd->proxy,
                                                pd->method->name,
                                                pd->method->arguments);

    return efl_finalize(efl_super(obj, MY_CLASS));
}

static void
_efl_dbus_model_method_proxy_set(Eo *obj                     EFL_UNUSED,
                                 Efl_Dbus_Model_Method_Data *pd,
                                 Efl_Dbus_Proxy             *proxy)
{
    pd->proxy = proxy;
}

static void
_efl_dbus_model_method_method_set(Eo *obj                     EFL_UNUSED,
                                  Efl_Dbus_Model_Method_Data *pd,
                                  const Efl_Dbus_Introspection_Method *method)
{
    pd->method = method;
}

static void
_efl_dbus_model_method_call(Eo *obj                        EFL_UNUSED,
                            Efl_Dbus_Model_Method_Data *pd EFL_UNUSED)
{
    Efl_Dbus_Model_Arguments_Data *data =
        efl_data_scope_get(obj, EFL_DBUS_MODEL_ARGUMENTS_CLASS);
    Efl_Dbus_Message *msg =
        efl_dbus_proxy_method_call_new(data->proxy, data->name);
    Efl_Dbus_Message_Iter *iter = efl_dbus_message_iter_get(msg);
    const Efl_Dbus_Introspection_Argument *argument;
    const Eina_List                       *it;
    Efl_Dbus_Pending                      *pending;
    unsigned int                           i = 0;

    EINA_LIST_FOREACH(data->arguments, it, argument)
    {
        Eina_Slstr       *name;
        const Eina_Value *value;
        const char       *signature;
        Efl_Bool          ret;

        if (ELDBUS_INTROSPECTION_ARGUMENT_DIRECTION_IN != argument->direction)
            continue;

        name = eina_slstr_printf(ARGUMENT_FORMAT, i);
        EINA_SAFETY_ON_NULL_GOTO(name, on_error);

        value = eina_hash_find(data->properties, name);
        EINA_SAFETY_ON_NULL_GOTO(value, on_error);

        signature = argument->type;
        if (dbus_type_is_basic(signature[0]))
            ret = _message_iter_from_eina_value(signature, iter, value);
        else ret = _message_iter_from_eina_value_struct(signature, iter, value);

        EINA_SAFETY_ON_FALSE_GOTO(ret, on_error);

        ++i;
    }

    pending            = efl_dbus_proxy_send(data->proxy,
                                  msg,
                                  _efl_dbus_model_method_call_cb,
                                  pd,
                                  -1);
    data->pending_list = eina_list_append(data->pending_list, pending);

    return;

on_error:
    efl_dbus_message_unref(msg);
}

static void
_efl_dbus_model_method_call_cb(void                   *data,
                               const Efl_Dbus_Message *msg,
                               Efl_Dbus_Pending       *pending)
{
    Efl_Dbus_Model_Method_Data    *pd = (Efl_Dbus_Model_Method_Data *)data;
    Efl_Dbus_Model_Arguments_Data *args_data =
        efl_data_scope_get(pd->obj, EFL_DBUS_MODEL_ARGUMENTS_CLASS);

    if (efl_dbus_model_arguments_process_arguments(args_data, msg, pending))
        efl_event_callback_call(pd->obj,
                                EFL_DBUS_MODEL_METHOD_EVENT_SUCCESSFUL_CALL,
                                NULL);
}

#include "efl_dbus_model_method.eo.c"
