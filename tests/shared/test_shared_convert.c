/*
 * Copyright (C) 2008 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <locale.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(convert_simple)
{
    char tmp[128];
    char ref[128];

    efl_assert_fail_if(eina_convert_itoa(0, tmp) != 1);
    efl_assert_fail_if(strcmp(tmp, "0") != 0);

    efl_assert_fail_if(eina_convert_itoa(-1, tmp) != 2);
    efl_assert_fail_if(strcmp(tmp, "-1") != 0);

    efl_assert_fail_if(eina_convert_itoa(100, tmp) != 3);
    efl_assert_fail_if(strcmp(tmp, "100") != 0);

    efl_assert_fail_if(eina_convert_itoa(-100, tmp) != 4);
    efl_assert_fail_if(strcmp(tmp, "-100") != 0);

    efl_assert_fail_if(eina_convert_itoa(10000000, tmp) != 8);
    efl_assert_fail_if(strcmp(tmp, "10000000") != 0);

    snprintf(ref, sizeof(ref), "%d", INT_MIN);
    efl_assert_fail_if(eina_convert_itoa(INT_MIN, tmp) != (int)strlen(ref));
    efl_assert_fail_if(strcmp(tmp, ref) != 0);

    efl_assert_fail_if(eina_convert_xtoa(0, tmp) != 1);
    efl_assert_fail_if(strcmp(tmp, "0") != 0);

    efl_assert_fail_if(eina_convert_xtoa(0xA1, tmp) != 2);
    efl_assert_fail_if(strcmp(tmp, "a1") != 0);

    efl_assert_fail_if(eina_convert_xtoa(0xFF00EF0E, tmp) != 8);
    efl_assert_fail_if(strcmp(tmp, "ff00ef0e") != 0);
}

#define EET_TEST_DOUBLE0 123.45689
#define EET_TEST_DOUBLE1 1.0
#define EET_TEST_DOUBLE2 0.25
#define EET_TEST_DOUBLE3 0.0001234
#define EET_TEST_DOUBLE4 123456789.9876543210

static void
_eina_convert_check(double test, int length)
{
    char          tmp[128];
    long long int m = 0;
    long          e = 0;
    double        r;

    efl_assert_fail_if(eina_convert_dtoa(test, tmp) != length);
    efl_assert_fail_if(eina_convert_atod(tmp, 128, &m, &e) != EFL_TRUE);
    r = ldexp((double)m, e);
    efl_assert_fail_if(fabs(r - test) > DBL_MIN);
}

TEST(convert_double)
{
    long long int m = 0;
    long          e = 0;

    _eina_convert_check(EET_TEST_DOUBLE0, 20);
    _eina_convert_check(-EET_TEST_DOUBLE0, 21);
    _eina_convert_check(EET_TEST_DOUBLE1, 6);
    _eina_convert_check(EET_TEST_DOUBLE2, 6);
    _eina_convert_check(EET_TEST_DOUBLE3, 21);
    _eina_convert_check(EET_TEST_DOUBLE4, 21);

    efl_assert_fail_if(eina_convert_atod("ah ah ah", 8, &m, &e) != EFL_FALSE);
    efl_assert_fail_if(eina_convert_atod("0xjo", 8, &m, &e) != EFL_FALSE);
    efl_assert_fail_if(eina_convert_atod("0xp", 8, &m, &e) != EFL_FALSE);
}

static void
_eina_convert_fp_check(double d, Eina_F32p32 fp, int length)
{
    char        tmp1[128];
    char        tmp2[128];
    Eina_F32p32 fpc;
    double      fpd;
    int         l1;
    int         l2;

    l1 = eina_convert_dtoa(d, tmp1);
    l2 = eina_convert_fptoa(fp, tmp2);
/*    fprintf(stderr, "[%s](%i) vs [%s](%i)\n", tmp1, l1, tmp2, l2); */
    efl_assert_fail_if(l1 != l2);
    efl_assert_fail_if(length != l1);
    efl_assert_fail_if(strcmp(tmp1, tmp2) != 0);

    efl_assert_fail_if(!eina_convert_atofp(tmp2, l2, &fpc));
/*    fprintf(stderr, "%016x vs %016x\n", fpc, fp); */
    efl_assert_fail_if(fpc != fp);

    efl_assert_fail_if(!eina_convert_atofp(tmp1, l1, &fpc));
    fpd = eina_f32p32_double_to(fpc);
/*    fprintf(stderr, "%0.16f vs %0.16f\n", fpd, d); */
    efl_assert_fail_if(fabs(fpd - d) > DBL_MIN);

    d  = -d;
    fp = -fp;

    l1 = eina_convert_dtoa(d, tmp1);
    l2 = eina_convert_fptoa(fp, tmp2);
    efl_assert_fail_if(l1 != l2);
    efl_assert_fail_if(length + 1 != l1);
    efl_assert_fail_if(strcmp(tmp1, tmp2) != 0);

    efl_assert_fail_if(!eina_convert_atofp(tmp2, l2, &fpc));
/*    fprintf(stderr, "%016x vs %016x\n", fpc, fp); */
    efl_assert_fail_if(fpc != fp);

    efl_assert_fail_if(!eina_convert_atofp(tmp1, l1, &fpc));
    fpd = eina_f32p32_double_to(fpc);
/*    fprintf(stderr, "%0.16f vs %0.16f\n", fpd, d); */
    efl_assert_fail_if(fabs(fpd - d) > DBL_MIN);
}

