#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>

#include "eolian_database.h"

EOLIAN_API Eolian_Value
eolian_expression_eval(const Eolian_Expression *expr, Eolian_Expression_Mask m)
{
  Eolian_Value err;
  err.type = EOLIAN_EXPR_UNKNOWN;
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, err);
  return database_expr_eval(NULL, (Eolian_Expression *)expr, m, NULL, NULL);
}

EOLIAN_API Efl_Bool
eolian_expression_eval_fill(const Eolian_Expression *expr,
                            Eolian_Expression_Mask   m,
                            Eolian_Value            *val)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, EFL_FALSE);
  Eolian_Value ret =
    database_expr_eval(NULL, (Eolian_Expression *)expr, m, NULL, NULL);
  if (ret.type == EOLIAN_EXPR_UNKNOWN) return EFL_FALSE;
  *val = ret;
  return EFL_TRUE;
}

static void
_append_char_escaped(Efl_Strbuf *buf, char c)
{
  switch (c)
  {
    case '\'':
      eina_strbuf_append(buf, "\\\'");
      break;
    case '\"':
      eina_strbuf_append(buf, "\\\"");
      break;
    case '\?':
      eina_strbuf_append(buf, "\\\?");
      break;
    case '\\':
      eina_strbuf_append(buf, "\\\\");
      break;
    case '\a':
      eina_strbuf_append(buf, "\\a");
      break;
    case '\b':
      eina_strbuf_append(buf, "\\b");
      break;
    case '\f':
      eina_strbuf_append(buf, "\\f");
      break;
    case '\n':
      eina_strbuf_append(buf, "\\n");
      break;
    case '\r':
      eina_strbuf_append(buf, "\\r");
      break;
    case '\t':
      eina_strbuf_append(buf, "\\t");
      break;
    case '\v':
      eina_strbuf_append(buf, "\\v");
      break;
    default:
      if ((c < 32) || (c > 126))
        eina_strbuf_append_printf(buf, "\\x%X", (unsigned char)c);
      else eina_strbuf_append_char(buf, c);
      break;
  }
}

static void
_number_to_str(const Eolian_Value *v, Efl_Strbuf *buf)
{
  switch (v->type)
  {
    case EOLIAN_EXPR_INT:
      eina_strbuf_append_printf(buf, "%d", v->value.i);
      break;
    case EOLIAN_EXPR_UINT:
      eina_strbuf_append_printf(buf, "%uU", v->value.u);
      break;
    case EOLIAN_EXPR_LONG:
      eina_strbuf_append_printf(buf, "%ldL", v->value.l);
      break;
    case EOLIAN_EXPR_ULONG:
      eina_strbuf_append_printf(buf, "%luUL", v->value.ul);
      break;
    case EOLIAN_EXPR_LLONG:
      eina_strbuf_append_printf(buf, "%ldLL", (long)v->value.ll);
      break;
    case EOLIAN_EXPR_ULLONG:
      eina_strbuf_append_printf(buf, "%luULL", (unsigned long)v->value.ull);
      break;
    case EOLIAN_EXPR_FLOAT:
      eina_strbuf_append_printf(buf, "%ff", v->value.f);
      break;
    case EOLIAN_EXPR_DOUBLE:
      eina_strbuf_append_printf(buf, "%f", v->value.d);
      break;
    default:
      break;
  }
}

EOLIAN_API Eina_Stringshare *
eolian_expression_value_to_literal(const Eolian_Value *val)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(val, NULL);
  switch (val->type)
  {
    case EOLIAN_EXPR_BOOL:
      return eina_stringshare_add(val->value.b ? "EFL_TRUE" : "EFL_FALSE");
    case EOLIAN_EXPR_NULL:
      return eina_stringshare_add("NULL");
    case EOLIAN_EXPR_CHAR:
      {
        char        c   = val->value.c;
        Efl_Strbuf *buf = eina_strbuf_new();
        const char *ret;
        eina_strbuf_append_char(buf, '\'');
        _append_char_escaped(buf, c);
        eina_strbuf_append_char(buf, '\'');
        ret = eina_stringshare_add(eina_strbuf_string_get(buf));
        eina_strbuf_free(buf);
        return ret;
      }
    case EOLIAN_EXPR_STRING:
      {
        const char *ret;
        char       *c   = (char *)val->value.s;
        Efl_Strbuf *buf = eina_strbuf_new();
        eina_strbuf_append_char(buf, '\"');
        while (*c)
          _append_char_escaped(buf, *(c++));
        eina_strbuf_append_char(buf, '\"');
        ret = eina_stringshare_add(eina_strbuf_string_get(buf));
        eina_strbuf_free(buf);
        return ret;
      }
    case EOLIAN_EXPR_INT:
    case EOLIAN_EXPR_UINT:
    case EOLIAN_EXPR_LONG:
    case EOLIAN_EXPR_ULONG:
    case EOLIAN_EXPR_LLONG:
    case EOLIAN_EXPR_ULLONG:
    case EOLIAN_EXPR_FLOAT:
    case EOLIAN_EXPR_DOUBLE:
      {
        const char *ret;
        Efl_Strbuf *buf = eina_strbuf_new();
        _number_to_str(val, buf);
        ret = eina_stringshare_add(eina_strbuf_string_get(buf));
        eina_strbuf_free(buf);
        return ret;
      }
    default:
      return NULL;
  }
}

static const char *_binops[] = { "+",  "-", "*", "/",  "%",  "==",
                                 "!=", ">", "<", ">=", "<=", "&&",
                                 "||", "&", "|", "^",  "<<", ">>" };

static const char *_unops[] = { "-", "+", "!", "~" };

