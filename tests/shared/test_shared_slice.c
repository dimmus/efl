/*
 * Copyright (C) 2016 ProFUSION embedded systems
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(test_slice_init)
{
    Eina_Rw_Slice a         = EINA_SLICE_STR_LITERAL("hello world");
    char          buf[1024] = "";
    Eina_Rw_Slice b         = EINA_SLICE_ARRAY(buf);
    Eina_Rw_Slice c         = EINA_SLICE_STR("hi there");
    EINA_RW_SLICE_DECLARE(d, 512);

    efl_assert_true(a.len == sizeof("hello world") - 1);
    efl_assert_true(strcmp(a.mem, "hello world") == 0);

    efl_assert_true(b.len == sizeof(buf));

    efl_assert_true(c.len == strlen("hi there"));
    efl_assert_true(strcmp(c.mem, "hi there") == 0);

    efl_assert_true(d.len == 512);
}

TEST(test_slice_ro)
{
    Eina_Slice    slice = EINA_SLICE_STR_LITERAL("hi there");
    Eina_Slice    a, needle;
    Eina_Rw_Slice dup;
    Efl_Bool      r;
    const void   *p;
    char         *str;

    dup = eina_slice_dup(slice);
    efl_assert_true(dup.len == slice.len);
    efl_assert_true(dup.mem != slice.mem);
    efl_assert_true(eina_slice_compare(eina_rw_slice_slice_get(dup), slice) ==
                    0);
    free(dup.mem);

    str = eina_slice_strdup(slice);
    efl_assert_true(str != NULL);
    efl_assert_true(strcmp(str, "hi there") == 0);
    free(str);

    a = eina_slice_seek(slice, strlen("hi "), SEEK_SET);
    efl_assert_true(a.mem == slice.bytes + strlen("hi "));
    efl_assert_true(a.len == slice.len - strlen("hi "));

    a = eina_slice_seek(slice, 0, SEEK_SET);
    efl_assert_true(a.mem == slice.bytes + 0);
    efl_assert_true(a.len == slice.len);

    a = eina_slice_seek(slice, -1, SEEK_END);
    efl_assert_true(a.mem == slice.bytes + slice.len - 1);
    efl_assert_true(a.len == 1);

    a = eina_slice_seek(slice, 0, SEEK_END);
    efl_assert_true(a.mem == eina_slice_end_get(slice));
    efl_assert_true(a.len == 0);

    p = eina_slice_end_get(slice);
    efl_assert_true(p == slice.bytes + slice.len);

    slice     = (Eina_Slice)EINA_SLICE_STR_LITERAL("HELLO WORLD");
    slice.len = strlen("hi there"); /* crop... */

    p = eina_slice_strchr(slice, ' ');
    efl_assert_true(p ==
                    slice.bytes + 5); /* 5 = index of ' ' in HELLO WORLD.. */

    p = eina_slice_strchr(slice, '!');
    efl_assert_true(p == NULL);

    needle = (Eina_Slice)EINA_SLICE_STR_LITERAL(" W");
    p      = eina_slice_find(slice, needle);
    efl_assert_true(p ==
                    slice.bytes + 5); /* 5 = index of ' W' in HELLO WORLD..*/

    needle = (Eina_Slice)EINA_SLICE_STR_LITERAL("LO");
    p      = eina_slice_find(slice, needle);
    efl_assert_true(p ==
                    slice.bytes + 3); /* 3 = index of 'LO' in HELLO WORLD..*/

    needle = (Eina_Slice)EINA_SLICE_STR_LITERAL("HELLO");
    p      = eina_slice_find(slice, needle);
    efl_assert_true(p == slice.mem);

    needle = (Eina_Slice)EINA_SLICE_STR_LITERAL(
        "WORLD"); /* would go out of boundaries */
    p = eina_slice_find(slice, needle);
    efl_assert_true(p == NULL);

    r = eina_slice_startswith(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("HEL"));
    efl_assert_true(r == EFL_TRUE);

    r = eina_slice_startswith(slice,
                              (Eina_Slice)EINA_SLICE_STR_LITERAL("WORLD"));
    efl_assert_true(r == EFL_FALSE);

    r = eina_slice_startswith(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL(""));
    efl_assert_true(r == EFL_FALSE);

    r = eina_slice_endswith(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("WO"));
    efl_assert_true(r == EFL_TRUE);

    r = eina_slice_endswith(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("WORLD"));
    efl_assert_true(r == EFL_FALSE);

    r = eina_slice_endswith(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL(""));
    efl_assert_true(r == EFL_FALSE);
}

