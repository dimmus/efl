/*
 * Copyright (C) 2012 ProFUSION embedded systems
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <inttypes.h>
#include <float.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

#define FP_ERR         (1)
#define CHECK_FP(a, b) ((a - b) <= FP_ERR)

TEST(value_test_simple)
{
    Eina_Value    *value;
    char           c;
    short          s;
    int            i;
    long           l;
    int64_t        i64;
    unsigned char  uc;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;

    value = eina_value_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(value != NULL);
    efl_assert_true(eina_value_set(value, 'x'));
    efl_assert_true(eina_value_get(value, &c));
    efl_assert_true(c == 'x');
    efl_assert_true(eina_value_char_get(value, &c));
    efl_assert_fail_if(eina_value_double_get(value, &d));
    efl_assert_true(eina_value_int64_convert(value, &i64));
    efl_assert_true(i64 == 'x');
    efl_assert_true(c == 'x');
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_set(value, 300));
    efl_assert_true(eina_value_get(value, &s));
    efl_assert_true(s == 300);
    efl_assert_true(eina_value_short_get(value, &s));
    efl_assert_fail_if(eina_value_char_get(value, &c));
    efl_assert_true(eina_value_int_convert(value, &i));
    efl_assert_true(i == 300);
    efl_assert_true(s == 300);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_set(value, -12345));
    efl_assert_true(eina_value_get(value, &i));
    efl_assert_true(i == -12345);
    efl_assert_true(eina_value_int_get(value, &i));
    efl_assert_fail_if(eina_value_short_get(value, &s));
    efl_assert_true(eina_value_long_convert(value, &l));
    efl_assert_true(l == -12345);
    efl_assert_true(i == -12345);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_set(value, 0xb33f));
    efl_assert_true(eina_value_get(value, &l));
    efl_assert_true(l == 0xb33f);
    efl_assert_true(eina_value_long_get(value, &l));
    efl_assert_fail_if(eina_value_int_get(value, &i));
    efl_assert_true(eina_value_int_convert(value, &i));
    efl_assert_true(i == 0xb33f);
    efl_assert_true(l == 0xb33f);
    eina_value_flush(value);

   /*
    * On Windows, long is always a 4 bytes type, so this check
    * will never work on Windows.
    */
#ifndef _WIN32
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_set(value, 0x0011223344556677));
    efl_assert_true(eina_value_get(value, &i64));
    efl_assert_true(i64 == 0x0011223344556677);
    efl_assert_true(eina_value_int64_get(value, &i64));
    efl_assert_fail_if(eina_value_long_get(value, &l));
    efl_assert_true(eina_value_long_convert(value, &l));
    efl_assert_true(l == (long)0x0011223344556677);
    efl_assert_true(i64 == 0x0011223344556677);
    eina_value_flush(value);
#endif

   /* unsigned: */

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_set(value, 200));
    efl_assert_true(eina_value_get(value, &uc));
    efl_assert_true(uc == 200);
    efl_assert_true(eina_value_uchar_get(value, &uc));
    efl_assert_fail_if(eina_value_int64_get(value, &i64));
    efl_assert_true(eina_value_uint64_convert(value, &u64));
    efl_assert_true(u64 == 200);
    efl_assert_true(uc == 200);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_set(value, 65535));
    efl_assert_true(eina_value_get(value, &us));
    efl_assert_true(us == 65535);
    efl_assert_true(eina_value_ushort_get(value, &us));
    efl_assert_fail_if(eina_value_uchar_get(value, &uc));
    efl_assert_true(eina_value_uint_convert(value, &ui));
    efl_assert_true(ui == 65535);
    efl_assert_true(us == 65535);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_set(value, 4000000000U));
    efl_assert_true(eina_value_get(value, &ui));
    efl_assert_true(ui == 4000000000U);
    efl_assert_true(eina_value_uint_get(value, &ui));
    efl_assert_fail_if(eina_value_ushort_get(value, &us));
    efl_assert_true(eina_value_ulong_convert(value, &ul));
    efl_assert_true(ul == 4000000000U);
    efl_assert_true(ui == 4000000000U);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_set(value, 3000000001UL));
    efl_assert_true(eina_value_get(value, &ul));
    efl_assert_true(ul == 3000000001UL);
    efl_assert_true(eina_value_ulong_get(value, &ul));
    efl_assert_fail_if(eina_value_uint_get(value, &ui));
    efl_assert_true(eina_value_uint64_convert(value, &u64));
    efl_assert_true(u64 == 3000000001UL);
    efl_assert_true(ul == 3000000001UL);
    eina_value_flush(value);

   /*
    * On Windows, long is always a 4 bytes type, so this check
    * will never work on Windows.
    */
#ifndef _WIN32
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_set(value, 0x1122334455667788));
    efl_assert_true(eina_value_get(value, &u64));
    efl_assert_true(u64 == 0x1122334455667788);
    efl_assert_true(eina_value_uint64_get(value, &u64));
    efl_assert_fail_if(eina_value_ulong_get(value, &ul));
    efl_assert_true(eina_value_ulong_convert(value, &ul));
    efl_assert_true(ul == (unsigned long)0x1122334455667788);
    efl_assert_true(u64 == 0x1122334455667788);
    eina_value_flush(value);
#endif

   /* floating point */
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_set(value, 0.1234));
    efl_assert_true(eina_value_get(value, &f));
    efl_assert_true(CHECK_FP(0.1234, f));
    efl_assert_true(eina_value_float_get(value, &f));
    efl_assert_fail_if(eina_value_uint64_get(value, &u64));
    efl_assert_true(eina_value_double_convert(value, &d));
    efl_assert_true(CHECK_FP(0.1234, d));
    efl_assert_true(CHECK_FP(0.1234, f));
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_set(value, 34567.8));
    efl_assert_true(eina_value_get(value, &d));
    efl_assert_true(CHECK_FP(34567.8, d));
    efl_assert_true(eina_value_double_get(value, &d));
    efl_assert_fail_if(eina_value_float_get(value, &f));
    efl_assert_true(eina_value_float_convert(value, &f));
    efl_assert_true(CHECK_FP(34567.8, d));
    efl_assert_true(CHECK_FP(34567.8, f));
    eina_value_flush(value);

    eina_value_free(value);
}