static Efl_Bool
_expr_serialize(const Eolian_Expression *expr, Efl_Strbuf *buf, Efl_Bool outer)
{
  switch (expr->type)
  {
    case EOLIAN_EXPR_UNKNOWN:
      return EFL_FALSE;
    case EOLIAN_EXPR_INT:
    case EOLIAN_EXPR_UINT:
    case EOLIAN_EXPR_LONG:
    case EOLIAN_EXPR_ULONG:
    case EOLIAN_EXPR_LLONG:
    case EOLIAN_EXPR_ULLONG:
    case EOLIAN_EXPR_FLOAT:
    case EOLIAN_EXPR_DOUBLE:
    case EOLIAN_EXPR_STRING:
    case EOLIAN_EXPR_CHAR:
      {
        Eolian_Value v;
        v.type        = expr->type;
        v.value       = expr->value;
        const char *x = eolian_expression_value_to_literal(&v);
        if (!x) return EFL_FALSE;
        eina_strbuf_append(buf, x);
        eina_stringshare_del(x);
        break;
      }
    case EOLIAN_EXPR_NULL:
      eina_strbuf_append(buf, "null");
      break;
    case EOLIAN_EXPR_BOOL:
      eina_strbuf_append(buf, expr->value.b ? "true" : "false");
      break;
    case EOLIAN_EXPR_NAME:
      {
        eina_strbuf_append(buf, expr->value.s);
        break;
      }
    case EOLIAN_EXPR_UNARY:
      eina_strbuf_append(buf, _unops[expr->unop]);
      _expr_serialize(expr->expr, buf, EFL_FALSE);
      break;
    case EOLIAN_EXPR_BINARY:
      if (!outer) eina_strbuf_append_char(buf, '(');
      _expr_serialize(expr->lhs, buf, EFL_FALSE);
      eina_strbuf_append_printf(buf, " %s ", _binops[expr->binop]);
      _expr_serialize(expr->rhs, buf, EFL_FALSE);
      if (!outer) eina_strbuf_append_char(buf, ')');
      break;
    default:
      return EFL_FALSE;
  }
  return EFL_TRUE;
}

EOLIAN_API Eina_Stringshare *
eolian_expression_serialize(const Eolian_Expression *expr)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, NULL);
  Efl_Strbuf *buf = eina_strbuf_new();
  if (!_expr_serialize(expr, buf, EFL_TRUE))
  {
    eina_strbuf_free(buf);
    return NULL;
  }
  const char *ret = eina_stringshare_add(eina_strbuf_string_get(buf));
  eina_strbuf_free(buf);
  return ret;
}

EOLIAN_API Eolian_Expression_Type
eolian_expression_type_get(const Eolian_Expression *expr)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, EOLIAN_EXPR_UNKNOWN);
  return expr->type;
}

EOLIAN_API Eolian_Binary_Operator
eolian_expression_binary_operator_get(const Eolian_Expression *expr)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, EOLIAN_BINOP_INVALID);
  EINA_SAFETY_ON_FALSE_RETURN_VAL(expr->type == EOLIAN_EXPR_BINARY,
                                  EOLIAN_BINOP_INVALID);
  return expr->binop;
}

EOLIAN_API const Eolian_Expression *
eolian_expression_binary_lhs_get(const Eolian_Expression *expr)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, NULL);
  EINA_SAFETY_ON_FALSE_RETURN_VAL(expr->type == EOLIAN_EXPR_BINARY, NULL);
  return expr->lhs;
}

EOLIAN_API const Eolian_Expression *
eolian_expression_binary_rhs_get(const Eolian_Expression *expr)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, NULL);
  EINA_SAFETY_ON_FALSE_RETURN_VAL(expr->type == EOLIAN_EXPR_BINARY, NULL);
  return expr->rhs;
}

EOLIAN_API Eolian_Unary_Operator
eolian_expression_unary_operator_get(const Eolian_Expression *expr)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, EOLIAN_UNOP_INVALID);
  EINA_SAFETY_ON_FALSE_RETURN_VAL(expr->type == EOLIAN_EXPR_UNARY,
                                  EOLIAN_UNOP_INVALID);
  return expr->unop;
}

EOLIAN_API const Eolian_Expression *
eolian_expression_unary_expression_get(const Eolian_Expression *expr)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, NULL);
  EINA_SAFETY_ON_FALSE_RETURN_VAL(expr->type == EOLIAN_EXPR_UNARY, NULL);
  return expr->expr;
}

EOLIAN_API Eolian_Value
eolian_expression_value_get(const Eolian_Expression *expr)
{
  Eolian_Value v;
  v.type = EOLIAN_EXPR_UNKNOWN;
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, v);
  EINA_SAFETY_ON_FALSE_RETURN_VAL(expr->type != EOLIAN_EXPR_UNKNOWN &&
                                    expr->type != EOLIAN_EXPR_BINARY &&
                                    expr->type != EOLIAN_EXPR_UNARY,
                                  v);
  v.type  = expr->type;
  v.value = expr->value;
  return v;
}

EOLIAN_API Efl_Bool
eolian_expression_value_get_fill(const Eolian_Expression *expr,
                                 Eolian_Value            *val)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(expr, EFL_FALSE);
  EINA_SAFETY_ON_FALSE_RETURN_VAL(expr->type != EOLIAN_EXPR_UNKNOWN &&
                                    expr->type != EOLIAN_EXPR_BINARY &&
                                    expr->type != EOLIAN_EXPR_UNARY,
                                  EFL_FALSE);
  val->type  = expr->type;
  val->value = expr->value;
  return EFL_TRUE;
}
