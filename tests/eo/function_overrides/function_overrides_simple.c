#include "Efl_Eo.h"
#include "function_overrides_simple.h"

#include "../eunit_tests.h"

#define MY_CLASS SIMPLE_CLASS

Efl_Bool class_print_called = EFL_FALSE;
Efl_Bool class_print2_called = EFL_FALSE;

static void
_a_set(Eo *obj EFL_UNUSED, void *class_data, int a)
{
   Simple_Public_Data *pd = class_data;
   testlog("%s %d\n", efl_class_name_get(MY_CLASS), a);
   pd->a = a;
}

static Efl_Bool
_a_print(Eo *obj EFL_UNUSED, void *class_data)
{
   Simple_Public_Data *pd = class_data;
   testlog("Print %s %d\n", efl_class_name_get(MY_CLASS), pd->a);

   return EFL_TRUE;
}

static Efl_Bool
_class_print(Efl_Class *klass, void *class_data EFL_UNUSED)
{
   testlog("Print %s-%s\n", efl_class_name_get(klass), efl_class_name_get(MY_CLASS));
   Efl_Bool called = EFL_FALSE;
   called = simple_class_print(efl_super(klass, MY_CLASS));
   fail_if(called);

   called = simple_class_print2(efl_super(klass, MY_CLASS));
   fail_if(called);

   return EFL_TRUE;
}

static Efl_Bool
_class_print2(Efl_Class *klass, void *class_data EFL_UNUSED)
{
   testlog("Print %s-%s\n", efl_class_name_get(klass), efl_class_name_get(MY_CLASS));

   return EFL_TRUE;
}

EFL_VOID_FUNC_BODYV(simple_a_set, EFL_FUNC_CALL(a), int a);
EFL_FUNC_BODY(simple_a_print, Efl_Bool, EFL_FALSE);
EFL_FUNC_BODY_CONST(simple_class_print, Efl_Bool, EFL_FALSE);
EFL_FUNC_BODY_CONST(simple_class_print2, Efl_Bool, EFL_FALSE);

static Efl_Bool
_class_initializer(Efl_Class *klass)
{
   EFL_OPS_DEFINE(ops,
         EFL_OBJECT_OP_FUNC(simple_a_set, _a_set),
         EFL_OBJECT_OP_FUNC(simple_a_print, _a_print),
         EFL_OBJECT_OP_FUNC(simple_class_print, _class_print),
         EFL_OBJECT_OP_FUNC(simple_class_print2, _class_print2),
   );

   return efl_class_functions_set(klass, &ops, NULL);
}

static const Efl_Class_Description class_desc = {
     EO_VERSION,
     "Simple",
     EFL_CLASS_TYPE_REGULAR,
     sizeof(Simple_Public_Data),
     _class_initializer,
     NULL,
     NULL
};

EFL_DEFINE_CLASS(simple_class_get, &class_desc, EO_CLASS, NULL);