TEST(convert_fp)
{
    _eina_convert_fp_check(1.0, 0x0000000100000000, 6);
    _eina_convert_fp_check(0.5, 0x0000000080000000, 8);
    _eina_convert_fp_check(0.625, 0x00000000a0000000, 8);
    _eina_convert_fp_check(256.0, 0x0000010000000000, 6);
    _eina_convert_fp_check(0.5, 0x0000000080000000, 8);
    _eina_convert_fp_check(128.625, 0x00000080a0000000, 10);
}

static void
_eina_convert_strtod_c_check(const char *str)
{
    double d1;
    double d2;
    char  *e1;
    char  *e2;

    e1 = NULL;
    d1 = eina_convert_strtod_c(str, &e1);

    e2 = NULL;
    d2 = strtod(str, &e2);

    switch (fpclassify(d2))
    {
        case FP_NAN:
            efl_assert_fail_if(fpclassify(d1) != FP_NAN);
            break;
        case FP_INFINITE:
            efl_assert_fail_if(fpclassify(d1) != FP_INFINITE);
            break;
        default:
            efl_assert_fail_if((fpclassify(d1) != FP_ZERO) &&
                    (fpclassify(d1) != FP_SUBNORMAL) &&
                    (fpclassify(d1) != FP_NORMAL));
            if (!EINA_DBL_EQ(d1, d2) || (e1 != e2))
            {
                fprintf(stderr, "  FP_NORMAL\n");
                fprintf(stderr, "  ERR: %s, %s\n", str, strerror(errno));
                fprintf(stderr,
                        "    E1 **%.6f**, **%g**, %s\n",
                        d1,
                        d1,
                        e1 ? e1 : "");
                fprintf(stderr,
                        "    E2 **%.6f**, **%g**, %s\n",
                        d2,
                        d2,
                        e2 ? e2 : "");
                if (!EINA_DBL_EQ(d1, d2)) printf("different value\n");
                if (e1 != e2) printf("different end position\n");
            }

            efl_assert_fail_if(!EINA_DBL_EQ(d1, d2) || (e1 != e2));
            break;
    }
}

TEST(convert_strtod_c_simple)
{
    char *old;

    old = setlocale(LC_ALL, "C");
    _eina_convert_strtod_c_check("0");
    _eina_convert_strtod_c_check("-0");
    _eina_convert_strtod_c_check(".1");
    _eina_convert_strtod_c_check("  .");
    _eina_convert_strtod_c_check("  1.2e3");
    _eina_convert_strtod_c_check(" +1.2e3");
    _eina_convert_strtod_c_check("1.2e3");
    _eina_convert_strtod_c_check("+1.2e3");
    _eina_convert_strtod_c_check("+1.e3");
    _eina_convert_strtod_c_check("-1.2e3");
    _eina_convert_strtod_c_check("-1.2e3.5");
    _eina_convert_strtod_c_check("-1.2e");
    _eina_convert_strtod_c_check("--1.2e3.5");
    _eina_convert_strtod_c_check("--1-.2e3.5");
    _eina_convert_strtod_c_check("-a");
    _eina_convert_strtod_c_check("a");
    _eina_convert_strtod_c_check(".1e");
    _eina_convert_strtod_c_check(".1e0");
    _eina_convert_strtod_c_check(".1e3");
    _eina_convert_strtod_c_check(".1e-3");
    _eina_convert_strtod_c_check(".1e-");
    _eina_convert_strtod_c_check(" .e-");
    _eina_convert_strtod_c_check(" .e");
    _eina_convert_strtod_c_check(" e");
    _eina_convert_strtod_c_check(" e0");
    _eina_convert_strtod_c_check(" ee");
    _eina_convert_strtod_c_check(" -e");
    _eina_convert_strtod_c_check(" .9");
    _eina_convert_strtod_c_check(" ..9");
    _eina_convert_strtod_c_check("009");
    _eina_convert_strtod_c_check("0.09e02");
    /* http://thread.gmane.org/gmane.editors.vim.devel/19268/ */
    _eina_convert_strtod_c_check("0.9999999999999999999999999999999999");
    _eina_convert_strtod_c_check("2.2250738585072010e-308"); // BUG
    /* PHP (slashdot.jp): http://opensource.slashdot.jp/story/11/01/08/0527259/PHP%E3%81%AE%E6%B5%AE%E5%8B%95%E5%B0%8F%E6%95%B0%E7%82%B9%E5%87%A6%E7%90%86%E3%81%AB%E7%84%A1%E9%99%90%E3%83%AB%E3%83%BC%E3%83%97%E3%81%AE%E3%83%90%E3%82%B0 */
    _eina_convert_strtod_c_check("2.2250738585072011e-308");
    /* Gauche: http://blog.practical-scheme.net/gauche/20110203-bitten-by-floating-point-numbers-again */
    _eina_convert_strtod_c_check("2.2250738585072012e-308");
    _eina_convert_strtod_c_check("2.2250738585072013e-308");
    _eina_convert_strtod_c_check("2.2250738585072014e-308");
    _eina_convert_strtod_c_check(" NaNfoo");
    _eina_convert_strtod_c_check(" -INFfoo");
    // this test isn't viable because libc actually fails the conversion (testing
    // glibc 2.28 on arch linux). either libc doesn't like the space at the start
    // thus doesn't skip it but assumes END of numbver string thus not converting
    // and returning NULL, or it doesn't like InFiNiTyfoo in some way, but either
    // way this test shows eina to be more robust and do some kind of conversion
    // and libc to fail and return NULL from strtod into the string pointer. it
    // also doesnt return an infinite fp thus hitting the default: case and thus
    // failing etc. ... so all in all remove the test as all it does it cause
    // failures and if anything shows libc to be failing more than eina.
    //   _eina_convert_strtod_c_check("  InFiNiTyfoo");
    setlocale(LC_ALL, old);
}
