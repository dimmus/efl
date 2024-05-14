#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#  include <evil_private.h> /* setenv unsetenv */
#endif

#include <Efl_Shared.h>
#include <Efl_Eo_Gen.h>

#include "efl-test-runner.h"

TEST(eolian_namespaces)
{
    const Eolian_Class *class11, *class112, *class21, *class_no, *impl_class,
        *iclass;
    const Eolian_Function  *fid;
    Eina_Iterator          *iter;
    Eolian_Function_Type    func_type;
    const char             *val1, *val2;
    const Eolian_Implement *impl;
    const Eolian_Unit      *unit;
    void                   *dummy;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "nmsp1_class1.eo")));

   /* Classes existence  */
    efl_assert_fail_if(!(class11 = eolian_unit_class_by_name_get(unit, "nmsp1.class1")));
    efl_assert_fail_if(!(class112 =
                  eolian_unit_class_by_name_get(unit, "nmsp1.nmsp11.class2")));
    efl_assert_fail_if(!(class21 = eolian_unit_class_by_name_get(unit, "nmsp2.class1")));
    efl_assert_fail_if(!(class_no = eolian_unit_class_by_name_get(unit, "no_nmsp")));

   /* Check names and namespaces*/
    efl_assert_fail_if(strcmp(eolian_class_short_name_get(class11), "class1"));
    efl_assert_fail_if(!(iter = eolian_class_namespaces_get(class11)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&val1)));
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    efl_assert_fail_if(strcmp(val1, "nmsp1"));
    eina_iterator_free(iter);

    efl_assert_fail_if(strcmp(eolian_class_short_name_get(class112), "class2"));
    efl_assert_fail_if(!(iter = eolian_class_namespaces_get(class112)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&val1)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&val2)));
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    efl_assert_fail_if(strcmp(val1, "nmsp1"));
    efl_assert_fail_if(strcmp(val2, "nmsp11"));
    eina_iterator_free(iter);

    efl_assert_fail_if(strcmp(eolian_class_short_name_get(class21), "class1"));
    efl_assert_fail_if(!(iter = eolian_class_namespaces_get(class21)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&val1)));
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    efl_assert_fail_if(strcmp(val1, "nmsp2"));
    eina_iterator_free(iter);

    efl_assert_fail_if(strcmp(eolian_class_short_name_get(class_no), "no_nmsp"));
    efl_assert_fail_if(eolian_class_namespaces_get(class_no));

   /* Inherits */
    efl_assert_fail_if(eolian_class_parent_get(class11) != class112);
    efl_assert_fail_if(!(iter = eolian_class_extensions_get(class11)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&iclass)));
    efl_assert_fail_if(iclass != class21);
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&iclass)));
    efl_assert_fail_if(iclass != class_no);
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);

   /* Implements */
    efl_assert_fail_if(!(iter = eolian_class_implements_get(class11)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!(impl_class = eolian_implement_class_get(impl)));
    efl_assert_fail_if(!(fid = eolian_implement_function_get(impl, &func_type)));
    efl_assert_fail_if(impl_class != class112);
    efl_assert_fail_if(strcmp(eolian_function_name_get(fid), "a"));
    efl_assert_fail_if(func_type != EOLIAN_PROP_SET);

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!(impl_class = eolian_implement_class_get(impl)));
    efl_assert_fail_if(!(fid = eolian_implement_function_get(impl, &func_type)));
    efl_assert_fail_if(impl_class != class_no);
    efl_assert_fail_if(strcmp(eolian_function_name_get(fid), "foo"));
    efl_assert_fail_if(func_type != EOLIAN_METHOD);
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);

    eolian_state_free(eos);
}

TEST(eolian_events)
{
    const Eolian_Class *class;
    Eina_Iterator      *iter;
    const char         *name, *type_name;
    const Eolian_Type  *type;
    const Eolian_Event *ev;
    const Eolian_Unit  *unit;
    void               *dummy;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "events.eo")));

   /* Class */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Events")));
    efl_assert_fail_if(
        strcmp(eolian_class_event_c_prefix_get(class), "totally_not_events"));

   /* Events */
    efl_assert_fail_if(!(iter = eolian_class_events_get(class)));
   /* Clicked */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&ev)));
    efl_assert_fail_if(!(name = eolian_event_name_get(ev)));
    efl_assert_fail_if(eolian_event_type_get(ev));
    efl_assert_fail_if(strcmp(name, "clicked"));
    efl_assert_fail_if(!eolian_event_is_beta(ev));
    efl_assert_fail_if(eolian_event_is_hot(ev));
    efl_assert_fail_if(eolian_event_is_restart(ev));
   /* Clicked,double */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&ev)));
    efl_assert_fail_if(!(name = eolian_event_name_get(ev)));
    efl_assert_fail_if(!(type = eolian_event_type_get(ev)));
    efl_assert_fail_if(strcmp(name, "clicked,double"));
    type_name = eolian_type_short_name_get(type);
    efl_assert_fail_if(strcmp(type_name, "Evas_Event_Clicked_Double_Info"));
    efl_assert_fail_if(eolian_event_is_beta(ev));
    efl_assert_fail_if(eolian_event_is_hot(ev));
    efl_assert_fail_if(eolian_event_is_restart(ev));
   /* Hot */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&ev)));
    efl_assert_fail_if(!(name = eolian_event_name_get(ev)));
    efl_assert_fail_if(eolian_event_type_get(ev));
    efl_assert_fail_if(strcmp(name, "hot"));
    efl_assert_fail_if(eolian_event_is_beta(ev));
    efl_assert_fail_if(!eolian_event_is_hot(ev));
    efl_assert_fail_if(eolian_event_is_restart(ev));
   /* Restart */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&ev)));
    efl_assert_fail_if(!(name = eolian_event_name_get(ev)));
    efl_assert_fail_if(eolian_event_type_get(ev));
    efl_assert_fail_if(strcmp(name, "restart"));
    efl_assert_fail_if(eolian_event_is_beta(ev));
    efl_assert_fail_if(eolian_event_is_hot(ev));
    efl_assert_fail_if(!eolian_event_is_restart(ev));
   /* Hot Restart */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&ev)));
    efl_assert_fail_if(!(name = eolian_event_name_get(ev)));
    efl_assert_fail_if(eolian_event_type_get(ev));
    efl_assert_fail_if(strcmp(name, "hot_restart"));
    efl_assert_fail_if(eolian_event_is_beta(ev));
    efl_assert_fail_if(!eolian_event_is_hot(ev));
    efl_assert_fail_if(!eolian_event_is_restart(ev));

    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);
   /* Check eolian_class_event_by_name_get */
    efl_assert_fail_if(!eolian_class_event_by_name_get(class, "clicked,double"));
    efl_assert_fail_if(eolian_class_event_by_name_get(class, "clicked,triple"));

    eolian_state_free(eos);
}

TEST(eolian_override)
{
    Eina_Iterator         *iter;
    const Eolian_Class    *impl_class = NULL;
    const Eolian_Function *impl_func  = NULL;
    const Eolian_Class *class, *base;
    const Eolian_Implement *impl;
    const Eolian_Unit      *unit;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "override.eo")));

   /* Class */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Override")));
    efl_assert_fail_if(!(base = eolian_unit_class_by_name_get(unit, "Base")));

   /* Implements */
    efl_assert_fail_if(!(iter = eolian_class_implements_get(class)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl))); /* skip a */

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!eolian_implement_is_auto(impl, EOLIAN_PROP_SET));
    efl_assert_fail_if(eolian_implement_is_empty(impl, EOLIAN_PROP_SET));
    efl_assert_fail_if(eolian_implement_is_pure_virtual(impl, EOLIAN_PROP_SET));
    efl_assert_fail_if(!(impl_class = eolian_implement_class_get(impl)));
    efl_assert_fail_if(!(impl_func = eolian_implement_function_get(impl, NULL)));
    efl_assert_fail_if(impl_class != class);
    efl_assert_fail_if(eolian_implement_implementing_class_get(impl) != class);
    efl_assert_fail_if(strcmp(eolian_function_name_get(impl_func), "b"));

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl))); /* skip c */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl))); /* skip foo */

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(eolian_implement_is_auto(impl, EOLIAN_METHOD));
    efl_assert_fail_if(!eolian_implement_is_empty(impl, EOLIAN_METHOD));
    efl_assert_fail_if(eolian_implement_is_pure_virtual(impl, EOLIAN_METHOD));
    efl_assert_fail_if(!(impl_class = eolian_implement_class_get(impl)));
    efl_assert_fail_if(!(impl_func = eolian_implement_function_get(impl, NULL)));
    efl_assert_fail_if(impl_class != class);
    efl_assert_fail_if(eolian_implement_implementing_class_get(impl) != class);
    efl_assert_fail_if(strcmp(eolian_function_name_get(impl_func), "bar"));

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(eolian_implement_is_auto(impl, EOLIAN_METHOD));
    efl_assert_fail_if(eolian_implement_is_empty(impl, EOLIAN_METHOD));
    efl_assert_fail_if(eolian_implement_is_pure_virtual(impl, EOLIAN_METHOD));
    efl_assert_fail_if(!(impl_class = eolian_implement_class_get(impl)));
    efl_assert_fail_if(!(impl_func = eolian_implement_function_get(impl, NULL)));
    efl_assert_fail_if(impl_class != base);
    efl_assert_fail_if(eolian_implement_implementing_class_get(impl) != class);
    efl_assert_fail_if(strcmp(eolian_function_name_get(impl_func), "constructor"));

    eina_iterator_free(iter);

    eolian_state_free(eos);
}