TEST(value_test_compare)
{
    Eina_Value *a, *b;

    a = eina_value_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(a != NULL);
    b = eina_value_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(b != NULL);

    efl_assert_true(eina_value_set(a, 123));
    efl_assert_true(eina_value_set(b, 123));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, -10));
    efl_assert_true(eina_value_set(b, 123));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 123));
    efl_assert_true(eina_value_set(b, 10));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_set(a, 1230));
    efl_assert_true(eina_value_set(b, 1230));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, -100));
    efl_assert_true(eina_value_set(b, 1230));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 1230));
    efl_assert_true(eina_value_set(b, -100));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_set(a, 300000));
    efl_assert_true(eina_value_set(b, 300000));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, -100));
    efl_assert_true(eina_value_set(b, 300000));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 300000));
    efl_assert_true(eina_value_set(b, -100));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_set(a, 300000L));
    efl_assert_true(eina_value_set(b, 300000L));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, -100L));
    efl_assert_true(eina_value_set(b, 300000L));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 300000L));
    efl_assert_true(eina_value_set(b, -100L));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_set(a, (int64_t)800000));
    efl_assert_true(eina_value_set(b, (int64_t)800000));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, (int64_t)-100));
    efl_assert_true(eina_value_set(b, (int64_t)8000000));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, (int64_t)8000000));
    efl_assert_true(eina_value_set(b, (int64_t)-100));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_set(a, 123));
    efl_assert_true(eina_value_set(b, 123));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, 10));
    efl_assert_true(eina_value_set(b, 123));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 123));
    efl_assert_true(eina_value_set(b, 10));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_set(a, 1230));
    efl_assert_true(eina_value_set(b, 1230));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, 100));
    efl_assert_true(eina_value_set(b, 1230));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 1230));
    efl_assert_true(eina_value_set(b, 100));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_set(a, 300000));
    efl_assert_true(eina_value_set(b, 300000));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, 100));
    efl_assert_true(eina_value_set(b, 300000));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 300000));
    efl_assert_true(eina_value_set(b, 100));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_set(a, 300000UL));
    efl_assert_true(eina_value_set(b, 300000UL));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, 100UL));
    efl_assert_true(eina_value_set(b, 300000UL));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, 300000UL));
    efl_assert_true(eina_value_set(b, 100UL));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_set(a, (uint64_t)8000000));
    efl_assert_true(eina_value_set(b, (uint64_t)8000000));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, (uint64_t)100));
    efl_assert_true(eina_value_set(b, (uint64_t)8000000));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, (uint64_t)8000000));
    efl_assert_true(eina_value_set(b, (uint64_t)100));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_setup(a, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_setup(b, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_set(a, "aaa"));
    efl_assert_true(eina_value_set(b, "aaa"));
    efl_assert_true(eina_value_compare(a, b) == 0);
    efl_assert_true(eina_value_set(a, "abc"));
    efl_assert_true(eina_value_set(b, "acd"));
    efl_assert_true(eina_value_compare(a, b) < 0);
    efl_assert_true(eina_value_set(a, "acd"));
    efl_assert_true(eina_value_set(b, "abc"));
    efl_assert_true(eina_value_compare(a, b) > 0);
    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_array_setup(a, EINA_VALUE_TYPE_CHAR, 0));
    efl_assert_true(eina_value_array_setup(b, EINA_VALUE_TYPE_CHAR, 0));
    efl_assert_true(eina_value_compare(a, b) == 0);

    efl_assert_true(eina_value_array_append(a, 1));
    efl_assert_true(eina_value_array_append(a, 2));
    efl_assert_true(eina_value_array_append(a, 3));

    efl_assert_true(eina_value_array_append(b, 1));
    efl_assert_true(eina_value_array_append(b, 2));
    efl_assert_true(eina_value_array_append(b, 3));

    efl_assert_true(eina_value_compare(a, b) == 0);

    efl_assert_true(eina_value_array_set(a, 0, 0));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_array_set(a, 0, 10));
    efl_assert_true(eina_value_compare(a, b) > 0);

    efl_assert_true(eina_value_array_set(a, 0, 1));

    efl_assert_true(eina_value_array_set(b, 0, 0));
    efl_assert_true(eina_value_compare(a, b) > 0);

    efl_assert_true(eina_value_array_set(b, 0, 10));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_array_set(b, 0, 1));
    efl_assert_true(eina_value_compare(a, b) == 0);

   /* bigger arrays are greater */
    efl_assert_true(eina_value_array_append(b, 0));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_array_append(a, 0));
    efl_assert_true(eina_value_array_append(a, 0));
    efl_assert_true(eina_value_compare(a, b) > 0);

   /* bigger arrays are greater, unless an element says otherwise */
    efl_assert_true(eina_value_array_set(b, 0, 10));
    efl_assert_true(eina_value_compare(a, b) < 0);

    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_list_setup(a, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_list_setup(b, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_compare(a, b) == 0);

    efl_assert_true(eina_value_list_append(a, 1));
    efl_assert_true(eina_value_list_append(a, 2));
    efl_assert_true(eina_value_list_append(a, 3));

    efl_assert_true(eina_value_list_append(b, 1));
    efl_assert_true(eina_value_list_append(b, 2));
    efl_assert_true(eina_value_list_append(b, 3));

    efl_assert_true(eina_value_compare(a, b) == 0);

    efl_assert_true(eina_value_list_set(a, 0, 0));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_list_set(a, 0, 10));
    efl_assert_true(eina_value_compare(a, b) > 0);

    efl_assert_true(eina_value_list_set(a, 0, 1));

    efl_assert_true(eina_value_list_set(b, 0, 0));
    efl_assert_true(eina_value_compare(a, b) > 0);

    efl_assert_true(eina_value_list_set(b, 0, 10));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_list_set(b, 0, 1));
    efl_assert_true(eina_value_compare(a, b) == 0);

   /* bigger lists are greater */
    efl_assert_true(eina_value_list_append(b, 0));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_list_append(a, 0));
    efl_assert_true(eina_value_list_append(a, 0));
    efl_assert_true(eina_value_compare(a, b) > 0);

   /* bigger lists are greater, unless an element says otherwise */
    efl_assert_true(eina_value_list_set(b, 0, 10));
    efl_assert_true(eina_value_compare(a, b) < 0);

    eina_value_flush(a);
    eina_value_flush(b);

    efl_assert_true(eina_value_hash_setup(a, EINA_VALUE_TYPE_CHAR, 0));
    efl_assert_true(eina_value_hash_setup(b, EINA_VALUE_TYPE_CHAR, 0));
    efl_assert_true(eina_value_compare(a, b) == 0);

    efl_assert_true(eina_value_hash_set(a, "abc", 1));
    efl_assert_true(eina_value_hash_set(a, "xyz", 2));
    efl_assert_true(eina_value_hash_set(a, "hello", 3));

    efl_assert_true(eina_value_hash_set(b, "abc", 1));
    efl_assert_true(eina_value_hash_set(b, "xyz", 2));
    efl_assert_true(eina_value_hash_set(b, "hello", 3));

    efl_assert_true(eina_value_compare(a, b) == 0);

    efl_assert_true(eina_value_hash_set(a, "abc", 0));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_hash_set(a, "abc", 10));
    efl_assert_true(eina_value_compare(a, b) > 0);

    efl_assert_true(eina_value_hash_set(a, "abc", 1));

    efl_assert_true(eina_value_hash_set(b, "abc", 0));
    efl_assert_true(eina_value_compare(a, b) > 0);

    efl_assert_true(eina_value_hash_set(b, "abc", 10));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_hash_set(b, "abc", 1));
    efl_assert_true(eina_value_compare(a, b) == 0);

   /* bigger hashs are greater */
    efl_assert_true(eina_value_hash_set(b, "newkey", 0));
    efl_assert_true(eina_value_compare(a, b) < 0);

    efl_assert_true(eina_value_hash_set(a, "newkey", 0));
    efl_assert_true(eina_value_hash_set(a, "onemorenewkey", 0));
    efl_assert_true(eina_value_compare(a, b) > 0);

   /* bigger hashs are greater, unless an element says otherwise */
    efl_assert_true(eina_value_hash_set(b, "abc", 10));
    efl_assert_true(eina_value_compare(a, b) < 0);

    eina_value_free(a);
    eina_value_free(b);
}

TEST(value_test_string)
{
    Eina_Value *value;
    const char *s;

    value = eina_value_new(EINA_VALUE_TYPE_STRING);
    efl_assert_true(value != NULL);
    efl_assert_true(eina_value_set(value, "hello world!"));
    efl_assert_true(eina_value_get(value, &s));
    efl_assert_str_eq(s, "hello world!");

    efl_assert_true(eina_value_set(value, "eina-value"));
    efl_assert_true(eina_value_get(value, &s));
    efl_assert_str_eq(s, "eina-value");

    eina_value_flush(value);
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_STRING));

    efl_assert_true(eina_value_set(value, "profusion"));
    efl_assert_true(eina_value_get(value, &s));
    efl_assert_str_eq(s, "profusion");

    eina_value_free(value);
}

TEST(value_test_pvariant)
{
    Eina_Value    *value;
    char           c, in_c;
    short          s, in_s;
    int            i, in_i;
    long           l, in_l;
    int64_t        i64, in_i64;
    unsigned char  uc, in_uc;
    unsigned short us, in_us;
    unsigned int   ui, in_ui;
    unsigned long  ul, in_ul;
    uint64_t       u64, in_u64;
    float          f, in_f;
    double         d, in_d;
    const char    *str, *in_str;

    value = eina_value_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(value != NULL);
    in_c = 'x';
    efl_assert_true(eina_value_pset(value, &in_c));
    efl_assert_true(eina_value_pget(value, &c));
    efl_assert_true(c == 'x');
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_SHORT));
    in_s = 300;
    efl_assert_true(eina_value_pset(value, &in_s));
    efl_assert_true(eina_value_pget(value, &s));
    efl_assert_true(s == 300);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_INT));
    in_i = -12345;
    efl_assert_true(eina_value_pset(value, &in_i));
    efl_assert_true(eina_value_pget(value, &i));
    efl_assert_true(i == -12345);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_LONG));
    in_l = 0xb33f;
    efl_assert_true(eina_value_pset(value, &in_l));
    efl_assert_true(eina_value_pget(value, &l));
    efl_assert_true(l == 0xb33f);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_INT64));
    in_i64 = 0x0011223344556677;
    efl_assert_true(eina_value_pset(value, &in_i64));
    efl_assert_true(eina_value_pget(value, &i64));
    efl_assert_true(i64 == 0x0011223344556677);
    eina_value_flush(value);

   /* unsigned: */

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UCHAR));
    in_uc = 200;
    efl_assert_true(eina_value_pset(value, &in_uc));
    efl_assert_true(eina_value_pget(value, &uc));
    efl_assert_true(uc == 200);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_USHORT));
    in_us = 65535;
    efl_assert_true(eina_value_pset(value, &in_us));
    efl_assert_true(eina_value_pget(value, &us));
    efl_assert_true(us == 65535);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UINT));
    in_ui = 4000000000U;
    efl_assert_true(eina_value_pset(value, &in_ui));
    efl_assert_true(eina_value_pget(value, &ui));
    efl_assert_true(ui == 4000000000U);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_ULONG));
    in_ul = 3000000001UL;
    efl_assert_true(eina_value_pset(value, &in_ul));
    efl_assert_true(eina_value_pget(value, &ul));
    efl_assert_true(ul == 3000000001UL);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UINT64));
    in_u64 = 0x1122334455667788;
    efl_assert_true(eina_value_pset(value, &in_u64));
    efl_assert_true(eina_value_pget(value, &u64));
    efl_assert_true(u64 == 0x1122334455667788);
    eina_value_flush(value);

   /* floating point */
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_FLOAT));
    in_f = 0.1234;
    efl_assert_true(eina_value_pset(value, &in_f));
    efl_assert_true(eina_value_pget(value, &f));
    efl_assert_true(CHECK_FP(0.1234, f));
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_DOUBLE));
    in_d = 34567.8;
    efl_assert_true(eina_value_pset(value, &in_d));
    efl_assert_true(eina_value_pget(value, &d));
    efl_assert_true(CHECK_FP(34567.8, d));
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_STRING));
    in_str = "hello world!";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "hello world!");

    in_str = "eina-value";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "eina-value");

    eina_value_flush(value);
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_STRING));

    in_str = "profusion";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "profusion");

    eina_value_free(value);
}

