/*
 * Copyright (C) 2010 Gustavo Sverzut Barbieri
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(str_simple)
{
    size_t      i;
    char       *str, *ret;
    const char *escape_ret = "\\ a\\\\x\\'";

    efl_assert_fail_if(!eina_str_has_prefix("", ""));

    efl_assert_fail_if(!eina_str_has_prefix("x", "x"));
    efl_assert_fail_if(!eina_str_has_prefix("xab", "x"));
    efl_assert_fail_if(!eina_str_has_prefix("xab", "xab"));

    efl_assert_fail_if(eina_str_has_prefix("x", "xab"));
    efl_assert_fail_if(eina_str_has_prefix("xab", "xyz"));
    efl_assert_fail_if(eina_str_has_prefix("", "x"));
    efl_assert_fail_if(eina_str_has_prefix("X", "x"));
    efl_assert_fail_if(eina_str_has_prefix("xAb", "X"));
    efl_assert_fail_if(eina_str_has_prefix("xAb", "xab"));

    efl_assert_fail_if(!eina_str_has_suffix("", ""));

    efl_assert_fail_if(!eina_str_has_suffix("x", "x"));
    efl_assert_fail_if(!eina_str_has_suffix("abx", "x"));
    efl_assert_fail_if(!eina_str_has_suffix("xab", "xab"));

    efl_assert_fail_if(eina_str_has_suffix("x", "xab"));
    efl_assert_fail_if(eina_str_has_suffix("xab", "xyz"));
    efl_assert_fail_if(eina_str_has_suffix("", "x"));
    efl_assert_fail_if(eina_str_has_suffix("X", "x"));
    efl_assert_fail_if(eina_str_has_suffix("aBx", "X"));
    efl_assert_fail_if(eina_str_has_suffix("xaB", "Xab"));

    efl_assert_fail_if(!eina_str_has_extension("", ""));

    efl_assert_fail_if(!eina_str_has_extension("x", "x"));
    efl_assert_fail_if(!eina_str_has_extension("abx", "x"));
    efl_assert_fail_if(!eina_str_has_extension("xab", "xab"));
    efl_assert_fail_if(!eina_str_has_extension("x", "X"));
    efl_assert_fail_if(!eina_str_has_extension("abx", "X"));
    efl_assert_fail_if(!eina_str_has_extension("xab", "Xab"));
    efl_assert_fail_if(!eina_str_has_extension("X", "X"));
    efl_assert_fail_if(!eina_str_has_extension("aBx", "X"));
    efl_assert_fail_if(!eina_str_has_extension("xaB", "Xab"));

    efl_assert_fail_if(eina_str_has_extension("x", "xab"));
    efl_assert_fail_if(eina_str_has_extension("xab", "xyz"));
    efl_assert_fail_if(eina_str_has_extension("", "x"));
    efl_assert_fail_if(eina_str_has_extension("x", "xAb"));
    efl_assert_fail_if(eina_str_has_extension("xab", "xYz"));
    efl_assert_fail_if(eina_str_has_extension("", "x"));

    efl_assert_fail_if(eina_streq("xab", NULL));
    efl_assert_fail_if(eina_streq(NULL, "xab"));
    efl_assert_fail_if(eina_streq("x", "xab"));
    efl_assert_fail_if(eina_streq("xab", "XAB"));
    efl_assert_fail_if(eina_streq("x", "x "));
    efl_assert_fail_if(!eina_streq("xab", "xab"));

    efl_assert_fail_if(eina_strlen_bounded("abc", 4) != strlen("abc"));
    efl_assert_fail_if(eina_strlen_bounded("abc", 2) != (size_t)-1);

    str = malloc(sizeof(char) * 4);
    strcpy(str, "bSd");
    eina_str_tolower(&str);
    efl_assert_fail_if(strcmp(str, "bsd") != 0);
    eina_str_toupper(&str);
    efl_assert_fail_if(strcmp(str, "BSD") != 0);
    free(str);

    str = malloc(sizeof(char) * 8);
    strcpy(str, " a\\x'");
    ret = eina_str_escape(str);
    efl_assert_fail_if(strlen(ret) != strlen(escape_ret));
    for (i = 0; i <= strlen(ret); i++)
        efl_assert_fail_if(ret[i] != escape_ret[i]);
    free(str);
    free(ret);

    str = malloc(sizeof(char) * 4);
    strcpy(str, "a\t ");
    ret = eina_str_escape(str);
    efl_assert_fail_if(!eina_streq(ret, "a\\t\\ "));
    free(str);
    free(ret);
}

TEST(str_split)
{
    char       **result;
    unsigned int elements;
    char        *str;

    result = eina_str_split_full(NULL, ":", 1, &elements);
    efl_assert_fail_if(result != NULL);
    efl_assert_fail_if(elements != 0);

    result = eina_str_split_full("nomatch", NULL, 1, &elements);
    efl_assert_fail_if(result != NULL);
    efl_assert_fail_if(elements != 0);

    result = eina_str_split_full("match:match", ":", 1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 1);
    efl_assert_fail_if(strcmp(result[0], "match:match") != 0);
    free(result[0]);
    free(result);

    result =
        eina_str_split_full("match:match:nomatch:nomatch", ":", 3, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 3);
    efl_assert_fail_if(strcmp(result[0], "match") != 0);
    efl_assert_fail_if(strcmp(result[1], "match") != 0);
    efl_assert_fail_if(strcmp(result[2], "nomatch:nomatch") != 0);
    free(result[0]);
    free(result);

    result = eina_str_split_full("nomatch", "", -1, &elements);
    efl_assert_fail_if(result != NULL);
    efl_assert_fail_if(elements != 0);

    result = eina_str_split_full("nomatch", "x", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 1);
    efl_assert_fail_if(strcmp(result[0], "nomatch") != 0);
    efl_assert_fail_if(result[1]);
    free(result[0]);
    free(result);

    result = eina_str_split_full("nomatch", "xyz", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 1);
    efl_assert_fail_if(strcmp(result[0], "nomatch") != 0);
    efl_assert_fail_if(result[1]);
    free(result[0]);
    free(result);

    result = eina_str_split_full("match:match:match", ":", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 3);
    for (elements = 0; elements < 3 - 1; elements++)
        efl_assert_fail_if(strcmp(result[elements], "match") != 0);
    efl_assert_fail_if(result[3]);
    free(result[0]);
    free(result);

    str    = malloc(sizeof(char) * 1024);
    str[0] = 0;
    for (elements = 0; elements < 300; elements++)
        strcat(str, "ma:");
    strcat(str, "ma");
    result = eina_str_split_full(str, ":", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 301);
    free(result[0]);
    free(result);
    free(str);

    result = eina_str_split_full("a:b:c", ":", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 3);
    efl_assert_fail_if(strcmp(result[0], "a") != 0);
    efl_assert_fail_if(strcmp(result[1], "b") != 0);
    efl_assert_fail_if(strcmp(result[2], "c") != 0);
    efl_assert_fail_if(result[3]);
    free(result[0]);
    free(result);

    result = eina_str_split_full("a:b:", ":", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 3);
    efl_assert_fail_if(strcmp(result[0], "a") != 0);
    efl_assert_fail_if(strcmp(result[1], "b") != 0);
    efl_assert_fail_if(strcmp(result[2], "") != 0);
    efl_assert_fail_if(result[3]);
    free(result[0]);
    free(result);

    result = eina_str_split_full(":b:c", ":", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 3);
    efl_assert_fail_if(strcmp(result[0], "") != 0);
    efl_assert_fail_if(strcmp(result[1], "b") != 0);
    efl_assert_fail_if(strcmp(result[2], "c") != 0);
    efl_assert_fail_if(result[3]);
    free(result[0]);
    free(result);

    result = eina_str_split_full(":", ":", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 2);
    efl_assert_fail_if(strcmp(result[0], "") != 0);
    efl_assert_fail_if(strcmp(result[1], "") != 0);
    efl_assert_fail_if(result[2]);
    free(result[0]);
    free(result);

    result = eina_str_split_full("a", "!!!!!!!!!", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 1);
    efl_assert_fail_if(strcmp(result[0], "a") != 0);
    efl_assert_fail_if(result[1]);
    free(result[0]);
    free(result);

    result = eina_str_split_full("aaba", "ab", -1, &elements);
    efl_assert_fail_if(result == NULL);
    efl_assert_fail_if(elements != 2);
    efl_assert_fail_if(strcmp(result[0], "a") != 0);
    efl_assert_fail_if(strcmp(result[1], "a") != 0);
    efl_assert_fail_if(result[2]);
    free(result[0]);
    free(result);
}

TEST(str_lcat_lcpy)
{
    char   dst[9];
    size_t ret;
    size_t ds = (sizeof(dst) / sizeof(char));

    dst[0] = '\0';

    ret = eina_strlcat(dst, "cat1", ds);
    efl_assert_fail_if(ret != 4);
    efl_assert_fail_if(strcmp(dst, "cat1") != 0);

    ret = eina_strlcat(dst, NULL, ds);
    efl_assert_fail_if(ret != 4);
    efl_assert_fail_if(strcmp(dst, "cat1") != 0);

    ret = eina_strlcat(dst, "cat234", ds);
    efl_assert_fail_if(ret != (ds - 1 + 2));
    efl_assert_fail_if(strcmp(dst, "cat1cat2") != 0);

    ret = eina_strlcat(dst, "cat3", ds);
    efl_assert_fail_if(ret != (ds - 1 + 4));
    efl_assert_fail_if(strcmp(dst, "cat1cat2") != 0);

    ret = eina_strlcat(dst, "cat3", ds - 1);
    efl_assert_fail_if(ret != (ds - 1 + 4));
    efl_assert_fail_if(strcmp(dst, "cat1cat2") != 0);

    ret = eina_strlcpy(dst, "copycopy", ds);
    efl_assert_fail_if(ret != 8);
    efl_assert_fail_if(strcmp(dst, "copycopy") != 0);

    ret = eina_strlcpy(dst, "copy2copy2", ds);
    efl_assert_fail_if(ret != 10);
    efl_assert_fail_if(strcmp(dst, "copy2cop") != 0);
}

TEST(str_join_len)
{
    char   dst[9];
    size_t ret;
    size_t ds = (sizeof(dst) / sizeof(char));

    dst[0] = '\0';

    ret = eina_str_join_len(dst, ds, '#', "ab", 2, "cde", 3);
    efl_assert_fail_if(ret != 6);
    efl_assert_fail_if(strcmp(dst, "ab#cde") != 0);

    ret = eina_str_join_len(dst, ds, '#', "abcdefghi", 9, "cde", 3);
    efl_assert_fail_if(ret != 13);
    efl_assert_fail_if(strcmp(dst, "abcdefgh") != 0);

    ret = eina_str_join_len(dst, ds, '#', "abcdefgh", 8, "cde", 3);
    efl_assert_fail_if(ret != 12);
    efl_assert_fail_if(strcmp(dst, "abcdefgh") != 0);

    ret = eina_str_join_len(dst, ds, '#', "abcd", 4, "efgh", 4);
    efl_assert_fail_if(ret != 9);
    efl_assert_fail_if(strcmp(dst, "abcd#efg") != 0);
}

TEST(str_memdup)
{
    struct temp
    {
        int    i;
        char  *s;
        double d;
    } t1, *t2;

    unsigned char  buf[7];
    unsigned char *temp_buf;

    t1.i = 1234;
    t1.s = "hello";
    t1.d = 123.456;

    t2 = (struct temp *)eina_memdup((unsigned char *)&t1,
                                    sizeof(struct temp),
                                    EFL_TRUE);
    efl_assert_fail_if(t2->i != t1.i);
    efl_assert_fail_if(strcmp(t2->s, t1.s) != 0);
    efl_assert_fail_if(!EINA_FLT_EQ(t2->d, t1.d));
    free(t2);

    memcpy(buf, "aaabbb", 6);
    temp_buf = eina_memdup(buf, 6, EFL_TRUE);
    efl_assert_fail_if(strcmp((char *)temp_buf, "aaabbb") != 0);
    free(temp_buf);
}

TEST(str_strftime)
{
    time_t     curr_time;
    struct tm *info;
    char      *buf;

    curr_time = time(NULL);
    info      = localtime(&curr_time);

    buf = eina_strftime("%I:%M%p", info);
    efl_assert_fail_if(buf == NULL);
    free(buf);
}

#ifdef HAVE_ICONV
TEST(str_convert)
{
    char *utf8  = "\xc3\xa9\xc3\xa1\xc3\xba\xc3\xad\xc3\xa8\xc3\xa0\xc3\xb9\xc3"
                  "\xac\xc3\xab\xc3\xa4\xc3\xbc\xc3\xaf";
    char *utf16 = "\xe9\x0\xe1\x0\xfa\x0\xed\x0\xe8\x0\xe0\x0\xf9\x0\xec\x0\xeb"
                  "\x0\xe4\x0\xfc\x0\xef\x0";
    char *ret;
    size_t ret_sz;
    int    i;

    ret = eina_str_convert("UTF-8", "UTF-16LE", utf8);
    efl_assert_fail_if(ret == NULL);
    for (i = 0; i < 24; i++)
        efl_assert_fail_if(ret[i] != utf16[i]);

    free(ret);

    ret = eina_str_convert_len("UTF-8", "UTF-16LE", utf8, 24, &ret_sz);
    efl_assert_fail_if(ret == NULL);
    efl_assert_fail_if(ret_sz != 24);
    for (i = 0; i < 24; i++)
        efl_assert_fail_if(ret[i] != utf16[i]);
    free(ret);
}
#endif