TEST(eolian_consts)
{
    const Eolian_Function *fid = NULL;
    const Eolian_Class *class;
    const Eolian_Unit *unit;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "consts.eo")));
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Consts")));

   /* Method */
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD)));
    efl_assert_fail_if(EFL_FALSE == eolian_function_object_is_const(fid));

    eolian_state_free(eos);
}

TEST(eolian_ctor_dtor)
{
    Eina_Iterator         *iter;
    const Eolian_Class    *impl_class = NULL;
    const Eolian_Function *impl_func  = NULL;
    const Eolian_Class *class, *base;
    const Eolian_Implement   *impl;
    const Eolian_Constructor *ctor;
    const Eolian_Unit        *unit;
    void                     *dummy;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "ctor_dtor.eo")));
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Ctor_Dtor")));
    efl_assert_fail_if(!(base = eolian_unit_class_by_name_get(unit, "Base")));

   /* Class ctor/dtor */
    efl_assert_fail_if(!eolian_class_ctor_enable_get(class));
    efl_assert_fail_if(!eolian_class_dtor_enable_get(class));

   /* Base ctor/dtor */
    efl_assert_fail_if(!(iter = eolian_class_implements_get(class)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!(impl_class = eolian_implement_class_get(impl)));
    efl_assert_fail_if(!(impl_func = eolian_implement_function_get(impl, NULL)));
    efl_assert_fail_if(impl_class != base);
    efl_assert_fail_if(strcmp(eolian_function_name_get(impl_func), "constructor"));
    efl_assert_fail_if(!eolian_function_is_constructor(impl_func, base));
    efl_assert_fail_if(!eolian_function_is_constructor(impl_func, class));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&impl)));
    efl_assert_fail_if(!(impl_class = eolian_implement_class_get(impl)));
    efl_assert_fail_if(!(impl_func = eolian_implement_function_get(impl, NULL)));
    efl_assert_fail_if(impl_class != base);
    efl_assert_fail_if(strcmp(eolian_function_name_get(impl_func), "destructor"));
    efl_assert_fail_if(eolian_function_is_constructor(impl_func, base));
    efl_assert_fail_if(eolian_function_is_constructor(impl_func, class));
    eina_iterator_free(iter);

   /* Custom ctors/dtors */
    efl_assert_fail_if(
        !eolian_class_function_by_name_get(base, "destructor", EOLIAN_METHOD));
    efl_assert_fail_if(!(iter = eolian_class_constructors_get(class)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&ctor)));
    efl_assert_fail_if(eolian_constructor_is_optional(ctor));
    efl_assert_fail_if(!(impl_class = eolian_constructor_class_get(ctor)));
    efl_assert_fail_if(!(impl_func = eolian_constructor_function_get(ctor)));
    efl_assert_fail_if(impl_class != class);
    efl_assert_fail_if(
        strcmp(eolian_function_name_get(impl_func), "custom_constructor_1"));
    efl_assert_fail_if(!eolian_function_is_constructor(impl_func, class));
    efl_assert_fail_if(eolian_function_is_constructor(impl_func, base));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&ctor)));
    efl_assert_fail_if(!eolian_constructor_is_optional(ctor));
    efl_assert_fail_if(!(impl_class = eolian_constructor_class_get(ctor)));
    efl_assert_fail_if(!(impl_func = eolian_constructor_function_get(ctor)));
    efl_assert_fail_if(impl_class != class);
    efl_assert_fail_if(
        strcmp(eolian_function_name_get(impl_func), "custom_constructor_2"));
    efl_assert_fail_if(!eolian_function_is_constructor(impl_func, class));
    efl_assert_fail_if(eolian_function_is_constructor(impl_func, base));
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);

    eolian_state_free(eos);
}

TEST(eolian_typedef)
{
    const Eolian_Type     *type      = NULL;
    const Eolian_Typedecl *tdl       = NULL;
    const char            *type_name = NULL;
    Eina_Iterator         *iter      = NULL;
    const Eolian_Class *class;
    const Eolian_Unit *unit;
    const char        *file;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "typedef.eo")));

   /* Check that the class Dummy is still readable */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Typedef")));
    efl_assert_fail_if(!eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD));

   /* Basic type */
    efl_assert_fail_if(!(tdl = eolian_unit_alias_by_name_get(unit, "Evas.Coord")));
    efl_assert_fail_if(eolian_typedecl_type_get(tdl) != EOLIAN_TYPEDECL_ALIAS);
    efl_assert_fail_if(!(type_name = eolian_typedecl_short_name_get(tdl)));
    efl_assert_fail_if(strcmp(type_name, "Coord"));
    efl_assert_fail_if(!(type_name = eolian_typedecl_c_type_get(tdl)));
    efl_assert_fail_if(strcmp(type_name, "typedef int Evas_Coord"));
    eina_stringshare_del(type_name);
    efl_assert_fail_if(!(type = eolian_typedecl_base_type_get(tdl)));
    efl_assert_fail_if(!(type_name = eolian_type_short_name_get(type)));
    efl_assert_fail_if(eolian_type_is_const(type));
    efl_assert_fail_if(eolian_type_base_type_get(type));
    efl_assert_fail_if(strcmp(type_name, "int"));

   /* File */
    efl_assert_fail_if(!(file = eolian_object_file_get((const Eolian_Object *)tdl)));
    efl_assert_fail_if(strcmp(file, "typedef.eo"));

   /* Lowest alias base */
    efl_assert_fail_if(!(tdl = eolian_unit_alias_by_name_get(unit, "Evas.Coord3")));
    efl_assert_fail_if(!(type = eolian_typedecl_aliased_base_get(tdl)));
    efl_assert_fail_if(strcmp(eolian_type_short_name_get(type), "int"));

   /* Complex type */
    efl_assert_fail_if(!(tdl = eolian_unit_alias_by_name_get(unit, "List_Objects")));
    efl_assert_fail_if(!(type_name = eolian_typedecl_short_name_get(tdl)));
    efl_assert_fail_if(strcmp(type_name, "List_Objects"));
    efl_assert_fail_if(!(type = eolian_typedecl_base_type_get(tdl)));
    efl_assert_fail_if(!(type_name = eolian_type_c_type_get(type)));
    efl_assert_fail_if(strcmp(type_name, "Eina_Iterator *"));
    eina_stringshare_del(type_name);
    efl_assert_fail_if(!(type = eolian_type_base_type_get(type)));
    efl_assert_fail_if(!!eolian_type_next_type_get(type));
    efl_assert_fail_if(!(type_name = eolian_type_c_type_get(type)));
    efl_assert_fail_if(strcmp(type_name, "Typedef *"));
    eina_stringshare_del(type_name);

   /* List */
    efl_assert_fail_if(!(iter = eolian_state_aliases_by_file_get(eos, "typedef.eo")));
    efl_assert_fail_if(!eina_iterator_next(iter, (void **)&tdl));
    efl_assert_fail_if(!(type_name = eolian_typedecl_short_name_get(tdl)));
    efl_assert_fail_if(strcmp(type_name, "Coord"));
    efl_assert_fail_if(!eina_iterator_next(iter, (void **)&tdl));
    efl_assert_fail_if(!(type_name = eolian_typedecl_short_name_get(tdl)));
    efl_assert_fail_if(strcmp(type_name, "List_Objects"));
   /* coord2 and coord3, skip */
    efl_assert_fail_if(!eina_iterator_next(iter, (void **)&tdl));
    efl_assert_fail_if(!eina_iterator_next(iter, (void **)&tdl));
   /* not generated extern, skip */
    efl_assert_fail_if(!eina_iterator_next(iter, (void **)&tdl));
   /* not generated undefined type, skip */
    efl_assert_fail_if(!eina_iterator_next(iter, (void **)&tdl));
    efl_assert_fail_if(eina_iterator_next(iter, (void **)&tdl));

    eolian_state_free(eos);
}

