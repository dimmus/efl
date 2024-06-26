#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"

EOLIAN_API const Eolian_Class *
eolian_constructor_class_get(const Eolian_Constructor *ctor)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(ctor, NULL);
  return ctor->klass;
}

EOLIAN_API const Eolian_Function *
eolian_constructor_function_get(const Eolian_Constructor *ctor)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(ctor, NULL);
  const Eolian_Class *klass = eolian_constructor_class_get(ctor);
  if (!klass) return NULL;
  return eolian_class_function_by_name_get(klass,
                                           ctor->base.name +
                                             strlen(klass->base.name) + 1,
                                           EOLIAN_UNRESOLVED);
}

EOLIAN_API Efl_Bool
eolian_constructor_is_optional(const Eolian_Constructor *ctor)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(ctor, EFL_FALSE);
  return ctor->is_optional;
}
