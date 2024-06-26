#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"
#include "eo_lexer.h"

EOLIAN_API Eolian_Type_Type
eolian_type_type_get(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, EOLIAN_TYPE_UNKNOWN_TYPE);
  return tp->type;
}

EOLIAN_API Eolian_Type_Builtin_Type
eolian_type_builtin_type_get(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, EOLIAN_TYPE_BUILTIN_INVALID);
  return tp->btype;
}

EOLIAN_API Eolian_Typedecl_Type
eolian_typedecl_type_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, EOLIAN_TYPEDECL_UNKNOWN);
  return tp->type;
}

EOLIAN_API Eina_Iterator *
eolian_typedecl_struct_fields_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  if (tp->type != EOLIAN_TYPEDECL_STRUCT) return NULL;
  return eina_list_iterator_new(tp->field_list);
}

EOLIAN_API const Eolian_Struct_Type_Field *
eolian_typedecl_struct_field_get(const Eolian_Typedecl *tp, const char *field)
{
  Eolian_Struct_Type_Field *sf = NULL;
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(field, NULL);
  if (tp->type != EOLIAN_TYPEDECL_STRUCT) return NULL;
  sf = eina_hash_find(tp->fields, field);
  if (!sf) return NULL;
  return sf;
}

EOLIAN_API const Eolian_Documentation *
eolian_typedecl_struct_field_documentation_get(
  const Eolian_Struct_Type_Field *fl)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, NULL);
  return fl->doc;
}

EOLIAN_API const Eolian_Type *
eolian_typedecl_struct_field_type_get(const Eolian_Struct_Type_Field *fl)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, NULL);
  return fl->type;
}

EOLIAN_API Efl_Bool
eolian_typedecl_struct_field_is_by_ref(const Eolian_Struct_Type_Field *fl)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, EFL_FALSE);
  return fl->by_ref;
}

EOLIAN_API Efl_Bool
eolian_typedecl_struct_field_is_move(const Eolian_Struct_Type_Field *fl)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, EFL_FALSE);
  return fl->move;
}

EOLIAN_API Eina_Stringshare *
eolian_typedecl_struct_field_c_type_get(const Eolian_Struct_Type_Field *fl)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, NULL);
  Efl_Strbuf *buf = eina_strbuf_new();
  database_type_to_str(fl->type, buf, NULL, EOLIAN_C_TYPE_DEFAULT, fl->by_ref);
  Eina_Stringshare *ret = eina_stringshare_add(eina_strbuf_string_get(buf));
  eina_strbuf_free(buf);
  return ret;
}

EOLIAN_API Eina_Iterator *
eolian_typedecl_enum_fields_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  if (tp->type != EOLIAN_TYPEDECL_ENUM) return NULL;
  return eina_list_iterator_new(tp->field_list);
}

EOLIAN_API const Eolian_Enum_Type_Field *
eolian_typedecl_enum_field_get(const Eolian_Typedecl *tp, const char *field)
{
  Eolian_Enum_Type_Field *ef = NULL;
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(field, NULL);
  if (tp->type != EOLIAN_TYPEDECL_ENUM) return NULL;
  ef = eina_hash_find(tp->fields, field);
  if (!ef) return NULL;
  return ef;
}

EOLIAN_API Eina_Stringshare *
eolian_typedecl_enum_field_c_constant_get(const Eolian_Enum_Type_Field *fl)
{
  Eina_Stringshare *ret;
  Efl_Strbuf       *buf;
  char             *bufp, *p;
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, NULL);
  buf = eina_strbuf_new();
  if (fl->base_enum->legacy) eina_strbuf_append(buf, fl->base_enum->legacy);
  else eina_strbuf_append(buf, fl->base_enum->base.c_name);
  eina_strbuf_append_char(buf, '_');
  eina_strbuf_append(buf, fl->base.name);
  bufp = eina_strbuf_string_steal(buf);
  eina_strbuf_free(buf);
  eina_str_toupper(&bufp);
  while ((p = strchr(bufp, '.')))
    *p = '_';
  ret = eina_stringshare_add(bufp);
  free(bufp);
  return ret;
}

EOLIAN_API const Eolian_Documentation *
eolian_typedecl_enum_field_documentation_get(const Eolian_Enum_Type_Field *fl)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, NULL);
  return fl->doc;
}