TEST(value_test_to_string)
{
    Eina_Value    *value;
    char           c, in_c;
    short          s, in_s;
    int            i, in_i;
    long           l, in_l;
    int64_t        i64, in_i64;
    unsigned char  uc, in_uc;
    unsigned short us, in_us;
    unsigned int   ui, in_ui;
    unsigned long  ul, in_ul;
    uint64_t       u64, in_u64;
    float          f, in_f;
    double         d, in_d;
    const char    *str, *in_str;
    char          *out;
    char           buf[256];

    value = eina_value_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(value != NULL);
    in_c = 'x';
    efl_assert_true(eina_value_pset(value, &in_c));
    efl_assert_true(eina_value_pget(value, &c));
    efl_assert_true(c == 'x');
    snprintf(buf, sizeof(buf), "%hhd", in_c);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_SHORT));
    in_s = 300;
    efl_assert_true(eina_value_pset(value, &in_s));
    efl_assert_true(eina_value_pget(value, &s));
    efl_assert_true(s == 300);
    snprintf(buf, sizeof(buf), "%hd", in_s);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_INT));
    in_i = -12345;
    efl_assert_true(eina_value_pset(value, &in_i));
    efl_assert_true(eina_value_pget(value, &i));
    efl_assert_true(i == -12345);
    snprintf(buf, sizeof(buf), "%d", in_i);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_LONG));
    in_l = 0xb33f;
    efl_assert_true(eina_value_pset(value, &in_l));
    efl_assert_true(eina_value_pget(value, &l));
    efl_assert_true(l == 0xb33f);
    snprintf(buf, sizeof(buf), "%ld", in_l);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_INT64));
    in_i64 = 0x0011223344556677;
    efl_assert_true(eina_value_pset(value, &in_i64));
    efl_assert_true(eina_value_pget(value, &i64));
    efl_assert_true(i64 == 0x0011223344556677);
    snprintf(buf, sizeof(buf), "%" PRId64, in_i64);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

   /* unsigned: */

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UCHAR));
    in_uc = 200;
    efl_assert_true(eina_value_pset(value, &in_uc));
    efl_assert_true(eina_value_pget(value, &uc));
    efl_assert_true(uc == 200);
    snprintf(buf, sizeof(buf), "%hhu", in_uc);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_USHORT));
    in_us = 65535;
    efl_assert_true(eina_value_pset(value, &in_us));
    efl_assert_true(eina_value_pget(value, &us));
    efl_assert_true(us == 65535);
    snprintf(buf, sizeof(buf), "%hu", in_us);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UINT));
    in_ui = 4000000000U;
    efl_assert_true(eina_value_pset(value, &in_ui));
    efl_assert_true(eina_value_pget(value, &ui));
    efl_assert_true(ui == 4000000000U);
    snprintf(buf, sizeof(buf), "%u", in_ui);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_ULONG));
    in_ul = 3000000001UL;
    efl_assert_true(eina_value_pset(value, &in_ul));
    efl_assert_true(eina_value_pget(value, &ul));
    efl_assert_true(ul == 3000000001UL);
    snprintf(buf, sizeof(buf), "%lu", in_ul);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_UINT64));
    in_u64 = 0x1122334455667788;
    efl_assert_true(eina_value_pset(value, &in_u64));
    efl_assert_true(eina_value_pget(value, &u64));
    efl_assert_true(u64 == 0x1122334455667788);
    snprintf(buf, sizeof(buf), "%" PRIu64, in_u64);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    free(out);
    eina_value_flush(value);

   /* floating point */
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_FLOAT));
    in_f = 0.1234;
    efl_assert_true(eina_value_pset(value, &in_f));
    efl_assert_true(eina_value_pget(value, &f));
    efl_assert_true(CHECK_FP(0.1234, f));
    snprintf(buf, sizeof(buf), "%g", in_f);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_true(strncmp(buf, out, 6) == 0); /* stupid float... */
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_DOUBLE));
    in_d = 34567.8;
    efl_assert_true(eina_value_pset(value, &in_d));
    efl_assert_true(eina_value_pget(value, &d));
    efl_assert_true(CHECK_FP(34567.8, d));
    snprintf(buf, sizeof(buf), "%g", in_d);
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_true(strncmp(buf, out, 7) == 0); /* stupid double... */
    free(out);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_STRING));
    in_str = "hello world!";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "hello world!");
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(in_str, out);
    free(out);

    in_str = "eina-value";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "eina-value");
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(in_str, out);
    free(out);

    eina_value_flush(value);
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_STRING));

    in_str = "profusion";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "profusion");
    out = eina_value_to_string(value);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(in_str, out);
    free(out);

    eina_value_free(value);
}

TEST(value_test_to_binbuf)
{
    Eina_Value  *value;
    char         c, in_c;
    const char  *str, *in_str;
    Eina_Binbuf *bin;
    const char  *out;
    char         buf[256];

    value = eina_value_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(value != NULL);
    in_c = 'x';
    efl_assert_true(eina_value_pset(value, &in_c));
    efl_assert_true(eina_value_pget(value, &c));
    efl_assert_true(c == 'x');
    snprintf(buf, sizeof(buf), "%c", in_c);
    bin = eina_value_to_binbuf(value);
    out = (char *)eina_binbuf_string_get(bin);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(buf, out);
    eina_binbuf_free(bin);
    eina_value_flush(value);

    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_STRING));
    in_str = "hello world!";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "hello world!");
    bin = eina_value_to_binbuf(value);
    out = (char *)eina_binbuf_string_get(bin);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(in_str, out);
    eina_binbuf_free(bin);

    in_str = "eina-value";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "eina-value");
    bin = eina_value_to_binbuf(value);
    out = (char *)eina_binbuf_string_get(bin);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(in_str, out);
    eina_binbuf_free(bin);

    eina_value_flush(value);
    efl_assert_true(eina_value_setup(value, EINA_VALUE_TYPE_STRING));

    in_str = "profusion";
    efl_assert_true(eina_value_pset(value, &in_str));
    efl_assert_true(eina_value_pget(value, &str));
    efl_assert_str_eq(str, "profusion");
    bin = eina_value_to_binbuf(value);
    out = (char *)eina_binbuf_string_get(bin);
    efl_assert_true(out != NULL);
    efl_assert_str_eq(in_str, out);
    eina_binbuf_free(bin);

    eina_value_free(value);
}

