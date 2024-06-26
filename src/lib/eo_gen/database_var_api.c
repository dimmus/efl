#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"

EOLIAN_API const Eolian_Documentation *
eolian_constant_documentation_get(const Eolian_Constant *var)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
  return var->doc;
}

EOLIAN_API const Eolian_Type *
eolian_constant_type_get(const Eolian_Constant *var)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
  return var->base_type;
}

EOLIAN_API const Eolian_Expression *
eolian_constant_value_get(const Eolian_Constant *var)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(var, NULL);
  return var->value;
}

EOLIAN_API Efl_Bool
eolian_constant_is_extern(const Eolian_Constant *var)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(var, EFL_FALSE);
  return var->is_extern;
}