TEST(eolian_complex_type)
{
    const Eolian_Function           *fid       = NULL;
    const Eolian_Function_Parameter *param     = NULL;
    const Eolian_Type               *type      = NULL;
    const char                      *type_name = NULL;
    Eina_Iterator                   *iter      = NULL;
    const Eolian_Class *class;
    const Eolian_Unit *unit;
    void              *dummy;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "complex_type.eo")));
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Complex_Type")));

   /* Properties return type */
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "a", EOLIAN_PROPERTY)));
    efl_assert_fail_if(!(type = eolian_function_return_type_get(fid, EOLIAN_PROP_SET)));
    efl_assert_fail_if(
        !(type_name = eolian_function_return_c_type_get(fid, EOLIAN_PROP_SET)));
    efl_assert_fail_if(!eolian_function_return_is_move(fid, EOLIAN_PROP_SET));
    efl_assert_fail_if(eolian_type_builtin_type_get(type) != EOLIAN_TYPE_BUILTIN_LIST);
    efl_assert_fail_if(strcmp(type_name, "Eina_List *"));
    eina_stringshare_del(type_name);
    efl_assert_fail_if(!(type = eolian_type_base_type_get(type)));
    efl_assert_fail_if(!!eolian_type_next_type_get(type));
    efl_assert_fail_if(!(type_name = eolian_type_c_type_get(type)));
    efl_assert_fail_if(eolian_type_is_move(type));
    efl_assert_fail_if(eolian_type_builtin_type_get(type) != EOLIAN_TYPE_BUILTIN_ARRAY);
    efl_assert_fail_if(strcmp(type_name, "Eina_Array *"));
    eina_stringshare_del(type_name);
    efl_assert_fail_if(!(type = eolian_type_base_type_get(type)));
    efl_assert_fail_if(!!eolian_type_next_type_get(type));
    efl_assert_fail_if(!(type_name = eolian_type_c_type_get(type)));
    efl_assert_fail_if(!eolian_type_is_move(type));
    efl_assert_fail_if(strcmp(type_name, "Efl_Strbuf *"));
    eina_stringshare_del(type_name);
   /* Properties parameter type */
    efl_assert_fail_if(!(iter = eolian_property_values_get(fid, EOLIAN_PROP_GET)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "value"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(!(type_name = eolian_parameter_c_type_get(param, EFL_FALSE)));
    efl_assert_fail_if(!eolian_parameter_is_move(param));
    efl_assert_fail_if(strcmp(type_name, "Eina_List *"));
    eina_stringshare_del(type_name);
    efl_assert_fail_if(!(type = eolian_type_base_type_get(type)));
    efl_assert_fail_if(!!eolian_type_next_type_get(type));
    efl_assert_fail_if(!(type_name = eolian_type_c_type_get(type)));
    efl_assert_fail_if(eolian_type_is_move(type));
    efl_assert_fail_if(strcmp(type_name, "const char *"));
    eina_stringshare_del(type_name);

   /* Methods return type */
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD)));
    efl_assert_fail_if(!(type = eolian_function_return_type_get(fid, EOLIAN_METHOD)));
    efl_assert_fail_if(
        !(type_name = eolian_function_return_c_type_get(fid, EOLIAN_METHOD)));
    efl_assert_fail_if(!eolian_function_return_is_move(fid, EOLIAN_METHOD));
    efl_assert_fail_if(strcmp(type_name, "Eina_List *"));
    eina_stringshare_del(type_name);
    efl_assert_fail_if(!(type = eolian_type_base_type_get(type)));
    efl_assert_fail_if(!!eolian_type_next_type_get(type));
    efl_assert_fail_if(!(type_name = eolian_type_c_type_get(type)));
    efl_assert_fail_if(eolian_type_is_move(type));
    efl_assert_fail_if(eolian_type_builtin_type_get(type) !=
            EOLIAN_TYPE_BUILTIN_STRINGSHARE);
    efl_assert_fail_if(strcmp(type_name, "Eina_Stringshare *"));
    eina_stringshare_del(type_name);
   /* Methods parameter types */
    efl_assert_fail_if(!(iter = eolian_function_parameters_get(fid)));

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "buf"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(!(type_name = eolian_parameter_c_type_get(param, EFL_FALSE)));
    efl_assert_fail_if(!eolian_parameter_is_move(param));
    efl_assert_fail_if(eolian_type_builtin_type_get(type) != EOLIAN_TYPE_BUILTIN_MSTRING);
    efl_assert_fail_if(strcmp(type_name, "char *"));
    eina_stringshare_del(type_name);

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "sl"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(!(type_name = eolian_parameter_c_type_get(param, EFL_FALSE)));
    efl_assert_fail_if(eolian_parameter_is_move(param));
    efl_assert_fail_if(eolian_type_builtin_type_get(type) != EOLIAN_TYPE_BUILTIN_SLICE);
    efl_assert_fail_if(strcmp(type_name, "Eina_Slice"));
    eina_stringshare_del(type_name);

    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);

    eolian_state_free(eos);
}

TEST(eolian_scope)
{
    const Eolian_Function *fid = NULL;
    const Eolian_Class *class;
    const Eolian_Unit *unit;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "scope.eo")));
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Scope")));

   /* Property scope */
    efl_assert_fail_if(eolian_class_function_by_name_get(class, "a", EOLIAN_PROPERTY));
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "a", EOLIAN_PROP_GET)));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_PROP_GET) !=
            EOLIAN_SCOPE_PROTECTED);
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "b", EOLIAN_PROP_GET)));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_PROP_GET) !=
            EOLIAN_SCOPE_PUBLIC);
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "c", EOLIAN_PROPERTY)));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_PROP_GET) !=
            EOLIAN_SCOPE_PUBLIC);
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_PROP_SET) !=
            EOLIAN_SCOPE_PROTECTED);

   /* Method scope */
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD)));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_METHOD) !=
            EOLIAN_SCOPE_PUBLIC);
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "bar", EOLIAN_METHOD)));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_METHOD) !=
            EOLIAN_SCOPE_PROTECTED);
    efl_assert_fail_if(!(
        fid =
            eolian_class_function_by_name_get(class, "foobar", EOLIAN_METHOD)));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_METHOD) !=
            EOLIAN_SCOPE_PUBLIC);

    eolian_state_free(eos);
}