TEST(value_test_convert_char)
{
    Eina_Value    *value, conv;
    char           c;
    short          s;
    int            i;
    long           l;
    int64_t        i64;
    unsigned char  uc;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;
    const char    *str;

    value = eina_value_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_set(value, 123));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &uc));
    efl_assert_true(uc == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &us));
    efl_assert_true(us == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &c));
    efl_assert_true(c == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &s));
    efl_assert_true(s == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == 123);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, 123));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, 123));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "123");
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_set(value, -123));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_uchar)
{
    Eina_Value    *value, conv;
    char           c;
    short          s;
    int            i;
    long           l;
    int64_t        i64;
    unsigned char  uc;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;
    const char    *str;

    value = eina_value_new(EINA_VALUE_TYPE_UCHAR);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_set(value, 31));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &uc));
    efl_assert_true(uc == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &us));
    efl_assert_true(us == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &c));
    efl_assert_true(c == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &s));
    efl_assert_true(s == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == 31);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, 31));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, 31));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "31");
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_set(value, 200));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_short)
{
    Eina_Value    *value, conv;
    short          s;
    int            i;
    long           l;
    int64_t        i64;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;
    const char    *str;

    value = eina_value_new(EINA_VALUE_TYPE_SHORT);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_set(value, 12345));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &us));
    efl_assert_true(us == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &s));
    efl_assert_true(s == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == 12345);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, 12345));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, 12345));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "12345");
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

   // check negative value
    efl_assert_true(eina_value_set(value, -12345));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_ushort)
{
    Eina_Value    *value, conv;
    int            i;
    long           l;
    int64_t        i64;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;
    const char    *str;

    value = eina_value_new(EINA_VALUE_TYPE_USHORT);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_set(value, 54321));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &us));
    efl_assert_true(us == 54321);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == 54321);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == 54321);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == 54321);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == 54321);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == 54321);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == 54321);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, 54321));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, 54321));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "54321");
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_int)
{
    Eina_Value   *value, conv;
    int           i;
    long          l;
    int64_t       i64;
    unsigned int  ui;
    unsigned long ul;
    uint64_t      u64;
    float         f;
    double        d;
    const char   *str;

    value = eina_value_new(EINA_VALUE_TYPE_INT);
    efl_assert_true(value != NULL);

    const int max_positive_signed_4_bytes = 0x7FFFFFFF;

    efl_assert_true(eina_value_set(value, max_positive_signed_4_bytes));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == (unsigned int)max_positive_signed_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == (unsigned long)max_positive_signed_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == (uint64_t)max_positive_signed_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == max_positive_signed_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == max_positive_signed_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == max_positive_signed_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, max_positive_signed_4_bytes));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, max_positive_signed_4_bytes));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "2147483647"); // "2147483647" == 0x7FFFFFFF
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

   // check negative value
    efl_assert_true(eina_value_set(value, -1234567890));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
   // value should be positive
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_uint)
{
    Eina_Value   *value, conv;
    unsigned int  ui;
    unsigned long ul;
    uint64_t      u64;
    float         f;
    double        d;
    const char   *str;

    value = eina_value_new(EINA_VALUE_TYPE_UINT);
    efl_assert_true(value != NULL);

    const unsigned int max_positive_unsigned_4_bytes = 0xFFFFFFFF;

    efl_assert_true(eina_value_set(value, max_positive_unsigned_4_bytes));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == max_positive_unsigned_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == max_positive_unsigned_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == max_positive_unsigned_4_bytes);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, max_positive_unsigned_4_bytes));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, max_positive_unsigned_4_bytes));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "4294967295"); // 4294967295 == 0xFFFFFFFF
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
   // value too big
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_long)
{
    Eina_Value    *value, conv;
    char           c;
    short          s;
    int            i;
    long           l;
    int64_t        i64;
    unsigned char  uc;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;
    const char    *str;

    value = eina_value_new(EINA_VALUE_TYPE_LONG);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_set(value, 32l));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &uc));
    efl_assert_true(uc == 32);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &us));
    efl_assert_true(us == 32);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == 32);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == 32ul);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == 32ull);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &c));
    efl_assert_true(c == 32);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &s));
    efl_assert_true(s == 32);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == 32);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == 32l);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == 32ll);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, 32));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, 32));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "32");
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_set(value, 128l));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, 256l));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, 32768l));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, 65536l));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, -32l));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_ulong)
{
    Eina_Value    *value, conv;
    char           c;
    short          s;
    int            i;
    long           l;
    int64_t        i64;
    unsigned char  uc;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;
    const char    *str;

    value = eina_value_new(EINA_VALUE_TYPE_ULONG);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_set(value, 42ul));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &uc));
    efl_assert_true(uc == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &us));
    efl_assert_true(us == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == 42ul);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(u64 == 42ull);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &c));
    efl_assert_true(c == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &s));
    efl_assert_true(s == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == 42l);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == 42ll);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, 42));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, 42));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "42");
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_set(value, 128ul));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, 256ul));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, 32768ul));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, 65536ul));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_convert_float)
{
    Eina_Value    *value, conv;
    char           c;
    short          s;
    int            i;
    long           l;
    int64_t        i64;
    unsigned char  uc;
    unsigned short us;
    unsigned int   ui;
    unsigned long  ul;
    uint64_t       u64;
    float          f;
    double         d;
    const char    *str;

    float max_float_value = FLT_MAX;
    float min_float_value = FLT_MIN;

    value = eina_value_new(EINA_VALUE_TYPE_FLOAT);
    efl_assert_true(value != NULL);
    
    efl_assert_true(eina_value_set(value, 42.354645));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &uc));
    efl_assert_true(uc == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &us));
    efl_assert_true(us == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ui));
    efl_assert_true(ui == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_ULONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &ul));
    efl_assert_true(ul == 42ul);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UINT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &u64));
    efl_assert_true(ul == 42ull);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &c));
    efl_assert_true(c == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &s));
    efl_assert_true(s == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i));
    efl_assert_true(i == 42);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_LONG));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &l));
    efl_assert_true(l == 42l);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_INT64));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &i64));
    efl_assert_true(i64 == 42ll);
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, max_float_value));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, max_float_value));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &str));
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "42.354645");
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, max_float_value));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, max_float_value));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, max_float_value));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, min_float_value));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, min_float_value));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, min_float_value));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, -max_float_value));

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_FLOAT));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &f));
    efl_assert_true(CHECK_FP(f, -max_float_value));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_DOUBLE));
    efl_assert_true(eina_value_convert(value, &conv));
    efl_assert_true(eina_value_get(&conv, &d));
    efl_assert_true(CHECK_FP(d, -max_float_value));
    eina_value_flush(&conv);

   /* negative tests */
    efl_assert_true(eina_value_set(value, -max_float_value));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_CHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, -max_float_value));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_UCHAR));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, -max_float_value));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_SHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    efl_assert_true(eina_value_set(value, -max_float_value));
    efl_assert_true(eina_value_setup(&conv, EINA_VALUE_TYPE_USHORT));
    efl_assert_fail_if(eina_value_convert(value, &conv));
    eina_value_flush(&conv);

    eina_value_free(value);
}

TEST(value_test_array)
{
    Eina_Value      *value, other;
    Eina_Value_Array desc;
    Eina_Inarray    *inarray;
    char             c;
    char             buf[1024];
    char            *str;

    value = eina_value_array_new(EINA_VALUE_TYPE_CHAR, 0);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_array_append(value, 'k'));
    efl_assert_true(eina_value_array_append(value, '-'));
    efl_assert_true(eina_value_array_append(value, 's'));

    efl_assert_true(eina_value_array_get(value, 0, &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_array_get(value, 1, &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_array_get(value, 2, &c));
    efl_assert_true(c == 's');

    efl_assert_true(eina_value_array_insert(value, 0, '!'));
    efl_assert_true(eina_value_array_get(value, 0, &c));
    efl_assert_true(c == '!');
    efl_assert_true(eina_value_array_get(value, 1, &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_array_get(value, 2, &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_array_get(value, 3, &c));
    efl_assert_true(c == 's');

    efl_assert_true(eina_value_array_set(value, 0, '*'));
    efl_assert_true(eina_value_array_get(value, 0, &c));
    efl_assert_true(c == '*');
    efl_assert_true(eina_value_array_get(value, 1, &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_array_get(value, 2, &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_array_get(value, 3, &c));
    efl_assert_true(c == 's');

    snprintf(buf,
             sizeof(buf),
             "[%d, %d, %d, %d]",
             (int)'*',
             (int)'k',
             (int)'-',
             (int)'s');

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, buf);
    free(str);

    eina_value_flush(value);
    efl_assert_true(
        eina_value_array_setup(value, EINA_VALUE_TYPE_STRINGSHARE, 2));

    efl_assert_true(eina_value_array_append(value, "Enlightenment.org"));
    efl_assert_true(eina_value_array_append(value, "X11"));
    efl_assert_true(eina_value_array_append(value, "Pants"));
    efl_assert_true(eina_value_array_append(value, "on!!!"));
    efl_assert_true(eina_value_array_append(value, "k-s"));

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "[Enlightenment.org, X11, Pants, on!!!, k-s]");
    free(str);

    eina_value_flush(value);
    efl_assert_true(eina_value_array_setup(value, EINA_VALUE_TYPE_CHAR, 0));
    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_CHAR));

    efl_assert_true(eina_value_set(&other, 100));
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 100);

    efl_assert_true(eina_value_convert(&other, value));
    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "[100]");
    free(str);

    efl_assert_true(eina_value_array_set(value, 0, 33));
    efl_assert_true(eina_value_convert(value, &other));
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 33);

    inarray = eina_inarray_new(sizeof(char), 0);
    efl_assert_true(inarray != NULL);
    c = 11;
    efl_assert_true(eina_inarray_push(inarray, &c) >= 0);
    c = 21;
    efl_assert_true(eina_inarray_push(inarray, &c) >= 0);
    c = 31;
    efl_assert_true(eina_inarray_push(inarray, &c) >= 0);
    desc.subtype = EINA_VALUE_TYPE_CHAR;
    desc.step    = 0;
    desc.array   = inarray;
    efl_assert_true(eina_value_set(value, desc)); /* manually configure */
    eina_inarray_free(inarray);

    efl_assert_true(eina_value_array_get(value, 0, &c));
    efl_assert_true(c == 11);
    efl_assert_true(eina_value_array_get(value, 1, &c));
    efl_assert_true(c == 21);
    efl_assert_true(eina_value_array_get(value, 2, &c));
    efl_assert_true(c == 31);

    eina_value_free(value);
}