TEST(test_slice_rw)
{
    uint8_t       buf[]    = "hi there";
    Eina_Rw_Slice rw_slice = EINA_SLICE_ARRAY(buf);
    Eina_Slice    ro_slice;
    Eina_Rw_Slice a;
    Efl_Bool      r;
    const void   *p;
    char         *str;

    buf[sizeof(buf) - 1] = 0xff;
    rw_slice.len--; /* do not account \0 (now 0xff) */
    ro_slice = eina_rw_slice_slice_get(rw_slice);

    efl_assert_true(rw_slice.len == ro_slice.len);
    efl_assert_true(rw_slice.mem == ro_slice.mem);

    a = eina_slice_dup(ro_slice);
    efl_assert_true(a.len == ro_slice.len);
    efl_assert_true(a.mem != ro_slice.mem);
    efl_assert_true(eina_rw_slice_compare(a, rw_slice) == 0);
    free(a.mem);

    str = eina_rw_slice_strdup(rw_slice);
    efl_assert_true(str != NULL);
    efl_assert_true(strcmp(str, "hi there") == 0);
    free(str);

    a = eina_rw_slice_seek(rw_slice, strlen("hi "), SEEK_SET);
    efl_assert_true(a.mem == rw_slice.bytes + strlen("hi "));
    efl_assert_true(a.len == rw_slice.len - strlen("hi "));

    a = eina_rw_slice_seek(rw_slice, 0, SEEK_SET);
    efl_assert_true(a.mem == rw_slice.bytes + 0);
    efl_assert_true(a.len == rw_slice.len);

    a = eina_rw_slice_seek(rw_slice, -1, SEEK_END);
    efl_assert_true(a.mem == rw_slice.bytes + rw_slice.len - 1);
    efl_assert_true(a.len == 1);

    a = eina_rw_slice_seek(rw_slice, 0, SEEK_END);
    efl_assert_true(a.mem == eina_rw_slice_end_get(rw_slice));
    efl_assert_true(a.len == 0);

    p = eina_rw_slice_end_get(rw_slice);
    efl_assert_true(p == rw_slice.bytes + rw_slice.len);

    ro_slice = (Eina_Slice)EINA_SLICE_STR_LITERAL(
        "HELLO WORLD, big string to be cropped");
    a = eina_rw_slice_copy(rw_slice, ro_slice);

    efl_assert_true(a.mem == rw_slice.mem);
    efl_assert_true(a.len == rw_slice.len);
    efl_assert_true(strncmp(a.mem, "HELLO WO", a.len) == 0);

    p = eina_rw_slice_strchr(rw_slice, ' ');
    efl_assert_true(p ==
                    rw_slice.bytes + 5); /* 5 = index of ' ' in HELLO WORLD.. */

    p = eina_rw_slice_strchr(rw_slice, '!');
    efl_assert_true(p == NULL);

    ro_slice = (Eina_Slice)EINA_SLICE_STR_LITERAL(" W");
    p        = eina_rw_slice_find(rw_slice, ro_slice);
    efl_assert_true(p ==
                    rw_slice.bytes + 5); /* 5 = index of ' W' in HELLO WORLD..*/

    ro_slice = (Eina_Slice)EINA_SLICE_STR_LITERAL("LO");
    p        = eina_rw_slice_find(rw_slice, ro_slice);
    efl_assert_true(p ==
                    rw_slice.bytes + 3); /* 3 = index of 'LO' in HELLO WORLD..*/

    ro_slice = (Eina_Slice)EINA_SLICE_STR_LITERAL("HELLO");
    p        = eina_rw_slice_find(rw_slice, ro_slice);
    efl_assert_true(p == rw_slice.mem);

    ro_slice = (Eina_Slice)EINA_SLICE_STR_LITERAL(
        "WORLD"); /* would go out of boundaries */
    p = eina_rw_slice_find(rw_slice, ro_slice);
    efl_assert_true(p == NULL);

    efl_assert_true(buf[sizeof(buf) - 1] == 0xff);

    r = eina_rw_slice_startswith(rw_slice,
                                 (Eina_Slice)EINA_SLICE_STR_LITERAL("HEL"));
    efl_assert_true(r == EFL_TRUE);

    r = eina_rw_slice_startswith(rw_slice,
                                 (Eina_Slice)EINA_SLICE_STR_LITERAL("WORLD"));
    efl_assert_true(r == EFL_FALSE);

    r = eina_rw_slice_startswith(rw_slice,
                                 (Eina_Slice)EINA_SLICE_STR_LITERAL(""));
    efl_assert_true(r == EFL_FALSE);

    r = eina_rw_slice_endswith(rw_slice,
                               (Eina_Slice)EINA_SLICE_STR_LITERAL("WO"));
    efl_assert_true(r == EFL_TRUE);

    r = eina_rw_slice_endswith(rw_slice,
                               (Eina_Slice)EINA_SLICE_STR_LITERAL("WORLD"));
    efl_assert_true(r == EFL_FALSE);

    r = eina_rw_slice_endswith(rw_slice,
                               (Eina_Slice)EINA_SLICE_STR_LITERAL(""));
    efl_assert_true(r == EFL_FALSE);
}