EOLIAN_API const Eolian_Expression *
eolian_typedecl_enum_field_value_get(const Eolian_Enum_Type_Field *fl,
                                     Efl_Bool                      force)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(fl, NULL);
  if (!force && !fl->is_public_value) return NULL;
  return fl->value;
}

EOLIAN_API const char *
eolian_typedecl_enum_legacy_prefix_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  if (tp->type != EOLIAN_TYPEDECL_ENUM) return NULL;
  return tp->legacy;
}

EOLIAN_API const Eolian_Documentation *
eolian_typedecl_documentation_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  return tp->doc;
}

EOLIAN_API const Eolian_Type *
eolian_type_base_type_get(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  return tp->base_type;
}

EOLIAN_API const Eolian_Type *
eolian_type_next_type_get(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  return tp->next_type;
}

EOLIAN_API const Eolian_Typedecl *
eolian_type_typedecl_get(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  if (eolian_type_type_get(tp) != EOLIAN_TYPE_REGULAR) return NULL;
  return tp->tdecl;
}

EOLIAN_API const Eolian_Type *
eolian_typedecl_base_type_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  return tp->base_type;
}

EOLIAN_API const Eolian_Type *
eolian_type_aliased_base_get(const Eolian_Type *tp)
{
  if (!tp || tp->type != EOLIAN_TYPE_REGULAR || tp->is_ptr) return tp;
  const Eolian_Typedecl *btp = eolian_type_typedecl_get(tp);
  if (btp && (btp->type == EOLIAN_TYPEDECL_ALIAS))
    return eolian_typedecl_aliased_base_get(btp);
  return tp;
}

EOLIAN_API const Eolian_Type *
eolian_typedecl_aliased_base_get(const Eolian_Typedecl *tp)
{
  if (!tp || tp->type != EOLIAN_TYPEDECL_ALIAS) return NULL;
  return eolian_type_aliased_base_get(tp->base_type);
}

EOLIAN_API const Eolian_Class *
eolian_type_class_get(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  if (eolian_type_type_get(tp) != EOLIAN_TYPE_CLASS) return NULL;
  return tp->klass;
}

EOLIAN_API const Eolian_Error *
eolian_type_error_get(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  if (eolian_type_type_get(tp) != EOLIAN_TYPE_ERROR) return NULL;
  return tp->error;
}

EOLIAN_API Efl_Bool
eolian_type_is_move(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, EFL_FALSE);
  return tp->move;
}

EOLIAN_API Efl_Bool
eolian_type_is_const(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, EFL_FALSE);
  return tp->is_const;
}

EOLIAN_API Efl_Bool
eolian_type_is_ptr(const Eolian_Type *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, EFL_FALSE);
  return tp->is_ptr;
}

EOLIAN_API Efl_Bool
eolian_typedecl_is_extern(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, EFL_FALSE);
  return tp->is_extern;
}

EOLIAN_API Eina_Stringshare *
eolian_type_c_type_get(const Eolian_Type *tp)
{
  Eina_Stringshare *ret;
  Efl_Strbuf       *buf;
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  buf = eina_strbuf_new();
  database_type_to_str(tp, buf, NULL, EOLIAN_C_TYPE_DEFAULT, EFL_FALSE);
  ret = eina_stringshare_add(eina_strbuf_string_get(buf));
  eina_strbuf_free(buf);
  return ret;
}

EOLIAN_API Eina_Stringshare *
eolian_typedecl_c_type_get(const Eolian_Typedecl *tp)
{
  Eina_Stringshare *ret;
  Efl_Strbuf       *buf;
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  buf = eina_strbuf_new();
  database_typedecl_to_str(tp, buf);
  ret = eina_stringshare_add(eina_strbuf_string_get(buf));
  eina_strbuf_free(buf);
  return ret;
}

EOLIAN_API const char *
eolian_typedecl_free_func_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  return tp->freefunc;
}

EOLIAN_API const Eolian_Function *
eolian_typedecl_function_pointer_get(const Eolian_Typedecl *tp)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(tp, NULL);
  if (eolian_typedecl_type_get(tp) != EOLIAN_TYPEDECL_FUNCTION_POINTER)
    return NULL;
  return tp->function_pointer;
}
