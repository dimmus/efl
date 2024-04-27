/*
 * Copyright (C) 2008 Cedric Bail
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

#define TEST0      "te\0st/0"
#define TEST0_SIZE 7
#define TEST1      "te\0st/11"
#define TEST1_SIZE 8

TEST(binshare_simple)
{
    const char *t0;
    const char *t1;

    t0 = eina_binshare_add_length(TEST0, TEST0_SIZE);
    t1 = eina_binshare_add_length(TEST1, TEST1_SIZE);

    efl_assert_fail_if(t0 == NULL);
    efl_assert_fail_if(t1 == NULL);
    efl_assert_fail_if(memcmp(t0, TEST0, TEST0_SIZE) != 0);
    efl_assert_fail_if(memcmp(t1, TEST1, TEST1_SIZE) != 0);

    t0 = eina_binshare_ref(t0);
    efl_assert_fail_if(t0 == NULL);
    efl_assert_fail_if(memcmp(t0, TEST0, TEST0_SIZE) != 0);

    eina_binshare_del(t0);
    eina_binshare_del(t0);
    eina_binshare_del(t1);
}

TEST(binshare_small)
{
    char buf[4];
    int  i;

    for (i = 1; i < 3; i++)
    {
        const char *t0, *t1;
        int         j;

        for (j = 0; j < i; j++)
        {
            char c;
            for (c = 'a'; c <= 'z'; c++)
                buf[j] = c;
        }
        buf[i] = '\0';
        t0     = eina_binshare_add_length(buf, i);
        t1     = eina_binshare_add_length(buf, i);

        efl_assert_fail_if(t0 == NULL);
        efl_assert_fail_if(t1 == NULL);
        efl_assert_fail_if(t0 != t1);
        efl_assert_fail_if(memcmp(t0, buf, i) != 0);

        eina_binshare_del(t0);
        eina_binshare_del(t1);
    }
}

TEST(binshare_test_share)
{
    const char *t0;
    const char *t1;

    t0 = eina_binshare_add_length(TEST0, TEST0_SIZE);
    t1 = eina_binshare_add_length(TEST0, TEST0_SIZE);

    efl_assert_fail_if(t0 == NULL);
    efl_assert_fail_if(t1 == NULL);
    efl_assert_fail_if(memcmp(t0, TEST0, TEST0_SIZE) != 0);
    efl_assert_fail_if(memcmp(t1, TEST0, TEST0_SIZE) != 0);
    efl_assert_fail_if(t0 != t1);
    efl_assert_fail_if(TEST0_SIZE != eina_binshare_length(t0));

    eina_binshare_del(t0);
    eina_binshare_del(t1);
}

TEST(binshare_putstuff)
{
    const char *tmp;
    int         i;

    for (i = 100; i > 0; --i)
    {
        char         build[64] = "string_";
        unsigned int len;

        eina_convert_xtoa(i, build + 7);
        len = strlen(build);
        tmp = eina_binshare_add_length(build, len);
        efl_assert_ptr_ne(tmp, NULL);
        efl_assert_ptr_eq(tmp, eina_binshare_add_length(build, len));
        efl_assert_int_eq(len, eina_binshare_length(tmp));
        eina_binshare_del(tmp);
        eina_binshare_del(tmp);
    }
}

TEST(binshare_collision)
{
    Eina_Array *ea;
    char        buffer[50];
    int         i;
    const void *r;

    ea = eina_array_new(256);

    for (i = 0; i < 256; ++i)
    {
        unsigned int len;
        eina_convert_itoa(60000 - i, buffer);
        len = strlen(buffer);
        r   = eina_binshare_add_length(buffer, len);
        efl_assert_ptr_ne(r, NULL);
        eina_array_push(ea, r);
        r = eina_binshare_add_length(buffer, len);
        efl_assert_ptr_ne(r, NULL);
        r = eina_binshare_add_length(buffer, len);
        efl_assert_ptr_ne(r, NULL);
    }

    while (eina_array_count(ea))
    {
        r = eina_array_pop(ea);
        eina_binshare_del(r);
        eina_binshare_del(r);
        eina_binshare_del(r);
    }

    eina_array_free(ea);
}