TEST(eolian_simple_parsing)
{
    const Eolian_Function           *fid    = NULL;
    const char                      *string = NULL, *ptype = NULL;
    const Eolian_Function_Parameter *param = NULL;
    const Eolian_Expression         *expr  = NULL;
    const Eolian_Class *class;
    const Eolian_Type *tp;
    const Eolian_Unit *unit;
    Eina_Iterator     *iter;
    Eolian_Value       v, vv;
    void              *dummy;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "class_simple.eo")));
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Class_Simple")));
    efl_assert_fail_if(eolian_state_class_by_file_get(eos, "class_simple.eo") != class);
    efl_assert_fail_if(strcmp(eolian_object_file_get((const Eolian_Object *)class),
                   "class_simple.eo"));

   /* Class */
    efl_assert_fail_if(eolian_class_type_get(class) != EOLIAN_CLASS_REGULAR);
    efl_assert_fail_if(eolian_class_parent_get(class) != NULL);
    efl_assert_fail_if(eolian_class_extensions_get(class) != NULL);
    efl_assert_fail_if(
        strcmp(eolian_class_c_prefix_get(class), "efl_canvas_object_simple"));
    efl_assert_fail_if(strcmp(eolian_class_data_type_get(class), "Evas_Simple_Data"));
    Eina_Stringshare *dt = eolian_class_c_data_type_get(class);
    efl_assert_fail_if(strcmp(dt, "Evas_Simple_Data"));
    eina_stringshare_del(dt);

   /* c get func */
    efl_assert_fail_if(!(string = eolian_class_c_get_function_name_get(class)));
    efl_assert_fail_if(strcmp(string, "class_simple_class_get"));
    eina_stringshare_del(string);

   /* c name */
    efl_assert_fail_if(!(string = eolian_class_c_macro_get(class)));
    efl_assert_fail_if(strcmp(string, "CLASS_SIMPLE_CLASS"));
    eina_stringshare_del(string);
    efl_assert_fail_if(strcmp(eolian_class_c_name_get(class), "Class_Simple"));

   /* Property */
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "a", EOLIAN_PROPERTY)));
    efl_assert_fail_if(strcmp(eolian_function_name_get(fid), "a"));
    efl_assert_fail_if(!eolian_function_is_beta(fid));
    efl_assert_fail_if(eolian_function_class_get(fid) != class);
   /* Set return */
    tp = eolian_function_return_type_get(fid, EOLIAN_PROP_SET);
    efl_assert_fail_if(!tp);
    efl_assert_fail_if(eolian_type_builtin_type_get(tp) != EOLIAN_TYPE_BUILTIN_BOOL);
    efl_assert_fail_if(strcmp(eolian_type_short_name_get(tp), "bool"));
    expr = eolian_function_return_default_value_get(fid, EOLIAN_PROP_SET);
    efl_assert_fail_if(!expr);
    v = eolian_expression_eval(expr, EOLIAN_MASK_BOOL);
    efl_assert_fail_if(!eolian_expression_eval_fill(expr, EOLIAN_MASK_BOOL, &vv));
    efl_assert_fail_if(v.type != EOLIAN_EXPR_BOOL);
    efl_assert_fail_if(vv.type != EOLIAN_EXPR_BOOL);
   /* Get return */
    tp = eolian_function_return_type_get(fid, EOLIAN_PROP_GET);
    efl_assert_fail_if(tp);

   /* Function parameters */
    efl_assert_fail_if(eolian_property_keys_get(fid, EOLIAN_PROP_GET) != NULL);
    efl_assert_fail_if(!(iter = eolian_property_values_get(fid, EOLIAN_PROP_GET)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);
    efl_assert_fail_if(strcmp(eolian_type_short_name_get(eolian_parameter_type_get(param)),
                   "int"));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "value"));
    expr = eolian_parameter_default_value_get(param);
    efl_assert_fail_if(!expr);
    v = eolian_expression_eval(expr, EOLIAN_MASK_INT);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != 100);

   /* Method */
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD)));
    efl_assert_fail_if(!eolian_function_is_beta(fid));
   /* Function return */
    tp = eolian_function_return_type_get(fid, EOLIAN_METHOD);
    efl_assert_fail_if(!tp);
    string = eolian_function_return_c_type_get(fid, EOLIAN_METHOD);
    efl_assert_fail_if(!string);
    efl_assert_fail_if(strcmp(string, "char *"));
    eina_stringshare_del(string);
    expr = eolian_function_return_default_value_get(fid, EOLIAN_METHOD);
    efl_assert_fail_if(!expr);
    v = eolian_expression_eval(expr, EOLIAN_MASK_NULL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_NULL);

   /* Function parameters */
    efl_assert_fail_if(!(iter = eolian_function_parameters_get(fid)));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_IN);
    efl_assert_fail_if(strcmp(eolian_type_short_name_get(eolian_parameter_type_get(param)),
                   "int"));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "a"));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_INOUT);
    ptype = eolian_type_short_name_get(eolian_parameter_type_get(param));
    efl_assert_fail_if(strcmp(ptype, "char"));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "b"));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_OUT);
    efl_assert_fail_if(eolian_type_builtin_type_get(eolian_parameter_type_get(param)) !=
            EOLIAN_TYPE_BUILTIN_DOUBLE);
    efl_assert_fail_if(strcmp(eolian_type_short_name_get(eolian_parameter_type_get(param)),
                   "double"));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "c"));
    expr = eolian_parameter_default_value_get(param);
    efl_assert_fail_if(!expr);
    v = eolian_expression_eval(expr, EOLIAN_MASK_FLOAT);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_DOUBLE);
    efl_assert_fail_if(!EINA_DBL_EQ(v.value.d, 1337.6));
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_IN);
    efl_assert_fail_if(strcmp(eolian_type_short_name_get(eolian_parameter_type_get(param)),
                   "int"));
    efl_assert_fail_if(!eolian_parameter_is_by_ref(param));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "d"));
    efl_assert_fail_if(eina_iterator_next(iter, &dummy));
    eina_iterator_free(iter);

    eolian_state_free(eos);
}

TEST(eolian_struct)
{
    const Eolian_Struct_Type_Field *field = NULL;
    const Eolian_Type              *type = NULL, *ftype = NULL;
    const Eolian_Typedecl          *tdl = NULL;
    const Eolian_Class *class;
    const Eolian_Function *func;
    const Eolian_Unit     *unit;
    const char            *type_name;
    const char            *file;
    Eina_Iterator         *structs;

    Eolian_State *eos = eolian_state_new();

   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "struct.eo")));

   /* Check that the class Dummy is still readable */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Struct")));
    efl_assert_fail_if(!eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD));

    efl_assert_fail_if(!(structs = eolian_state_structs_by_file_get(eos, "struct.eo")));
    eina_iterator_free(structs);

   /* named struct */
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Named")));
    efl_assert_fail_if(!(type_name = eolian_typedecl_short_name_get(tdl)));
    efl_assert_fail_if(!(file = eolian_object_file_get((const Eolian_Object *)tdl)));
    efl_assert_fail_if(eolian_typedecl_type_get(tdl) != EOLIAN_TYPEDECL_STRUCT);
    efl_assert_fail_if(strcmp(type_name, "Named"));
    efl_assert_fail_if(strcmp(file, "struct.eo"));
    efl_assert_fail_if(!(field = eolian_typedecl_struct_field_get(tdl, "field")));
    efl_assert_fail_if(!(ftype = eolian_typedecl_struct_field_type_get(field)));
    efl_assert_fail_if(!eolian_typedecl_struct_field_is_by_ref(field));
    efl_assert_fail_if(!(type_name = eolian_type_short_name_get(ftype)));
    efl_assert_fail_if(strcmp(type_name, "int"));
    efl_assert_fail_if(!(field = eolian_typedecl_struct_field_get(tdl, "something")));
    efl_assert_fail_if(!(ftype = eolian_typedecl_struct_field_type_get(field)));
    efl_assert_fail_if(eolian_typedecl_struct_field_is_by_ref(field));
    efl_assert_fail_if(!(type_name = eolian_typedecl_struct_field_c_type_get(field)));
    efl_assert_fail_if(strcmp(type_name, "const char *"));
    eina_stringshare_del(type_name);

   /* referencing */
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Another")));
    efl_assert_fail_if(!(type_name = eolian_typedecl_short_name_get(tdl)));
    efl_assert_fail_if(!(file = eolian_object_file_get((const Eolian_Object *)tdl)));
    efl_assert_fail_if(eolian_typedecl_type_get(tdl) != EOLIAN_TYPEDECL_STRUCT);
    efl_assert_fail_if(strcmp(type_name, "Another"));
    efl_assert_fail_if(strcmp(file, "struct.eo"));
    efl_assert_fail_if(!(field = eolian_typedecl_struct_field_get(tdl, "field")));
    efl_assert_fail_if(!(ftype = eolian_typedecl_struct_field_type_get(field)));
    efl_assert_fail_if(!(type_name = eolian_type_short_name_get(ftype)));
    efl_assert_fail_if(strcmp(type_name, "Named"));
    efl_assert_fail_if(eolian_type_type_get(ftype) != EOLIAN_TYPE_REGULAR);
    efl_assert_fail_if(eolian_typedecl_type_get(eolian_type_typedecl_get(ftype)) !=
            EOLIAN_TYPEDECL_STRUCT);

   /* opaque struct */
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Opaque")));
    efl_assert_fail_if(eolian_typedecl_type_get(tdl) != EOLIAN_TYPEDECL_STRUCT_OPAQUE);

   /* use in function */
    efl_assert_fail_if(!(
        func = eolian_class_function_by_name_get(class, "bar", EOLIAN_METHOD)));
    efl_assert_fail_if(!(type = eolian_function_return_type_get(func, EOLIAN_METHOD)));
    efl_assert_fail_if(eolian_type_type_get(type) != EOLIAN_TYPE_REGULAR);
    efl_assert_fail_if(!eolian_function_return_is_by_ref(func, EOLIAN_METHOD));
    efl_assert_fail_if(!(tdl = eolian_type_typedecl_get(type)));
    efl_assert_fail_if(eolian_typedecl_type_get(tdl) != EOLIAN_TYPEDECL_STRUCT);

    eolian_state_free(eos);
}