TEST(value_test_list_insert)
{
    Eina_Value *value;
    char        c;
    char       *str;
    char        buf[1024];

    value = eina_value_list_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_list_count(value) == 0);
    efl_assert_true(!eina_value_list_insert(value, 1, '-'));
    efl_assert_true(!eina_value_list_insert(value, 10, 'j'));
    efl_assert_true(eina_value_list_count(value) == 0);

    efl_assert_true(eina_value_list_insert(value, 0, 'k'));
    efl_assert_true(eina_value_list_insert(value, 1, '-'));
    efl_assert_true(eina_value_list_insert(value, 0, 's'));
    efl_assert_true(eina_value_list_insert(value, 1, 'j'));
    efl_assert_true(eina_value_list_count(value) == 4);

    efl_assert_true(eina_value_list_get(value, 0, &c));
    efl_assert_true(c == 's');
    efl_assert_true(eina_value_list_get(value, 1, &c));
    efl_assert_true(c == 'j');
    efl_assert_true(eina_value_list_get(value, 2, &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_list_get(value, 3, &c));
    efl_assert_true(c == '-');

    snprintf(buf,
             sizeof(buf),
             "[%d, %d, %d, %d]",
             (int)'s',
             (int)'j',
             (int)'k',
             (int)'-');

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, buf);

    free(str);
    eina_value_free(value);
}

TEST(value_test_list)
{
    Eina_Value     *value, other;
    Eina_Value_List desc;
    char            c;
    char            buf[1024];
    char           *str;
    const char     *s;

    value = eina_value_list_new(EINA_VALUE_TYPE_CHAR);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_list_append(value, 'k'));
    efl_assert_true(eina_value_list_append(value, '-'));
    efl_assert_true(eina_value_list_append(value, 's'));

    efl_assert_true(eina_value_list_get(value, 0, &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_list_get(value, 1, &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_list_get(value, 2, &c));
    efl_assert_true(c == 's');

    efl_assert_true(eina_value_list_insert(value, 0, '!'));
    efl_assert_true(eina_value_list_get(value, 0, &c));
    efl_assert_true(c == '!');
    efl_assert_true(eina_value_list_get(value, 1, &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_list_get(value, 2, &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_list_get(value, 3, &c));
    efl_assert_true(c == 's');

    efl_assert_true(eina_value_list_set(value, 0, '*'));
    efl_assert_true(eina_value_list_get(value, 0, &c));
    efl_assert_true(c == '*');
    efl_assert_true(eina_value_list_get(value, 1, &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_list_get(value, 2, &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_list_get(value, 3, &c));
    efl_assert_true(c == 's');

    snprintf(buf,
             sizeof(buf),
             "[%d, %d, %d, %d]",
             (int)'*',
             (int)'k',
             (int)'-',
             (int)'s');

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, buf);
    free(str);

    eina_value_flush(value);
    efl_assert_true(eina_value_list_setup(value, EINA_VALUE_TYPE_STRINGSHARE));

    efl_assert_true(eina_value_list_append(value, "Enlightenment.org"));
    efl_assert_true(eina_value_list_append(value, "X11"));
    efl_assert_true(eina_value_list_append(value, "Pants"));
    efl_assert_true(eina_value_list_append(value, "on!!!"));
    efl_assert_true(eina_value_list_append(value, "k-s"));

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "[Enlightenment.org, X11, Pants, on!!!, k-s]");
    free(str);

    eina_value_flush(value);
    efl_assert_true(eina_value_list_setup(value, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_CHAR));

    efl_assert_true(eina_value_set(&other, 100));
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 100);

    efl_assert_true(eina_value_convert(&other, value));
    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "[100]");
    free(str);

    efl_assert_true(eina_value_list_set(value, 0, 33));
    efl_assert_true(eina_value_convert(value, &other));
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 33);

    desc.subtype = EINA_VALUE_TYPE_STRING;
    desc.list    = NULL;
    desc.list    = eina_list_append(desc.list, "hello");
    desc.list    = eina_list_append(desc.list, "world");
    desc.list    = eina_list_append(desc.list, "eina");
    efl_assert_true(eina_list_count(desc.list) == 3);
    efl_assert_true(eina_value_set(value, desc));
    eina_list_free(desc.list);

    efl_assert_true(eina_value_list_get(value, 0, &s));
    efl_assert_true(s != NULL);
    efl_assert_str_eq(s, "hello");
    efl_assert_true(eina_value_list_get(value, 1, &s));
    efl_assert_true(s != NULL);
    efl_assert_str_eq(s, "world");
    efl_assert_true(eina_value_list_get(value, 2, &s));
    efl_assert_true(s != NULL);
    efl_assert_str_eq(s, "eina");

    eina_value_free(value);
}

TEST(value_test_hash)
{
    Eina_Value     *value, other;
    Eina_Value_Hash desc;
    char            c;
    char            buf[1024];
    char          **ptr;
    char           *str;
    const char     *s;

    value = eina_value_hash_new(EINA_VALUE_TYPE_CHAR, 0);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_hash_set(value, "first", 'k'));
    efl_assert_true(eina_value_hash_set(value, "second", '-'));
    efl_assert_true(eina_value_hash_set(value, "third", 's'));

    efl_assert_true(eina_value_hash_get(value, "first", &c));
    efl_assert_true(c == 'k');
    efl_assert_true(eina_value_hash_get(value, "second", &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_hash_get(value, "third", &c));
    efl_assert_true(c == 's');

    efl_assert_true(eina_value_hash_set(value, "first", '!'));
    efl_assert_true(eina_value_hash_get(value, "first", &c));
    efl_assert_true(c == '!');
    efl_assert_true(eina_value_hash_get(value, "second", &c));
    efl_assert_true(c == '-');
    efl_assert_true(eina_value_hash_get(value, "third", &c));
    efl_assert_true(c == 's');

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);

    snprintf(buf, sizeof(buf), "first: %d", (int)'!');
    efl_assert_true(strstr(str, buf) != NULL);

    snprintf(buf, sizeof(buf), "second: %d", (int)'-');
    efl_assert_true(strstr(str, buf) != NULL);

    snprintf(buf, sizeof(buf), "third: %d", (int)'s');
    efl_assert_true(strstr(str, buf) != NULL);

    free(str);

    eina_value_flush(value);
    efl_assert_true(
        eina_value_hash_setup(value, EINA_VALUE_TYPE_STRINGSHARE, 0));

    efl_assert_true(eina_value_hash_set(value, "a", "Enlightenment.org"));
    efl_assert_true(eina_value_hash_set(value, "b", "X11"));
    efl_assert_true(eina_value_hash_set(value, "c", "Pants"));
    efl_assert_true(eina_value_hash_set(value, "d", "on!!!"));
    efl_assert_true(eina_value_hash_set(value, "e", "k-s"));

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);

    eina_strlcpy(buf, "a: Enlightenment.org", sizeof(buf));
    efl_assert_true(strstr(str, buf) != NULL);

    eina_strlcpy(buf, "b: X11", sizeof(buf));
    efl_assert_true(strstr(str, buf) != NULL);

    eina_strlcpy(buf, "c: Pants", sizeof(buf));
    efl_assert_true(strstr(str, buf) != NULL);

    eina_strlcpy(buf, "d: on!!!", sizeof(buf));
    efl_assert_true(strstr(str, buf) != NULL);

    eina_strlcpy(buf, "e: k-s", sizeof(buf));
    efl_assert_true(strstr(str, buf) != NULL);

    free(str);

    eina_value_flush(value);
    efl_assert_true(eina_value_hash_setup(value, EINA_VALUE_TYPE_CHAR, 0));
    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_CHAR));

    efl_assert_true(eina_value_set(&other, 100));
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 100);

    efl_assert_true(eina_value_hash_set(value, "first", 33));
    efl_assert_true(eina_value_convert(value, &other));
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 33);

    desc.subtype            = EINA_VALUE_TYPE_STRING;
    desc.buckets_power_size = 0;
    desc.hash               = eina_hash_string_small_new(NULL);
    efl_assert_true(desc.hash != NULL);
   /* watch out hash pointer is to a size of subtype->value_size! */
    ptr  = malloc(sizeof(char *));
    *ptr = "there";
    efl_assert_true(eina_hash_add(desc.hash, "hi", ptr));
    ptr  = malloc(sizeof(char *));
    *ptr = "y";
    efl_assert_true(eina_hash_add(desc.hash, "x", ptr));
    efl_assert_true(eina_value_set(value, desc));

    free(eina_hash_find(desc.hash, "hi"));
    free(eina_hash_find(desc.hash, "x"));
    eina_hash_free(desc.hash);

    efl_assert_true(eina_value_hash_get(value, "hi", &s));
    efl_assert_true(s != NULL);
    efl_assert_str_eq(s, "there");

    efl_assert_true(eina_value_hash_get(value, "x", &s));
    efl_assert_true(s != NULL);
    efl_assert_str_eq(s, "y");

    eina_value_free(value);
}

