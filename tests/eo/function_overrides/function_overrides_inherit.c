#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "Efl_Eo.h"
#include "function_overrides_simple.h"
#include "function_overrides_inherit.h"

#define MY_CLASS INHERIT_CLASS

static const Efl_Class_Description class_desc = {
  EO_VERSION, "Inherit", EFL_CLASS_TYPE_REGULAR, 0, NULL, NULL, NULL
};

EFL_DEFINE_CLASS(inherit_class_get, &class_desc, SIMPLE_CLASS, NULL);