TEST(eolian_extern)
{
    const Eolian_Typedecl *tdl = NULL;
    const Eolian_Class *class;
    const Eolian_Unit *unit;

    Eolian_State *eos = eolian_state_new();

   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "extern.eo")));

   /* Check that the class Dummy is still readable */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Extern")));
    efl_assert_fail_if(!eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD));

   /* regular type */
    efl_assert_fail_if(!(tdl = eolian_unit_alias_by_name_get(unit, "Foo")));
    efl_assert_fail_if(eolian_typedecl_is_extern(tdl));

   /* extern type */
    efl_assert_fail_if(!(tdl = eolian_unit_alias_by_name_get(unit, "Evas.Coord")));
    efl_assert_fail_if(!eolian_typedecl_is_extern(tdl));

   /* regular struct */
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "X")));
    efl_assert_fail_if(eolian_typedecl_is_extern(tdl));

   /* extern struct */
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Y")));
    efl_assert_fail_if(!eolian_typedecl_is_extern(tdl));

    eolian_state_free(eos);
}

TEST(eolian_var)
{
    const Eolian_Constant   *var  = NULL;
    const Eolian_Expression *exp  = NULL;
    const Eolian_Type       *type = NULL;
    const Eolian_Class *class;
    const Eolian_Unit *unit;
    Eolian_Value       v;
    const char        *name;

    Eolian_State *eos = eolian_state_new();

   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "var.eo")));

   /* Check that the class Dummy is still readable */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Var")));
    efl_assert_fail_if(!eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD));

   /* regular constant */
    efl_assert_fail_if(!(var = eolian_unit_constant_by_name_get(unit, "Foo")));
    efl_assert_fail_if(eolian_constant_is_extern(var));
    efl_assert_fail_if(!(type = eolian_constant_type_get(var)));
    efl_assert_fail_if(!(name = eolian_type_short_name_get(type)));
    efl_assert_fail_if(strcmp(name, "int"));
    efl_assert_fail_if(!(exp = eolian_constant_value_get(var)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != 5);

    eolian_state_free(eos);
}

TEST(eolian_error)
{
    const Eolian_Unit *unit;
    const Eolian_Class *class;
    const Eolian_Function *f1, *f2;
    const Eolian_Type     *rtp1, *rtp2;
    const Eolian_Error    *err1, *err2;
    Eina_Stringshare      *str;

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "error.eo")));

    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Error")));
    efl_assert_fail_if(
        !(f1 = eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD)));
    efl_assert_fail_if(
        !(f2 = eolian_class_function_by_name_get(class, "bar", EOLIAN_METHOD)));

    efl_assert_fail_if(!(rtp1 = eolian_function_return_type_get(f1, EOLIAN_METHOD)));
    efl_assert_fail_if(!(rtp2 = eolian_function_return_type_get(f2, EOLIAN_METHOD)));

   /* single error */
    efl_assert_fail_if(eolian_type_type_get(rtp1) != EOLIAN_TYPE_ERROR);
    efl_assert_fail_if(eolian_type_next_type_get(rtp1) != NULL);
    efl_assert_fail_if(strcmp(eolian_type_name_get(rtp1), "Foo"));
    efl_assert_fail_if(!(str = eolian_type_c_type_get(rtp1)));
    efl_assert_fail_if(strcmp(str, "Eina_Error"));
    eina_stringshare_del(str);
    efl_assert_fail_if(!(err1 = eolian_type_error_get(rtp1)));
    efl_assert_fail_if(strcmp(eolian_error_message_get(err1), "something bad happened"));

   /* error range */
    efl_assert_fail_if(eolian_type_type_get(rtp2) != EOLIAN_TYPE_ERROR);
    efl_assert_fail_if(!(rtp1 = eolian_type_next_type_get(rtp2)));
    efl_assert_fail_if(strcmp(eolian_type_name_get(rtp2), "Foo"));
    efl_assert_fail_if(strcmp(eolian_type_name_get(rtp1), "Bar"));
   /* it's the same Foo here */
    efl_assert_fail_if(eolian_type_error_get(rtp2) != err1);
    efl_assert_fail_if(!(err2 = eolian_type_error_get(rtp1)));
    efl_assert_fail_if(strcmp(eolian_error_message_get(err1), "something bad happened"));
    efl_assert_fail_if(
        strcmp(eolian_error_message_get(err2), "another bad thing happened"));

    efl_assert_fail_if(!eolian_error_documentation_get(err1));
    efl_assert_fail_if(!eolian_error_documentation_get(err2));
    efl_assert_fail_if(eolian_error_is_beta(err1));
    efl_assert_fail_if(!eolian_error_is_beta(err2));

    eolian_state_free(eos);
}

TEST(eolian_enum)
{
    const Eolian_Enum_Type_Field *field = NULL;
    const Eolian_Constant        *var   = NULL;
    const Eolian_Typedecl        *tdl   = NULL;
    const Eolian_Type            *type  = NULL;
    const Eolian_Class *class;
    const Eolian_Expression *exp;
    const Eolian_Unit       *unit;
    Eina_Stringshare        *cname;
    const char              *name;
    Eolian_Value             v;

    Eolian_State *eos = eolian_state_new();

   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "enum.eo")));

   /* Check that the class Dummy is still readable */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Enum")));
    efl_assert_fail_if(!eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD));

    efl_assert_fail_if(!(tdl = eolian_unit_enum_by_name_get(unit, "Foo")));

    efl_assert_fail_if(!(field = eolian_typedecl_enum_field_get(tdl, "first")));
    efl_assert_fail_if(!(exp = eolian_typedecl_enum_field_value_get(field, EFL_FALSE)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != 0);

    efl_assert_fail_if(!(field = eolian_typedecl_enum_field_get(tdl, "bar")));
    efl_assert_fail_if(eolian_typedecl_enum_field_value_get(field, EFL_FALSE));

    efl_assert_fail_if(!(field = eolian_typedecl_enum_field_get(tdl, "baz")));
    efl_assert_fail_if(!(exp = eolian_typedecl_enum_field_value_get(field, EFL_FALSE)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != 15);

    efl_assert_fail_if(!(tdl = eolian_unit_enum_by_name_get(unit, "Baz")));

    efl_assert_fail_if(!(field = eolian_typedecl_enum_field_get(tdl, "flag1")));
    efl_assert_fail_if(!(exp = eolian_typedecl_enum_field_value_get(field, EFL_FALSE)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != (1 << 0));

    efl_assert_fail_if(!(field = eolian_typedecl_enum_field_get(tdl, "flag2")));
    efl_assert_fail_if(!(exp = eolian_typedecl_enum_field_value_get(field, EFL_FALSE)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != (1 << 1));

    efl_assert_fail_if(!(field = eolian_typedecl_enum_field_get(tdl, "flag3")));
    efl_assert_fail_if(!(exp = eolian_typedecl_enum_field_value_get(field, EFL_FALSE)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != (1 << 2));

    efl_assert_fail_if(!(tdl = eolian_unit_enum_by_name_get(unit, "Name.Spaced")));
    efl_assert_fail_if(!(field = eolian_typedecl_enum_field_get(tdl, "pants")));

    cname = eolian_typedecl_enum_field_c_constant_get(field);
    efl_assert_fail_if(strcmp(cname, "NAME_SPACED_PANTS"));
    eina_stringshare_del(cname);

    efl_assert_fail_if(!(var = eolian_unit_constant_by_name_get(unit, "Bah")));
    efl_assert_fail_if(!(type = eolian_constant_type_get(var)));
    efl_assert_fail_if(!(name = eolian_type_short_name_get(type)));
    efl_assert_fail_if(strcmp(name, "Baz"));
    efl_assert_fail_if(!(exp = eolian_constant_value_get(var)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != (1 << 0));

    efl_assert_fail_if(!(var = eolian_unit_constant_by_name_get(unit, "Pants")));
    efl_assert_fail_if(!(exp = eolian_constant_value_get(var)));
    v = eolian_expression_eval(exp, EOLIAN_MASK_ALL);
    efl_assert_fail_if(v.type != EOLIAN_EXPR_INT);
    efl_assert_fail_if(v.value.i != 5);

    eolian_state_free(eos);
}

TEST(eolian_class_funcs)
{
    const Eolian_Function *fid = NULL;
    const Eolian_Class *class;
    const Eolian_Unit *unit;

    Eolian_State *eos = eolian_state_new();
   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "class_funcs.eo")));
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Class_Funcs")));

   /* Class properties */
    efl_assert_fail_if(eolian_class_function_by_name_get(class, "a", EOLIAN_PROPERTY));
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "a", EOLIAN_PROP_GET)));
    efl_assert_fail_if(!eolian_function_is_static(fid));
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "b", EOLIAN_PROP_GET)));
    efl_assert_fail_if(eolian_function_is_static(fid));

   /* Class methods */
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD)));
    efl_assert_fail_if(!eolian_function_is_static(fid));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_METHOD) !=
            EOLIAN_SCOPE_PUBLIC);
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "bar", EOLIAN_METHOD)));
    efl_assert_fail_if(eolian_function_is_static(fid));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_METHOD) !=
            EOLIAN_SCOPE_PUBLIC);
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "baz", EOLIAN_METHOD)));
    efl_assert_fail_if(!eolian_function_is_static(fid));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_METHOD) !=
            EOLIAN_SCOPE_PROTECTED);
    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "bah", EOLIAN_METHOD)));
    efl_assert_fail_if(eolian_function_is_static(fid));
    efl_assert_fail_if(eolian_function_scope_get(fid, EOLIAN_METHOD) !=
            EOLIAN_SCOPE_PROTECTED);

    eolian_state_free(eos);
}