TEST(value_test_timeval)
{
    Eina_Value    *value, other;
    struct timeval itv, otv;
    time_t         t;
    char           c;
    char          *str;
    char           buf[64];

    value = eina_value_new(EINA_VALUE_TYPE_TIMEVAL);
    efl_assert_true(value != NULL);

    itv.tv_sec  = 1;
    itv.tv_usec = 123;
    efl_assert_true(eina_value_set(value, itv));
    efl_assert_true(eina_value_get(value, &otv));
    efl_assert_true(itv.tv_sec == otv.tv_sec);
    efl_assert_true(itv.tv_usec == otv.tv_usec);

    itv.tv_sec  = 3;
    itv.tv_usec = -1;
    efl_assert_true(eina_value_set(value, itv));
    efl_assert_true(eina_value_get(value, &otv));
    itv.tv_sec  = 2;
    itv.tv_usec = 999999;
    efl_assert_true(itv.tv_sec == otv.tv_sec);
    efl_assert_true(itv.tv_usec == otv.tv_usec);

    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_CHAR));
    efl_assert_true(eina_value_convert(value, &other));
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 2);
    eina_value_flush(&other);

    itv.tv_sec  = 12345;
    itv.tv_usec = 6789;
    efl_assert_true(eina_value_set(value, itv));
    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);

    t = itv.tv_sec;
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&(t)));
    efl_assert_str_eq(str, buf);
    free(str);

    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_TIMEVAL));
    efl_assert_true(eina_value_set(&other, itv));
    efl_assert_true(eina_value_compare(value, &other) == 0);

    itv.tv_sec++;
    efl_assert_true(eina_value_set(&other, itv));
    efl_assert_true(eina_value_compare(value, &other) < 0);

    itv.tv_sec -= 2;
    efl_assert_true(eina_value_set(&other, itv));
    efl_assert_true(eina_value_compare(value, &other) > 0);

    itv.tv_sec++;
    efl_assert_true(eina_value_set(&other, itv));
    efl_assert_true(eina_value_compare(value, &other) == 0);

    itv.tv_usec++;
    efl_assert_true(eina_value_set(&other, itv));
    efl_assert_true(eina_value_compare(value, &other) < 0);

    itv.tv_usec -= 2;
    efl_assert_true(eina_value_set(&other, itv));
    efl_assert_true(eina_value_compare(value, &other) > 0);

    itv.tv_usec++;
    efl_assert_true(eina_value_set(&other, itv));
    efl_assert_true(eina_value_compare(value, &other) == 0);

    eina_value_flush(&other);

    eina_value_free(value);
}

TEST(value_test_blob)
{
    Eina_Value     *value, other;
    Eina_Value_Blob in, out;
    unsigned char   blob[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int             i      = 0x11223344;
    char           *str;

    value = eina_value_new(EINA_VALUE_TYPE_BLOB);
    efl_assert_true(value != NULL);

    in.ops    = NULL;
    in.memory = blob;
    in.size   = sizeof(blob);
    efl_assert_true(eina_value_set(value, in));
    efl_assert_true(eina_value_get(value, &out));
    efl_assert_true(out.memory == blob);
    efl_assert_true(out.size == sizeof(blob));
    efl_assert_true(memcmp(in.memory, out.memory, in.size) == 0);

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "BLOB(10, [01 02 03 04 05 06 07 08 09 0a])");
    free(str);

    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_INT));
    efl_assert_true(eina_value_set(&other, i));
    efl_assert_true(eina_value_convert(&other, value));
    efl_assert_true(eina_value_get(value, &out));

    efl_assert_true(out.memory != NULL);
    efl_assert_true(out.size == sizeof(int));
    efl_assert_true(memcmp(&i, out.memory, sizeof(int)) == 0);

    eina_value_flush(&other);

    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_STRING));
    efl_assert_true(eina_value_set(&other, "hi there!"));
    efl_assert_true(eina_value_convert(&other, value));
    efl_assert_true(eina_value_get(value, &out));
    efl_assert_true(out.memory != NULL);
    efl_assert_true(out.size == sizeof("hi there!"));
    efl_assert_str_eq(out.memory, "hi there!");

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "BLOB(10, [68 69 20 74 68 65 72 65 21 00])");
    free(str);

    eina_value_flush(&other);

    efl_assert_true(eina_value_array_setup(&other, EINA_VALUE_TYPE_CHAR, 0));
    efl_assert_true(eina_value_array_append(&other, 0xa));
    efl_assert_true(eina_value_array_append(&other, 0xb));
    efl_assert_true(eina_value_array_append(&other, 0xc));
    efl_assert_true(eina_value_convert(&other, value));
    efl_assert_true(eina_value_get(value, &out));
    efl_assert_true(out.memory != NULL);
    efl_assert_true(out.size == 3);

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "BLOB(3, [0a 0b 0c])");
    free(str);

    eina_value_flush(&other);

    efl_assert_true(eina_value_setup(&other, EINA_VALUE_TYPE_BLOB));
    efl_assert_true(eina_value_set(&other, in));
    efl_assert_true(eina_value_convert(value, &other));
    efl_assert_true(eina_value_get(&other, &out));
    efl_assert_true(out.memory != NULL);
    efl_assert_true(out.size == 3);

    str = eina_value_to_string(&other);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "BLOB(3, [0a 0b 0c])");
    free(str);

    eina_value_flush(&other);

    eina_value_free(value);
}

