#ifndef _EOLIAN_CLASS_SIMPLE_EO_H_
#define _EOLIAN_CLASS_SIMPLE_EO_H_

#ifndef _CLASS_SIMPLE_EO_CLASS_TYPE
#  define _CLASS_SIMPLE_EO_CLASS_TYPE

typedef Eo Class_Simple;

#endif

#ifndef _CLASS_SIMPLE_EO_TYPES
#  define _CLASS_SIMPLE_EO_TYPES

#  ifndef FOO
/** doc for constant
 *
 * @since 1.66
 *
 * @ingroup Foo
 */
#    define FOO 5
#  endif

#endif
/** Class Desc Simple
 *
 * @since 1.66
 *
 * @ingroup Class_Simple
 */
#define CLASS_SIMPLE_CLASS class_simple_class_get()

EAPI EAPI_WEAK const Efl_Class *class_simple_class_get(void) EINA_CONST;

#ifdef EFL_BETA_API_SUPPORT
/**
 * @brief Common desc for a
 *
 * comment a.set
 *
 * @param[in] obj The object.
 * @param[in] value Value description
 *
 * @return comment for property set return
 *
 * @since 1.66
 *
 * @ingroup Class_Simple
 */
EAPI EAPI_WEAK Efl_Bool efl_canvas_object_simple_a_set(Eo *obj, int value);
#endif /* EFL_BETA_API_SUPPORT */

#ifdef EFL_BETA_API_SUPPORT
/**
 * @brief Common desc for a
 *
 * @param[in] obj The object.
 *
 * @return Value description
 *
 * @since 1.66
 *
 * @ingroup Class_Simple
 */
EAPI EAPI_WEAK int efl_canvas_object_simple_a_get(const Eo *obj);
#endif /* EFL_BETA_API_SUPPORT */

#ifdef EFL_BETA_API_SUPPORT
/**
 * @brief comment foo
 *
 * @param[in] obj The object.
 * @param[in] a a
 * @param[in,out] b
 * @param[out] c
 * @param[in] d
 *
 * @return comment for method return
 *
 * @since 1.66
 *
 * @ingroup Class_Simple
 */
EAPI EAPI_WEAK char *
efl_canvas_object_simple_foo(Eo *obj, int a, char *b, double *c, int *d);
#endif /* EFL_BETA_API_SUPPORT */

#endif
