/*
 * Copyright (C) 2017 Cedric Bail
 */

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(eina_util_fpx)
{
    double a = 42.42, b = 42.41;
    float  c = 7.654, d = 8.765;

    efl_assert_int_eq(eina_dbl_exact(a, b), EFL_FALSE);
    efl_assert_int_eq(eina_dbl_exact(a, a), EFL_TRUE);
    efl_assert_int_eq(eina_flt_exact(c, d), EFL_FALSE);
    efl_assert_int_eq(eina_flt_exact(c, c), EFL_TRUE);
    efl_assert_int_eq(eina_dbl_exact(a, c), EFL_FALSE);
}

TEST(eina_util_fpeq)
{
    double a, b, c;
    double d, e, f;

    a = 6.0 * 7.0 + 7.0;
    b = 7.0 * 7.0;
    c = a - 0.1;

    d = 6.0 * 7.0 + 7.0;
    e = 7.0 * 7.0;
    f = a - 0.1;

    efl_assert_int_ne(EINA_DBL_EQ(a, b), 0);
    efl_assert_int_eq(EINA_DBL_EQ(a, c), 0);
    efl_assert_int_ne(EINA_DBL_EQ(d, e), 0);
    efl_assert_int_eq(EINA_DBL_EQ(d, f), 0);
}

TEST(eina_util_fpz)
{
    efl_assert_int_eq(EINA_FLT_NONZERO(7.0 - 7.0), 0);
    efl_assert_int_ne(EINA_FLT_NONZERO(7.1 - 7.0), 0);
    efl_assert_int_eq(EINA_FLT_NONZERO(0.0), 0);
    efl_assert_int_eq(EINA_DBL_NONZERO(7.0 - 7.0), 0);
    efl_assert_int_ne(EINA_DBL_NONZERO(7.1 - 7.0), 0);
    efl_assert_int_eq(EINA_DBL_NONZERO(0.0), 0);
}