TEST(value_test_struct)
{
    struct mybigst
    {
        int a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, x;
    };

    const Eina_Value_Struct_Member mybigst_members[] = {
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, a),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, b),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, c),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, d),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, e),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, f),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, g),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, h),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, i),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, j),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, k),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, l),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, m),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, n),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, o),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, p),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, q),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, r),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, s),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, t),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, u),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, v),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct mybigst, x),
        EINA_VALUE_STRUCT_MEMBER_SENTINEL
    };
    const Eina_Value_Struct_Desc mybigst_desc = {
        EINA_VALUE_STRUCT_DESC_VERSION,
        EINA_VALUE_STRUCT_OPERATIONS_BINSEARCH,
        mybigst_members,
        23,
        sizeof(struct mybigst)
    };

    struct myst
    {
        int  i;
        char c;
    };

    const Eina_Value_Struct_Member myst_members[] = {
        { "i",  EINA_VALUE_TYPE_INT, 0},
        { "c", EINA_VALUE_TYPE_CHAR, 4},
        {NULL,                 NULL, 0}
    };
    const Eina_Value_Struct_Desc myst_desc = { EINA_VALUE_STRUCT_DESC_VERSION,
                                               NULL,
                                               myst_members,
                                               2,
                                               sizeof(struct myst) };
    Eina_Value                  *value, other;
    int                          i;
    char                         c;
    char                        *str;

    value = eina_value_struct_new(&myst_desc);
    efl_assert_true(value != NULL);

    efl_assert_true(eina_value_struct_set(value, "i", 5678));
    efl_assert_true(eina_value_struct_set(value, "c", 0xf));

    efl_assert_true(eina_value_struct_get(value, "i", &i));
    efl_assert_true(i == 5678);
    efl_assert_true(eina_value_struct_get(value, "c", &c));
    efl_assert_true(c == 0xf);

    efl_assert_true(
        eina_value_struct_member_value_get(value, myst_members + 0, &other));
    efl_assert_true(other.type == EINA_VALUE_TYPE_INT);
    efl_assert_true(eina_value_get(&other, &i));
    efl_assert_true(i == 5678);
    eina_value_flush(&other);

    efl_assert_true(
        eina_value_struct_member_value_get(value, myst_members + 1, &other));
    efl_assert_true(other.type == EINA_VALUE_TYPE_CHAR);
    efl_assert_true(eina_value_get(&other, &c));
    efl_assert_true(c == 0xf);
    eina_value_flush(&other);

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "{i: 5678, c: 15}");
    free(str);

    efl_assert_fail_if(eina_value_struct_get(value, "x", 1234));

    i = 0x11223344;
    efl_assert_true(eina_value_struct_pset(value, "i", &i));
    i = -1;
    efl_assert_true(eina_value_struct_pget(value, "i", &i));
    efl_assert_true(i == 0x11223344);

    efl_assert_true(eina_value_copy(value, &other));
    str = eina_value_to_string(&other);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str, "{i: 287454020, c: 15}");
    free(str);

    eina_value_flush(&other);

    efl_assert_true(eina_value_struct_setup(&other, &mybigst_desc));
    efl_assert_true(eina_value_struct_set(&other, "a", 1));
    efl_assert_true(eina_value_struct_set(&other, "b", 2));
    efl_assert_true(eina_value_struct_set(&other, "c", 3));
    efl_assert_true(eina_value_struct_set(&other, "d", 4));
    efl_assert_true(eina_value_struct_set(&other, "e", 5));
    efl_assert_true(eina_value_struct_set(&other, "f", 6));
    efl_assert_true(eina_value_struct_set(&other, "g", 7));
    efl_assert_true(eina_value_struct_set(&other, "h", 8));
    efl_assert_true(eina_value_struct_set(&other, "i", 9));
    efl_assert_true(eina_value_struct_set(&other, "j", 10));
    efl_assert_true(eina_value_struct_set(&other, "k", 12));
    efl_assert_true(eina_value_struct_set(&other, "l", 13));
    efl_assert_true(eina_value_struct_set(&other, "m", 14));
    efl_assert_true(eina_value_struct_set(&other, "n", 15));
    efl_assert_true(eina_value_struct_set(&other, "o", 16));
    efl_assert_true(eina_value_struct_set(&other, "p", 17));
    efl_assert_true(eina_value_struct_set(&other, "q", 18));
    efl_assert_true(eina_value_struct_set(&other, "r", 19));
    efl_assert_true(eina_value_struct_set(&other, "s", 20));
    efl_assert_true(eina_value_struct_set(&other, "t", 21));
    efl_assert_true(eina_value_struct_set(&other, "u", 22));
    efl_assert_true(eina_value_struct_set(&other, "v", 23));
    efl_assert_true(eina_value_struct_set(&other, "x", 24));

    efl_assert_true(eina_value_struct_get(&other, "a", &i));
    efl_assert_true(i == 1);
    efl_assert_true(eina_value_struct_get(&other, "b", &i));
    efl_assert_true(i == 2);
    efl_assert_true(eina_value_struct_get(&other, "c", &i));
    efl_assert_true(i == 3);
    efl_assert_true(eina_value_struct_get(&other, "d", &i));
    efl_assert_true(i == 4);
    efl_assert_true(eina_value_struct_get(&other, "e", &i));
    efl_assert_true(i == 5);
    efl_assert_true(eina_value_struct_get(&other, "f", &i));
    efl_assert_true(i == 6);
    efl_assert_true(eina_value_struct_get(&other, "g", &i));
    efl_assert_true(i == 7);
    efl_assert_true(eina_value_struct_get(&other, "h", &i));
    efl_assert_true(i == 8);
    efl_assert_true(eina_value_struct_get(&other, "i", &i));
    efl_assert_true(i == 9);
    efl_assert_true(eina_value_struct_get(&other, "j", &i));
    efl_assert_true(i == 10);
    efl_assert_true(eina_value_struct_get(&other, "k", &i));
    efl_assert_true(i == 12);
    efl_assert_true(eina_value_struct_get(&other, "l", &i));
    efl_assert_true(i == 13);
    efl_assert_true(eina_value_struct_get(&other, "m", &i));
    efl_assert_true(i == 14);
    efl_assert_true(eina_value_struct_get(&other, "n", &i));
    efl_assert_true(i == 15);
    efl_assert_true(eina_value_struct_get(&other, "o", &i));
    efl_assert_true(i == 16);
    efl_assert_true(eina_value_struct_get(&other, "p", &i));
    efl_assert_true(i == 17);
    efl_assert_true(eina_value_struct_get(&other, "q", &i));
    efl_assert_true(i == 18);
    efl_assert_true(eina_value_struct_get(&other, "r", &i));
    efl_assert_true(i == 19);
    efl_assert_true(eina_value_struct_get(&other, "s", &i));
    efl_assert_true(i == 20);
    efl_assert_true(eina_value_struct_get(&other, "t", &i));
    efl_assert_true(i == 21);
    efl_assert_true(eina_value_struct_get(&other, "u", &i));
    efl_assert_true(i == 22);
    efl_assert_true(eina_value_struct_get(&other, "v", &i));
    efl_assert_true(i == 23);
    efl_assert_true(eina_value_struct_get(&other, "x", &i));
    efl_assert_true(i == 24);

    str = eina_value_to_string(&other);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str,
                      "{a: 1, b: 2, c: 3, d: 4, e: 5, f: 6, g: 7, h: 8, i: 9, "
                      "j: 10, k: 12, l: 13, m: 14, n: 15, o: 16, p: 17, q: 18, "
                      "r: 19, s: 20, t: 21, u: 22, v: 23, x: 24}");
    free(str);

    eina_value_flush(&other);
    eina_value_free(value);
}

TEST(value_test_array_of_struct)
{
    struct myst
    {
        int         a, b, c;
        const char *s;
    };

    const Eina_Value_Struct_Member myst_members[] = {
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct myst, a),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct myst, b),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT, struct myst, c),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_STRING, struct myst, s),
        EINA_VALUE_STRUCT_MEMBER_SENTINEL
    };
    const Eina_Value_Struct_Desc myst_desc = {
        EINA_VALUE_STRUCT_DESC_VERSION,
        EINA_VALUE_STRUCT_OPERATIONS_BINSEARCH,
        myst_members,
        4,
        sizeof(struct myst)
    };
    Eina_Value *value, array_item;
    char       *str;
    int         i;

    value = eina_value_array_new(EINA_VALUE_TYPE_STRUCT, 0);
    efl_assert_true(value != NULL);

    for (i = 0; i < 10; i++)
    {
        Eina_Value_Struct desc;
        struct myst       st;
        char              buf[64];

        snprintf(buf, sizeof(buf), "item%02d", i);
        st.a = i;
        st.b = i * 10;
        st.c = i * 100;
        st.s = buf;

        desc.desc   = &myst_desc;
        desc.memory = &st;
        efl_assert_true(eina_value_array_append(value, desc));
    }

    str = eina_value_to_string(value);
    efl_assert_true(str != NULL);
    efl_assert_str_eq(str,
                      "["
                      "{a: 0, b: 0, c: 0, s: item00}, "
                      "{a: 1, b: 10, c: 100, s: item01}, "
                      "{a: 2, b: 20, c: 200, s: item02}, "
                      "{a: 3, b: 30, c: 300, s: item03}, "
                      "{a: 4, b: 40, c: 400, s: item04}, "
                      "{a: 5, b: 50, c: 500, s: item05}, "
                      "{a: 6, b: 60, c: 600, s: item06}, "
                      "{a: 7, b: 70, c: 700, s: item07}, "
                      "{a: 8, b: 80, c: 800, s: item08}, "
                      "{a: 9, b: 90, c: 900, s: item09}"
                      "]");
    free(str);

    eina_value_array_value_get(value, 2, &array_item);
    eina_value_struct_get(&array_item, "a", &i);
    efl_assert_int_eq(i, 2);
    eina_value_struct_get(&array_item, "b", &i);
    efl_assert_int_eq(i, 20);
    eina_value_struct_get(&array_item, "c", &i);
    efl_assert_int_eq(i, 200);
    eina_value_struct_get(&array_item, "s", &str);
    efl_assert_str_eq(str, "item02");
    eina_value_flush(&array_item);

    eina_value_free(value);
}