TEST(eolian_free_func)
{
    const Eolian_Class *class;
    const Eolian_Typedecl *tdl;
    const Eolian_Unit     *unit;

    Eolian_State *eos = eolian_state_new();

   /* Parsing */
    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "free_func.eo")));

   /* Check that the class Dummy is still readable */
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Free_Func")));
    efl_assert_fail_if(!eolian_class_function_by_name_get(class, "foo", EOLIAN_METHOD));

   /* regular struct */
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Named1")));
    efl_assert_fail_if(eolian_typedecl_free_func_get(tdl));
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Named2")));
    efl_assert_fail_if(strcmp(eolian_typedecl_free_func_get(tdl), "test_free"));

   /* opaque struct */
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Opaque1")));
    efl_assert_fail_if(eolian_typedecl_free_func_get(tdl));
    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Opaque2")));
    efl_assert_fail_if(strcmp(eolian_typedecl_free_func_get(tdl), "opaque_free"));

    eolian_state_free(eos);
}

TEST(eolian_parse_import)
{
    const Eolian_Class *class;
    const Eolian_Typedecl *tdl;
    const Eolian_Unit     *unit;

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "import.eo")));
    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Import")));

    efl_assert_fail_if(!(tdl = eolian_unit_alias_by_name_get(unit, "Imported")));
    efl_assert_fail_if(strcmp(eolian_object_file_get((const Eolian_Object *)tdl),
                   "import_types.eot"));

    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Imported_Struct")));
    efl_assert_fail_if(strcmp(eolian_object_file_get((const Eolian_Object *)tdl),
                   "import_types.eot"));

    eolian_state_free(eos);
}

