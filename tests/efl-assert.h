/*
 * Copyright 2022 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "Efl_Shared.h"

/**************************/

#define ASSERT_STR_MATCH(_as, _bs) do { \
	const char *as = _as; \
	const char *bs = _bs; \
	assert(!!as == !!bs); \
	assert(!as || strcmp(as, bs) == 0); \
} while (0)

#define ASSERT_STR_ARRAY_MATCH(_name, _aa, _ba) do { \
	char * const *aa = _aa; \
	char * const *ba = _ba; \
	testlog("\tcomparing " _name ":\n"); \
	for (int _i = 0; aa[_i] || ba[_i]; _i++) { \
		testlog("\t\t[%d] '%s' == '%s'?\n", _i, aa[_i], ba[_i]); \
		ASSERT_STR_MATCH(aa[_i], ba[_i]); \
	} \
	testlog("\tsuccessfully compared " _name "\n"); \
} while (0)

/**************************/

/**
 * NaN comes out as is
 *This function is not intended for hiding NaN.
 */
EFL_UNUSED static float
ensure_unit_range(float v)
{
	const float tol = 1e-5f;
	const float lim_lo = -tol;
	const float lim_hi = 1.0f + tol;

	assert(v >= lim_lo);
	if (v < 0.0f)
		return 0.0f;
	assert(v <= lim_hi);
	if (v > 1.0f)
		return 1.0f;
	return v;
}

/**************************/

__attribute__((noreturn, format(printf, 1, 2)))
static inline void
efl_assert_fail_(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	abort();
}

#ifndef custom_assert_fail_
#define custom_assert_fail_ efl_assert_fail_
#endif

#define efl_assert_(a, b, val_type, val_fmt, cmp)				\
({																\
	val_type a_ = (a);											\
	val_type b_ = (b);											\
	bool cond = a_ cmp b_;										\
	if (!cond)													\
		custom_assert_fail_("%s:%u: Assertion %s %s %s (" val_fmt " %s " val_fmt ") failed!\n",	\
				    __FILE__, __LINE__, #a, #cmp, #b, a_, #cmp, b_);							\
	cond;																						\
})

#define efl_assert_fn_(fn, a, b, val_type, val_fmt, cmp)		\
({																\
	val_type a_ = (a);											\
	val_type b_ = (b);											\
	bool cond = fn(a_, b_) cmp 0;								\
	if (!cond)													\
		custom_assert_fail_("%s:%u: Assertion %s %s %s (" val_fmt " %s " val_fmt ") failed!\n",	\
				    __FILE__, __LINE__, #a, #cmp, #b, a_, #cmp, b_);							\
	cond;																						\
})

#define efl_assert_true(a) efl_assert_(a, true, bool, "%d", ==)
#define efl_assert_false(a) efl_assert_(a, true, bool, "%d", !=)
#define efl_assert_fail_if(a) efl_assert_false(a)

#define efl_assert_ptr_eq(a, b)   efl_assert_(a, b, const void *, "%p", ==)
#define efl_assert_ptr_ne(a, b)   efl_assert_(a, b, const void *, "%p", !=)
#define efl_assert_ptr_is_null(a) efl_assert_(a, NULL, const void *, "%p", ==)

#define efl_assert_double_eq(a, b) efl_assert_(a, b, double, "%.10g", ==)

#define efl_assert_int_eq(a, b) efl_assert_(a, b, int, "%d", ==)
#define efl_assert_int_ne(a, b) efl_assert_(a, b, int, "%d", !=)
#define efl_assert_int_ge(a, b) efl_assert_(a, b, int, "%d", >=)
#define efl_assert_int_le(a, b) efl_assert_(a, b, int, "%d", <=)

#define efl_assert_str_eq(a, b) efl_assert_fn_(strcmp, a, b, const char *, "%s", ==)
#define efl_assert_str_ne(a, b) efl_assert_fn_(strcmp, a, b, const char *, "%s", !=)