TEST(test_slice_print)
{
    char       buf[] = "Hello World";
    Eina_Slice slice = { .len = strlen("Hello"), .mem = buf };
    char       a[128], b[128];

    snprintf(a, sizeof(a), EINA_SLICE_FMT, EINA_SLICE_PRINT(slice));
    snprintf(b, sizeof(b), "%p+%zu", slice.mem, slice.len);
    efl_assert_true(strcmp(a, b) == 0);

    snprintf(a, sizeof(a), EINA_SLICE_STR_FMT, EINA_SLICE_STR_PRINT(slice));
    efl_assert_true(strcmp(a, "Hello") == 0);
}

TEST(test_slice_find)
{
    Eina_Slice  slice = EINA_SLICE_STR_LITERAL("abcdef");
    const char *p;

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("a"));
    efl_assert_fail_if(p == NULL);
    efl_assert_str_eq(p, (const char *)slice.bytes);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("ab"));
    efl_assert_fail_if(p == NULL);
    efl_assert_str_eq(p, (const char *)slice.bytes);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("abc"));
    efl_assert_fail_if(p == NULL);
    efl_assert_str_eq(p, (const char *)slice.bytes);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("f"));
    efl_assert_fail_if(p == NULL);
    efl_assert_str_eq(p, (const char *)slice.bytes + strlen("abcde"));

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("ef"));
    efl_assert_fail_if(p == NULL);
    efl_assert_str_eq(p, (const char *)slice.bytes + strlen("abcd"));

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("def"));
    efl_assert_fail_if(p == NULL);
    efl_assert_str_eq(p, (const char *)slice.bytes + strlen("abc"));

    p = eina_slice_find(slice, slice);
    efl_assert_fail_if(p == NULL);
    efl_assert_str_eq(p, (const char *)slice.bytes);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("x"));
    efl_assert_true(p == NULL);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("xyz"));
    efl_assert_true(p == NULL);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("xa"));
    efl_assert_true(p == NULL);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("xb"));
    efl_assert_true(p == NULL);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL("abcdefgh"));
    efl_assert_true(p == NULL);

    p = eina_slice_find(slice, (Eina_Slice)EINA_SLICE_STR_LITERAL(""));
    efl_assert_true(p == NULL);
}
