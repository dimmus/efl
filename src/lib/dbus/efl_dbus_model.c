#ifdef HAVE_CONFIG_H
#  include <efl_config.h>
#endif

#include "efl_dbus_model_private.h"

#include <Efl_Core.h>
#include <Efl_Shared.h>
#include <Efl_Dbus.h>

#define MY_CLASS      EFL_DBUS_MODEL_CLASS
#define MY_CLASS_NAME "Efl_Dbus_Model"

static void
efl_dbus_model_connect_do(Efl_Dbus_Model_Data *pd)
{
  if (pd->type == EFL_DBUS_CONNECTION_TYPE_ADDRESS)
  {
    if (pd->private)
      pd->connection = efl_dbus_address_connection_get(pd->address);
    else pd->connection = efl_dbus_private_address_connection_get(pd->address);
  }
  else
  {
    if (pd->private) pd->connection = efl_dbus_private_connection_get(pd->type);
    else pd->connection = efl_dbus_connection_get(pd->type);
  }

   // TODO: Register for disconnection event
  if (!pd->connection)
  {
    DBG("Unable to setup a connection [%i - %s] %i",
        pd->type,
        pd->address,
        pd->private);
  }
}

static void
_efl_dbus_model_connect(Eo *obj                  EFL_UNUSED,
                        Efl_Dbus_Model_Data     *pd,
                        Efl_Dbus_Connection_Type type,
                        const char              *address,
                        Efl_Bool                 priv)
{
  pd->type    = type;
  pd->address = eina_stringshare_add(address);
  pd->private = priv;
}

static void
_efl_dbus_model_connection_set(Eo *obj              EFL_UNUSED,
                               Efl_Dbus_Model_Data *pd,
                               Efl_Dbus_Connection *dbus)
{
  Efl_Dbus_Connection *tounref = pd->connection;

  efl_dbus_connection_ref(dbus);
  pd->connection = NULL;
  if (tounref) efl_dbus_connection_unref(tounref);
  pd->connection = dbus;
}

static Efl_Dbus_Connection *
_efl_dbus_model_connection_get(const Eo *obj        EFL_UNUSED,
                               Efl_Dbus_Model_Data *pd)
{
  return pd->connection;
}

static Efl_Object *
_efl_dbus_model_efl_object_finalize(Eo *obj, Efl_Dbus_Model_Data *pd)
{
  if (!pd->connection) efl_dbus_model_connect_do(pd);
  if (!pd->connection) return NULL;

  return efl_finalize(efl_super(obj, EFL_DBUS_MODEL_CLASS));
}

static void
_efl_dbus_model_efl_object_invalidate(Eo *obj, Efl_Dbus_Model_Data *pd)
{
  Efl_Dbus_Connection *connection = pd->connection;

  pd->connection = NULL;
  if (connection) efl_dbus_connection_unref(connection);

  efl_invalidate(efl_super(obj, MY_CLASS));
}

static void
_efl_dbus_model_efl_object_destructor(Eo *obj, Efl_Dbus_Model_Data *pd)
{
  eina_stringshare_del(pd->unique_name);
  pd->unique_name = NULL;

  eina_stringshare_del(pd->address);
  pd->address = NULL;

  efl_destructor(efl_super(obj, MY_CLASS));
}

static const char *
_efl_dbus_model_address_get(const Eo *obj EFL_UNUSED, Efl_Dbus_Model_Data *pd)
{
  return pd->address;
}

static Efl_Bool
_efl_dbus_model_private_get(const Eo *obj EFL_UNUSED, Efl_Dbus_Model_Data *pd)
{
  return pd->private;
}

static Efl_Dbus_Connection_Type
_efl_dbus_model_type_get(const Eo *obj EFL_UNUSED, Efl_Dbus_Model_Data *pd)
{
  return pd->type;
}

static Eina_Future *
_efl_dbus_model_efl_model_property_set(Eo                     *obj,
                                       Efl_Dbus_Model_Data *pd EFL_UNUSED,
                                       const char             *property,
                                       Eina_Value *value       EFL_UNUSED)
{
  Eina_Error err = EFL_MODEL_ERROR_READ_ONLY;

  if (!eina_streq(property, UNIQUE_NAME_PROPERTY))
    err = EFL_MODEL_ERROR_NOT_FOUND;
  return efl_loop_future_rejected(obj, err);
}

static Eina_Value *
_efl_dbus_model_efl_model_property_get(const Eo            *obj,
                                       Efl_Dbus_Model_Data *pd,
                                       const char          *property)
{
  DBG("(%p): property=%s", obj, property);

  if (!eina_streq(property, UNIQUE_NAME_PROPERTY)) goto on_error;

  if (!pd->connection) efl_dbus_model_connect_do(pd);

  if (pd->unique_name == NULL)
  {
    const char *unique_name;

    unique_name = efl_dbus_connection_unique_name_get(pd->connection);
    if (!unique_name) goto on_error;
    pd->unique_name = eina_stringshare_add(unique_name);
  }

  return eina_value_string_new(pd->unique_name);

on_error:
  return eina_value_error_new(EFL_MODEL_ERROR_NOT_FOUND);
}

static Eina_Iterator *
_efl_dbus_model_efl_model_properties_get(const Eo *obj           EFL_UNUSED,
                                         Efl_Dbus_Model_Data *pd EFL_UNUSED)
{
  char *unique[] = { UNIQUE_NAME_PROPERTY };

  return EINA_C_ARRAY_ITERATOR_NEW(unique);
}

static Efl_Object *
_efl_dbus_model_efl_model_child_add(Eo *obj                 EFL_UNUSED,
                                    Efl_Dbus_Model_Data *pd EFL_UNUSED)
{
  return NULL;
}

static void
_efl_dbus_model_efl_model_child_del(Eo *obj                 EFL_UNUSED,
                                    Efl_Dbus_Model_Data *pd EFL_UNUSED,
                                    Efl_Object *child       EFL_UNUSED)
{}

static Eina_Future *
_efl_dbus_model_efl_model_children_slice_get(Eo *obj                 EFL_UNUSED,
                                             Efl_Dbus_Model_Data *pd EFL_UNUSED,
                                             unsigned int start      EFL_UNUSED,
                                             unsigned int count      EFL_UNUSED)
{
  return efl_loop_future_rejected(obj, EFL_MODEL_ERROR_NOT_SUPPORTED);
}

static unsigned int
_efl_dbus_model_efl_model_children_count_get(const Eo *obj           EFL_UNUSED,
                                             Efl_Dbus_Model_Data *pd EFL_UNUSED)
{
  return 0;
}

#include "efl_dbus_model.eo.c"