TEST(value_test_optional_int)
{
   /* Eina_Value *value = eina_value_new(EINA_VALUE_TYPE_OPTIONAL); */
   /* Efl_Bool is_empty; */
   /* efl_assert_true(eina_value_optional_empty_is(value, &is_empty)); */
   /* efl_assert_true(is_empty); */

   /* // sets expectation */
   /* int expected_value = -12345; */
    /* efl_assert_true(eina_value_optional_pset(value, EINA_VALUE_TYPE_INT, &expected_value)); */
    /* efl_assert_true(eina_value_optional_empty_is(value, &is_empty)); */
    /* efl_assert_true(!is_empty); */

    /* // gets the actual value */
    /* int actual_value; */
    /* efl_assert_true(eina_value_optional_pget(value, &actual_value)); */
    /* efl_assert_int_eq(expected_value, actual_value); */

    /* // resets the optional */
    /* efl_assert_true(eina_value_optional_reset(value)); */
    /* efl_assert_true(eina_value_optional_empty_is(value, &is_empty)); */
    /* efl_assert_true(is_empty); */

    /* // Sets expectation again after reset */
    /* expected_value = -54321; */
    /* efl_assert_true(eina_value_optional_pset(value, EINA_VALUE_TYPE_INT, &expected_value)); */
    /* efl_assert_true(eina_value_optional_empty_is(value, &is_empty)); */
    /* efl_assert_true(!is_empty); */

    /* // gets the actual value */
    /* efl_assert_true(eina_value_optional_pget(value, &actual_value)); */
    /* efl_assert_int_eq(expected_value, actual_value); */

    /* eina_value_free(value); */
}

TEST(value_test_optional_string)
{
    Eina_Value *value = eina_value_new(EINA_VALUE_TYPE_OPTIONAL);
    Efl_Bool    is_empty;
    efl_assert_true(eina_value_optional_empty_is(value, &is_empty));
    efl_assert_true(is_empty);
    efl_assert_true(EINA_VALUE_TYPE_OPTIONAL);

    // sets expectation
    const char *expected_value = "Hello world!";
    efl_assert_true(eina_value_optional_pset(value,
                                             EINA_VALUE_TYPE_STRING,
                                             &expected_value));
    efl_assert_true(eina_value_optional_empty_is(value, &is_empty));
    efl_assert_true(!is_empty);

    // gets the actual value
    const char *actual_value;
    efl_assert_true(eina_value_optional_pget(value, &actual_value));
    efl_assert_str_eq(expected_value, actual_value);

    // resets the optional
    efl_assert_true(eina_value_optional_reset(value));
    efl_assert_true(eina_value_optional_empty_is(value, &is_empty));
    efl_assert_true(is_empty);

    // Sets expectation again after reset
    expected_value = "!dlrow olleH";
    efl_assert_true(eina_value_optional_pset(value,
                                             EINA_VALUE_TYPE_STRING,
                                             &expected_value));
    efl_assert_true(eina_value_optional_empty_is(value, &is_empty));
    efl_assert_true(!is_empty);

    // gets the actual value
    efl_assert_true(eina_value_optional_pget(value, &actual_value));
    efl_assert_str_eq(expected_value, actual_value);

    eina_value_free(value);
}

TEST(value_test_optional_struct_members)
{
    struct s
    {
        int64_t             a;
        Eina_Value_Optional text;
        int64_t             b;
    };

    const Eina_Value_Struct_Member members[] = {
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT64, struct s, a),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_OPTIONAL, struct s, text),
        EINA_VALUE_STRUCT_MEMBER(EINA_VALUE_TYPE_INT64, struct s, b),
        EINA_VALUE_STRUCT_MEMBER_SENTINEL
    };
    const Eina_Value_Struct_Desc desc = { EINA_VALUE_STRUCT_DESC_VERSION,
                                          NULL,
                                          members,
                                          3,
                                          sizeof(struct s) };

    Eina_Value *value = eina_value_struct_new(&desc);
    efl_assert_ptr_ne(NULL, value);

    int64_t expected_a = 0x1234567887654321ll;
    efl_assert_true(eina_value_struct_set(value, "a", expected_a));
    int64_t actual_a;
    efl_assert_true(eina_value_struct_get(value, "a", &actual_a));
    efl_assert_int_eq(expected_a, actual_a);

    int64_t expected_b = 0xEEDCBA9889ABCDEFll;
    efl_assert_true(eina_value_struct_set(value, "b", expected_b));
    int64_t actual_b;
    efl_assert_true(eina_value_struct_get(value, "b", &actual_b));
    efl_assert_int_eq(expected_b, actual_b);

    Eina_Value expected_value;
    efl_assert_true(
        eina_value_setup(&expected_value, EINA_VALUE_TYPE_OPTIONAL));
    const char *str = "Hello world!";
    efl_assert_true(eina_value_optional_pset(&expected_value,
                                             EINA_VALUE_TYPE_STRING,
                                             &str));
    efl_assert_true(
        eina_value_struct_value_set(value, "text", &expected_value));

    Eina_Value actual_value;
    efl_assert_true(eina_value_struct_value_get(value, "text", &actual_value));
    efl_assert_true(eina_value_compare(&expected_value, &actual_value) == 0);

    // tests if the value have been overriden
    efl_assert_true(eina_value_struct_get(value, "a", &actual_a));
    efl_assert_int_eq(expected_a, actual_a);
    efl_assert_true(eina_value_struct_get(value, "b", &actual_b));
    efl_assert_int_eq(expected_b, actual_b);

    eina_value_flush(&actual_value);
    eina_value_flush(&expected_value);

    eina_value_free(value);
}

TEST(value_test_value)
{
    Eina_Value  composed, v;
    const int   ivalue = 3500;
    int         i;
    const char *str;

    efl_assert_fail_if(!eina_value_setup(&composed, EINA_VALUE_TYPE_VALUE));
    efl_assert_fail_if(!eina_value_setup(&v, EINA_VALUE_TYPE_INT));
    efl_assert_fail_if(!eina_value_set(&v, ivalue));
    efl_assert_fail_if(!eina_value_set(&composed, v));

    efl_assert_fail_if(!eina_value_get(&composed, &v));
    efl_assert_fail_if(!eina_value_get(&v, &i));
    efl_assert_fail_if(i != ivalue);

    eina_value_flush(&v);
    efl_assert_fail_if(!eina_value_setup(&v, EINA_VALUE_TYPE_STRING));
    efl_assert_fail_if(!eina_value_convert(&composed, &v));
    efl_assert_fail_if(!eina_value_get(&v, &str));
    efl_assert_str_eq(str, "3500");

    eina_value_flush(&v);
    efl_assert_fail_if(!eina_value_copy(&composed, &v));
    efl_assert_fail_if(eina_value_compare(&composed, &v));

    eina_value_flush(&v);
    eina_value_flush(&composed);
}

TEST(value_test_value_string)
{
    Eina_Value  composed, v, str_v;
    const char *msg = "A string", *str;

    efl_assert_fail_if(!eina_value_setup(&composed, EINA_VALUE_TYPE_VALUE));
    efl_assert_fail_if(!eina_value_setup(&v, EINA_VALUE_TYPE_STRING));
    efl_assert_fail_if(!eina_value_set(&v, msg));
    efl_assert_fail_if(!eina_value_set(&composed, v));

    eina_value_flush(&v);
    efl_assert_fail_if(!eina_value_copy(&composed, &v));
    efl_assert_fail_if(eina_value_compare(&composed, &v));

    efl_assert_fail_if(!eina_value_get(&composed, &str_v));
    efl_assert_fail_if(!eina_value_get(&str_v, &str));
    efl_assert_str_eq(str, msg);
    eina_value_flush(&str_v);

    str = NULL;
    efl_assert_fail_if(!eina_value_get(&v, &str_v));
    efl_assert_fail_if(!eina_value_get(&str_v, &str));
    efl_assert_str_eq(str, msg);

    eina_value_flush(&str_v);
    eina_value_flush(&composed);
    eina_value_flush(&v);
}