TEST(eolian_parse_docs)
{
    const Eolian_Typedecl *tdl;
    const Eolian_Class *class;
    const Eolian_Event              *event;
    const Eolian_Constant           *var;
    const Eolian_Function           *fid;
    const Eolian_Documentation      *doc;
    const Eolian_Function_Parameter *par;
    const Eolian_Struct_Type_Field  *sfl;
    const Eolian_Enum_Type_Field    *efl;
    const Eolian_Unit               *unit;
    Eina_Iterator                   *itr;

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "eo_docs.eo")));

    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Foo")));
    efl_assert_fail_if(!(doc = eolian_typedecl_documentation_get(tdl)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc),
                   "This is struct Foo. It does stuff."));
    efl_assert_fail_if(strcmp(eolian_documentation_description_get(doc),
                   "Note: This is a note.\n\n"
                   "This is a longer description for struct Foo.\n\n"
                   "Warning: This is a warning. You can only use Warning: "
                   "and Note: at the beginning of a paragraph.\n\n"
                   "This is another paragraph."));
    efl_assert_fail_if(strcmp(eolian_documentation_since_get(doc), "1.66"));

    const char *sdesc = eolian_documentation_description_get(doc);
    Eina_List  *sdoc  = eolian_documentation_string_split(sdesc);

    char *dpar = eina_list_data_get(sdoc);
    efl_assert_fail_if(strcmp(dpar, "Note: This is a note."));
    sdoc = eina_list_remove_list(sdoc, sdoc);
    dpar = eina_list_data_get(sdoc);
    efl_assert_fail_if(strcmp(dpar, "This is a longer description for struct Foo."));
    EINA_LIST_FREE(sdoc, dpar)
    free(dpar);

    const char *tdoc =
        "Note: This is $something, see @pants, "
        "@Eo_Docs.meth, \\@ref foo and @[Things.Stuffs.foo,bar].";

    Eolian_Doc_Token tok;
    eolian_doc_token_init(&tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_UNKNOWN);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_MARK_NOTE);
    char *txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, "Note: "));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_TEXT);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, "This is "));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) !=
            EOLIAN_DOC_TOKEN_MARKUP_MONOSPACE);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, "something"));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_TEXT);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, ", see "));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_REF);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, "pants"));
    efl_assert_fail_if(eolian_doc_token_ref_resolve(&tok, eos, NULL, NULL) !=
            EOLIAN_OBJECT_CONSTANT);
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_TEXT);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, ", "));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_REF);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, "Eo_Docs.meth"));
    efl_assert_fail_if(eolian_doc_token_ref_resolve(&tok, eos, NULL, NULL) !=
            EOLIAN_OBJECT_FUNCTION);
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_TEXT);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, ", @ref foo and "));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_REF);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, "[Things.Stuffs.foo,bar]"));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(tdoc[0] != '\0');
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_TEXT);
    txt = eolian_doc_token_text_get(&tok);
    efl_assert_fail_if(strcmp(txt, "."));
    free(txt);
    tdoc = eolian_documentation_tokenize(tdoc, &tok);
    efl_assert_fail_if(tdoc != NULL);
    efl_assert_fail_if(eolian_doc_token_type_get(&tok) != EOLIAN_DOC_TOKEN_UNKNOWN);

    efl_assert_fail_if(!(sfl = eolian_typedecl_struct_field_get(tdl, "field1")));
    efl_assert_fail_if(!(doc = eolian_typedecl_struct_field_documentation_get(sfl)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Field documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(sfl = eolian_typedecl_struct_field_get(tdl, "field2")));
    efl_assert_fail_if(eolian_typedecl_struct_field_documentation_get(sfl));

    efl_assert_fail_if(!(sfl = eolian_typedecl_struct_field_get(tdl, "field3")));
    efl_assert_fail_if(!(doc = eolian_typedecl_struct_field_documentation_get(sfl)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc),
                   "Another field documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(tdl = eolian_unit_enum_by_name_get(unit, "Bar")));
    efl_assert_fail_if(!(doc = eolian_typedecl_documentation_get(tdl)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Docs for enum Bar."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));
    efl_assert_fail_if(strcmp(eolian_documentation_since_get(doc), "1.55"));

    efl_assert_fail_if(!(efl = eolian_typedecl_enum_field_get(tdl, "blah")));
    efl_assert_fail_if(eolian_typedecl_enum_field_documentation_get(efl));

    efl_assert_fail_if(!(efl = eolian_typedecl_enum_field_get(tdl, "foo")));
    efl_assert_fail_if(!(doc = eolian_typedecl_enum_field_documentation_get(efl)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc), "Docs for foo."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(efl = eolian_typedecl_enum_field_get(tdl, "bar")));
    efl_assert_fail_if(!(doc = eolian_typedecl_enum_field_documentation_get(efl)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc), "Docs for bar."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(tdl = eolian_unit_alias_by_name_get(unit, "Alias")));
    efl_assert_fail_if(!(doc = eolian_typedecl_documentation_get(tdl)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc), "Docs for typedef."));
    efl_assert_fail_if(strcmp(eolian_documentation_description_get(doc),
                   "More docs for typedef. See @Bar."));
    efl_assert_fail_if(strcmp(eolian_documentation_since_get(doc), "1.1337"));

    efl_assert_fail_if(!(var = eolian_unit_constant_by_name_get(unit, "pants")));
    efl_assert_fail_if(!(doc = eolian_constant_documentation_get(var)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc), "Docs for var."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(tdl = eolian_unit_struct_by_name_get(unit, "Opaque")));
    efl_assert_fail_if(!(doc = eolian_typedecl_documentation_get(tdl)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc),
                   "Opaque struct docs. See @Foo for another struct."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(class = eolian_unit_class_by_name_get(unit, "Eo_Docs")));
    efl_assert_fail_if(!(doc = eolian_class_documentation_get(class)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc), "Docs for class."));
    efl_assert_fail_if(strcmp(
        eolian_documentation_description_get(doc),
        "More docs for class. Testing references now. "
        "@Foo @Bar @Alias @pants @Eo_Docs.meth @Eo_Docs.prop "
        "@Eo_Docs.prop.get @Eo_Docs.prop.set @Foo.field1 @Bar.foo @Eo_Docs"));

    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(class, "meth", EOLIAN_METHOD)));
    const Eolian_Implement *fimp = eolian_function_implement_get(fid);
    efl_assert_fail_if(!(doc = eolian_implement_documentation_get(fimp, EOLIAN_METHOD)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Method documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(itr = eolian_function_parameters_get(fid)));

    efl_assert_fail_if(!eina_iterator_next(itr, (void **)&par));
    efl_assert_fail_if(!(doc = eolian_parameter_documentation_get(par)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Param documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!eina_iterator_next(itr, (void **)&par));
    efl_assert_fail_if(eolian_parameter_documentation_get(par));

    efl_assert_fail_if(!eina_iterator_next(itr, (void **)&par));
    efl_assert_fail_if(!(doc = eolian_parameter_documentation_get(par)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc),
                   "Another param documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(eina_iterator_next(itr, (void **)&par));
    eina_iterator_free(itr);

    efl_assert_fail_if(
        !(doc = eolian_function_return_documentation_get(fid, EOLIAN_METHOD)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Return documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(
        fid =
            eolian_class_function_by_name_get(class, "prop", EOLIAN_PROPERTY)));
    fimp = eolian_function_implement_get(fid);
    efl_assert_fail_if(!(doc = eolian_implement_documentation_get(fimp, EOLIAN_PROPERTY)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc),
                   "Property common documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));
    efl_assert_fail_if(strcmp(eolian_documentation_since_get(doc), "1.66"));
    efl_assert_fail_if(!(doc = eolian_implement_documentation_get(fimp, EOLIAN_PROP_GET)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Get documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));
    efl_assert_fail_if(!(doc = eolian_implement_documentation_get(fimp, EOLIAN_PROP_SET)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Set documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(itr = eolian_property_values_get(fid, EOLIAN_PROP_GET)));

    efl_assert_fail_if(!eina_iterator_next(itr, (void **)&par));
    efl_assert_fail_if(!(doc = eolian_parameter_documentation_get(par)));
    efl_assert_fail_if(
        strcmp(eolian_documentation_summary_get(doc), "Value documentation."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(eina_iterator_next(itr, (void **)&par));
    eina_iterator_free(itr);

    efl_assert_fail_if(!(event = eolian_class_event_by_name_get(class, "clicked")));
    efl_assert_fail_if(!(doc = eolian_event_documentation_get(event)));
    efl_assert_fail_if(strcmp(eolian_documentation_summary_get(doc), "Event docs."));
    efl_assert_fail_if(eolian_documentation_description_get(doc));

    efl_assert_fail_if(!(fid = eolian_class_function_by_name_get(class,
                                                      "no_doc_meth",
                                                      EOLIAN_METHOD)));
    efl_assert_fail_if(!(fimp = eolian_function_implement_get(fid)));
    efl_assert_fail_if((doc = eolian_implement_documentation_get(fimp, EOLIAN_METHOD)));

    efl_assert_fail_if(!(fid = eolian_class_function_by_name_get(class,
                                                      "doc_with_empty_doc",
                                                      EOLIAN_METHOD)));
    efl_assert_fail_if(!(fimp = eolian_function_implement_get(fid)));
    efl_assert_fail_if(!(doc = eolian_implement_documentation_get(fimp, EOLIAN_METHOD)));
    efl_assert_fail_if(eolian_documentation_summary_get(doc));

    eolian_state_free(eos);
}

TEST(eolian_function_types)
{
    const Eolian_Typedecl           *decl      = NULL;
    const Eolian_Typedecl           *arg_decl  = NULL;
    const Eolian_Type               *type      = NULL;
    const Eolian_Function           *fid       = NULL;
    const Eolian_Function_Parameter *param     = NULL;
    const Eolian_Unit               *unit      = NULL;
    Eina_Iterator                   *iter      = NULL;
    const char                      *type_name = NULL;
    void                            *dummy;

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

   /* Parsing */
    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "function_types.eot")));

   /* void func(void); */
    efl_assert_fail_if(!(decl = eolian_unit_alias_by_name_get(unit, "VoidFunc")));
    efl_assert_fail_if(eolian_typedecl_type_get(decl) != EOLIAN_TYPEDECL_FUNCTION_POINTER);

    efl_assert_fail_if(!(fid = eolian_typedecl_function_pointer_get(decl)));
    efl_assert_fail_if(eolian_function_type_get(fid) != EOLIAN_FUNCTION_POINTER);

    efl_assert_fail_if((eolian_function_return_type_get(
        fid,
        EOLIAN_FUNCTION_POINTER))); // void is null_return_type?
    efl_assert_fail_if((eolian_function_parameters_get(fid)));

   /* Function pointer with return and parameters */
    efl_assert_fail_if(!(decl = eolian_unit_alias_by_name_get(unit, "SimpleFunc")));
    efl_assert_fail_if(eolian_typedecl_type_get(decl) != EOLIAN_TYPEDECL_FUNCTION_POINTER);

    efl_assert_fail_if(!(fid = eolian_typedecl_function_pointer_get(decl)));
    efl_assert_fail_if(eolian_function_type_get(fid) != EOLIAN_FUNCTION_POINTER);
    efl_assert_fail_if(strcmp(eolian_function_name_get(fid), "SimpleFunc"));

    efl_assert_fail_if(!(type = eolian_function_return_type_get(
                  fid,
                  EOLIAN_FUNCTION_POINTER))); // void is null_return_type?
    efl_assert_fail_if(
        !(type_name =
              eolian_function_return_c_type_get(fid, EOLIAN_FUNCTION_POINTER)));
    efl_assert_fail_if(strcmp(type_name, "const char *"));
    efl_assert_fail_if(!(iter = (eolian_function_parameters_get(fid))));

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "a"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(!(type_name = eolian_parameter_c_type_get(param, EFL_FALSE)));
    efl_assert_fail_if(strcmp(type_name, "int"));

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "b"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(!(type_name = eolian_parameter_c_type_get(param, EFL_FALSE)));
    efl_assert_fail_if(strcmp(type_name, "double"));

    efl_assert_fail_if(eina_iterator_next(iter, &dummy));

   /* Function pointer with parameter attributes (in/out) */
    efl_assert_fail_if(!(decl = eolian_unit_alias_by_name_get(unit, "ComplexFunc")));
    efl_assert_fail_if(eolian_typedecl_type_get(decl) != EOLIAN_TYPEDECL_FUNCTION_POINTER);

    efl_assert_fail_if(!(fid = eolian_typedecl_function_pointer_get(decl)));
    efl_assert_fail_if(eolian_function_type_get(fid) != EOLIAN_FUNCTION_POINTER);

    efl_assert_fail_if(!(
        type = eolian_function_return_type_get(fid, EOLIAN_FUNCTION_POINTER)));
    efl_assert_fail_if(
        !(type_name =
              eolian_function_return_c_type_get(fid, EOLIAN_FUNCTION_POINTER)));
    efl_assert_fail_if(strcmp(type_name, "double"));
    efl_assert_fail_if(!(iter = (eolian_function_parameters_get(fid))));

   /* in string */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "c"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_IN);
    efl_assert_fail_if(eolian_parameter_is_move(param));
    efl_assert_fail_if(!(type_name = eolian_parameter_c_type_get(param, EFL_FALSE)));
    efl_assert_fail_if(strcmp(type_name, "const char *"));

   /*out own string */
    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "d"));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_OUT);
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(!eolian_parameter_is_move(param));
    efl_assert_fail_if(!(type_name = eolian_parameter_c_type_get(param, EFL_FALSE)));
    efl_assert_fail_if(strcmp(type_name, "char *"));

    efl_assert_fail_if(eina_iterator_next(iter, &dummy));

   /* Function pointer receiving another function pointer */
    efl_assert_fail_if(!(decl = eolian_unit_alias_by_name_get(unit, "FuncAsArgFunc")));
    efl_assert_fail_if(eolian_typedecl_type_get(decl) != EOLIAN_TYPEDECL_FUNCTION_POINTER);

    efl_assert_fail_if(!(fid = eolian_typedecl_function_pointer_get(decl)));
    efl_assert_fail_if(eolian_function_type_get(fid) != EOLIAN_FUNCTION_POINTER);

    efl_assert_fail_if(eolian_function_return_type_get(fid, EOLIAN_FUNCTION_POINTER));

    efl_assert_fail_if(!(iter = (eolian_function_parameters_get(fid))));

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "cb"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_IN);
    efl_assert_fail_if(eolian_parameter_is_move(param));
    efl_assert_fail_if(!(type_name = eolian_type_short_name_get(type)));
    efl_assert_fail_if(strcmp(type_name, "VoidFunc"));
    efl_assert_fail_if(!(arg_decl = eolian_type_typedecl_get(type)));
    efl_assert_fail_if(eolian_typedecl_type_get(arg_decl) !=
            EOLIAN_TYPEDECL_FUNCTION_POINTER);

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "another_cb"));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_IN);
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(eolian_parameter_is_move(param));
    efl_assert_fail_if(eolian_type_type_get(type) != EOLIAN_TYPE_REGULAR);
    efl_assert_fail_if(!(type_name = eolian_type_short_name_get(type)));
    efl_assert_fail_if(strcmp(type_name, "SimpleFunc"));
    efl_assert_fail_if(!(arg_decl = eolian_type_typedecl_get(type)));
    efl_assert_fail_if(eolian_typedecl_type_get(arg_decl) !=
            EOLIAN_TYPEDECL_FUNCTION_POINTER);

    efl_assert_fail_if(eina_iterator_next(iter, &dummy));

    eolian_state_free(eos);
}

