#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "Efl_Eo.h"
#include "function_overrides_simple.h"
#include "function_overrides_inherit.h"
#include "function_overrides_inherit2.h"

#include "../eunit_tests.h"

#define MY_CLASS INHERIT2_CLASS

static void
_a_set(Eo *obj, void *class_data EFL_UNUSED, int a)
{
  printf("%s %d\n", efl_class_name_get(MY_CLASS), a);
  simple_a_print(obj);
  simple_a_set(efl_super(obj, MY_CLASS), a + 1);

  Efl_Bool called = EFL_FALSE;
  called          = simple_a_print(efl_super(obj, MY_CLASS));
  fail_if(!called);
}

static Efl_Bool
_print(Eo *obj, void *class_data EFL_UNUSED)
{
  Efl_Bool called = EFL_FALSE;
  printf("Hey\n");
  called = inherit2_print(efl_super(obj, MY_CLASS));
  fail_if(called);

  return EFL_TRUE;
}

static Efl_Bool
_print2(Eo *obj EFL_UNUSED, void *class_data EFL_UNUSED)
{
  printf("Hey2\n");

  return EFL_TRUE;
}

EFL_FUNC_BODY(inherit2_print, Efl_Bool, EFL_FALSE);
EFL_FUNC_BODY(inherit2_print2, Efl_Bool, EFL_FALSE);

static Efl_Bool
_class_initializer(Efl_Class *klass)
{
  EFL_OPS_DEFINE(ops,
                 EFL_OBJECT_OP_FUNC(inherit2_print, _print),
                 EFL_OBJECT_OP_FUNC(inherit2_print2, _print2),
                 EFL_OBJECT_OP_FUNC(simple_a_set, _a_set), );

  return efl_class_functions_set(klass, &ops, NULL);
}

static const Efl_Class_Description class_desc = {
  EO_VERSION, "Inherit2", EFL_CLASS_TYPE_REGULAR, 0, _class_initializer,
  NULL,       NULL
};

EFL_DEFINE_CLASS(inherit2_class_get, &class_desc, INHERIT_CLASS, NULL);