TEST(eolian_function_as_arguments)
{
    const Eolian_Class              *cls       = NULL;
    const Eolian_Function           *fid       = NULL;
    const Eolian_Type               *type      = NULL;
    const Eolian_Typedecl           *arg_decl  = NULL;
    const Eolian_Function_Parameter *param     = NULL;
    const Eolian_Unit               *unit      = NULL;
    Eina_Iterator                   *iter      = NULL;
    const char                      *type_name = NULL;
    void                            *dummy;

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "function_as_argument.eo")));

    efl_assert_fail_if(
        !(cls = eolian_unit_class_by_name_get(unit, "Function_As_Argument")));

    efl_assert_fail_if(!(
        fid = eolian_class_function_by_name_get(cls, "set_cb", EOLIAN_METHOD)));

    efl_assert_fail_if(!(iter = (eolian_function_parameters_get(fid))));

    efl_assert_fail_if(!(eina_iterator_next(iter, (void **)&param)));
    efl_assert_fail_if(strcmp(eolian_parameter_name_get(param), "cb"));
    efl_assert_fail_if(!(type = eolian_parameter_type_get(param)));
    efl_assert_fail_if(eolian_parameter_direction_get(param) != EOLIAN_PARAMETER_IN);
    efl_assert_fail_if(eolian_parameter_is_move(param));
    efl_assert_fail_if(!(type_name = eolian_type_short_name_get(type)));
    efl_assert_fail_if(strcmp(type_name, "SimpleFunc"));
    efl_assert_fail_if(!(arg_decl = eolian_type_typedecl_get(type)));
    efl_assert_fail_if(eolian_typedecl_type_get(arg_decl) !=
            EOLIAN_TYPEDECL_FUNCTION_POINTER);

    efl_assert_fail_if(eina_iterator_next(iter, &dummy));

    eolian_state_free(eos);
}

TEST(eolian_parts)
{
    const Eolian_Unit  *unit;
    const Eolian_Class *cls;
    Eina_Iterator      *iter;
    Eolian_Part        *part;
    int                 i = 0;

    static const char *part_classes[] = { "Override", "Parts" };

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "parts.eo")));

    efl_assert_fail_if(!(cls = eolian_unit_class_by_name_get(unit, "Parts")));

    efl_assert_fail_if(!(iter = eolian_class_parts_get(cls)));

    EINA_ITERATOR_FOREACH(iter, part)
    {
        const Eolian_Documentation *doc;
        const Eolian_Class         *klass;
        char                        pattern[24];

        /* testlog(pattern, "part%d", i + 1); */
        efl_assert_str_eq(pattern, eolian_part_name_get(part));

        /* testlog(pattern, "Part %d", i + 1); */
        efl_assert_fail_if(!(doc = eolian_part_documentation_get(part)));
        efl_assert_str_eq(pattern, eolian_documentation_summary_get(doc));

        efl_assert_fail_if(!(klass = eolian_part_class_get(part)));
        efl_assert_str_eq(part_classes[i], eolian_class_short_name_get(klass));

        efl_assert_fail_if(eolian_part_is_beta(part) && (i == 0));

        i++;
    }
    eina_iterator_free(iter);

    eolian_state_free(eos);
}

TEST(eolian_mixins_require)
{
    const Eolian_Unit  *unit;
    const Eolian_Class *cl;
    const Eolian_Class *base;

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "mixins_require.eo")));

    efl_assert_fail_if(!(cl = eolian_state_class_by_name_get(eos, "Mixins.Require")));
    efl_assert_fail_if(!(base = eolian_state_class_by_name_get(eos, "Base")));

    efl_assert_ptr_eq(eolian_class_parent_get(cl), NULL);

   //Check that implements is empty
    {
        Eolian_Class  *extc;
        Eina_Iterator *ext = eolian_class_extensions_get(cl);

        EINA_ITERATOR_FOREACH(ext, extc)
        {
            testlog("Iterator should be empty");
        }
        eina_iterator_free(ext);
    }
   //check that implements contains this one class
    {
        Eolian_Implement *impl;
        Eina_Iterator    *i   = eolian_class_implements_get(cl);
        Eina_Array       *tmp = eina_array_new(1);

        EINA_ITERATOR_FOREACH(i, impl)
        {
            if (eolian_implement_class_get(impl) != cl)
            {
                eina_array_push(tmp, eolian_implement_class_get(impl));
                efl_assert_ptr_eq(eolian_implement_class_get(impl), base);
            }
        }
        efl_assert_int_eq(eina_array_count(tmp), 1);
        eina_array_free(tmp);
        eina_iterator_free(i);
    }
   //check that the mixins has the right require
    {
        Eina_Iterator *iter = eolian_class_requires_get(cl);
        Eina_Array    *tmp  = eina_array_new(1);

        EINA_ITERATOR_FOREACH(iter, cl)
        {
            eina_array_push(tmp, cl);
        }
        efl_assert_int_eq(eina_array_count(tmp), 1);
        efl_assert_ptr_eq(eina_array_data_get(tmp, 0), base);
        eina_array_free(tmp);
        eina_iterator_free(iter);
    }
    eolian_state_free(eos);
}

TEST(eolian_class_requires_classes)
{
    const Eolian_Unit  *unit;
    const Eolian_Class *cl;

    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    efl_assert_fail_if(!(unit = eolian_state_file_parse(eos, "class_requires.eo")));

    efl_assert_fail_if(!(cl = eolian_state_class_by_name_get(eos, "Class.Requires")));

    eolian_state_free(eos);
}

TEST(eolian_class_unimpl)
{
    Eolian_State *eos = eolian_state_new();

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    setenv("EOLIAN_CLASS_UNIMPLEMENTED_WARN", "1", 1);
    const Eolian_Unit *unit = eolian_state_file_parse(eos, "unimpl.eo");
    unsetenv("EOLIAN_CLASS_UNIMPLEMENTED_WARN");
    efl_assert_fail_if(!unit);

    eolian_state_free(eos);
}

TEST(eolian_version)
{
    Eolian_State      *eos = eolian_state_new();
    const Eolian_Unit *u1, *u2;

    efl_assert_fail_if(!eolian_state_directory_add(eos, TESTS_SRC_DIR "/data"));

    efl_assert_fail_if(!(u1 = eolian_state_file_parse(eos, "class_simple.eo")));
    efl_assert_fail_if(!(u2 = eolian_state_file_parse(eos, "base.eo")));
    efl_assert_fail_if(eolian_unit_version_get(u1) != 1);
    efl_assert_fail_if(eolian_unit_version_get(u2) != 1);

    eolian_state_free(eos);
}
