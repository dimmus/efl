/*
 * Copyright (C) 2008 Cedric Bail
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Efl_Shared.h>
#include "eina_hash.h"

#include "efl-test-runner.h"

#define EINA_HASH_BUCKET_SIZE 8

static unsigned int
_eina_string_key_length(const char *key)
{
    if (!key) return 0;

    return (int)strlen(key) + 1;
}

static int
_eina_string_key_cmp(const char *key1,
                     int         key1_length,
                     const char *key2,
                     int         key2_length)
{
    int delta;

    delta = key1_length - key2_length;
    if (delta) return delta;
    return strcmp(key1, key2);
}

static Efl_Bool
eina_foreach_check(EFL_UNUSED const Eina_Hash *hash,
                   const void                 *key,
                   void                       *data,
                   EFL_UNUSED void            *fdata)
{
    int *j = data;
    int  i;

    if (strlen(key) <= 0) return EFL_TRUE;

    i = atoi(key);
    efl_assert_fail_if(i != *j);

    return EFL_TRUE;
}

TEST(test_hash_simple)
{
    Eina_Hash *hash = NULL;
    int       *test;
    int        array[] = { 1, 42, 4, 5, 6 };

    /* As mempool is already initialized and it use hash, we should have 2 init. */
    hash = eina_hash_string_superfast_new(NULL);
    efl_assert_fail_if(hash == NULL);

    efl_assert_fail_if(eina_hash_add(hash, "1", &array[0]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "42", &array[1]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_direct_add(hash, "4", &array[2]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_direct_add(hash, "5", &array[3]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "", "") != EFL_TRUE);

    test = eina_hash_find(hash, "4");
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 4);

    test = eina_hash_find(hash, "42");
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 42);

    eina_hash_foreach(hash, eina_foreach_check, NULL);

    test = eina_hash_modify(hash, "5", &array[4]);
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 5);

    test = eina_hash_find(hash, "5");
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 6);

    efl_assert_fail_if(eina_hash_population(hash) != 5);

    efl_assert_fail_if(eina_hash_find(hash, "120") != NULL);

    efl_assert_fail_if(eina_hash_del(hash, "5", NULL) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_find(hash, "5") != NULL);

    efl_assert_fail_if(eina_hash_del(hash, NULL, &array[2]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_find(hash, "4") != NULL);

    efl_assert_fail_if(eina_hash_del(hash, NULL, &array[2]) != EFL_FALSE);

    efl_assert_fail_if(eina_hash_del(hash, "1", NULL) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_del(hash, "42", NULL) != EFL_TRUE);

    eina_hash_free(hash);
}

TEST(test_hash_crc)
{
    Eina_Hash *hash = NULL;
    int       *test;
    int        array[] = { 1, 42, 4, 5, 6 };

    hash = eina_hash_new(EINA_KEY_LENGTH(_eina_string_key_length),
                         EINA_KEY_CMP(_eina_string_key_cmp),
                         EINA_KEY_HASH(eina_hash_crc),
                         NULL,
                         EINA_HASH_BUCKET_SIZE);
    efl_assert_fail_if(hash == NULL);

    efl_assert_fail_if(eina_hash_add(hash, "1", &array[0]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "42", &array[1]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_direct_add(hash, "4", &array[2]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_direct_add(hash, "5", &array[3]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "", "") != EFL_TRUE);

    test = eina_hash_find(hash, "4");
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 4);

    test = eina_hash_find(hash, "42");
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 42);

    eina_hash_foreach(hash, eina_foreach_check, NULL);

    test = eina_hash_modify(hash, "5", &array[4]);
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 5);

    test = eina_hash_find(hash, "5");
    efl_assert_fail_if(!test);
    efl_assert_fail_if(*test != 6);

    efl_assert_fail_if(eina_hash_population(hash) != 5);

    efl_assert_fail_if(eina_hash_find(hash, "120") != NULL);

    efl_assert_fail_if(eina_hash_del(hash, "5", NULL) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_find(hash, "5") != NULL);

    efl_assert_fail_if(eina_hash_del(hash, NULL, &array[2]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_find(hash, "4") != NULL);

    efl_assert_fail_if(eina_hash_del(hash, NULL, &array[2]) != EFL_FALSE);

    efl_assert_fail_if(eina_hash_del(hash, "1", NULL) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_del(hash, "42", NULL) != EFL_TRUE);

    eina_hash_free(hash);
}

TEST(test_hash_extended)
{
    Eina_Hash   *hash = NULL;
    unsigned int i;
    unsigned int num_loops = 3011;
    char        *array;

    hash = eina_hash_string_djb2_new(NULL);
    efl_assert_fail_if(hash == NULL);

    efl_assert_fail_if(eina_hash_direct_add(hash, "42", "42") != EFL_TRUE);

    array = malloc(num_loops * 10);
    efl_assert_ptr_ne(array, NULL);

    for (i = 0; i < num_loops; ++i)
    {
        char *tmp = array + (i * 10);
        eina_convert_itoa(i + 42, tmp);
        efl_assert_fail_if(eina_hash_direct_add(hash, tmp, tmp) != EFL_TRUE);
    }

    efl_assert_fail_if(eina_hash_find(hash, "42") == NULL);

    eina_hash_free(hash);
    free(array);
}

TEST(test_hash_double_item)
{
    Eina_Hash *hash = NULL;
    int        i[]  = { 7, 7 };
    int       *test;

    hash = eina_hash_string_superfast_new(NULL);
    efl_assert_fail_if(hash == NULL);

    efl_assert_fail_if(eina_hash_add(hash, "7", &i[0]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "7", &i[1]) != EFL_TRUE);

    efl_assert_fail_if(eina_hash_del(hash, "7", &i[1]) != EFL_TRUE);
    test = eina_hash_find(hash, "7");
    efl_assert_fail_if(test != &i[0]);

    eina_hash_free(hash);
}

TEST(test_hash_all_int)
{
    Eina_Hash *hash;
    int64_t    j[] = { 4321312301243122, 6, 7, 128 };
    int        i[] = { 42, 6, 7, 0 };
    int64_t   *test2;
    int       *test;
    int        it;

    hash = eina_hash_int32_new(NULL);
    efl_assert_fail_if(hash == NULL);

    for (it = 0; it < 4; ++it)
        efl_assert_fail_if(eina_hash_add(hash, &i[it], &i[it]) != EFL_TRUE);

    efl_assert_fail_if(eina_hash_del(hash, &i[1], &i[1]) != EFL_TRUE);
    test = eina_hash_find(hash, &i[2]);
    efl_assert_fail_if(test != &i[2]);

    test = eina_hash_find(hash, &i[3]);
    efl_assert_fail_if(test != &i[3]);

    eina_hash_free(hash);

    hash = eina_hash_int64_new(NULL);
    efl_assert_fail_if(hash == NULL);

    for (it = 0; it < 4; ++it)
        efl_assert_fail_if(eina_hash_add(hash, &j[it], &j[it]) != EFL_TRUE);

    efl_assert_fail_if(eina_hash_del(hash, &j[1], &j[1]) != EFL_TRUE);
    test2 = eina_hash_find(hash, &j[0]);
    efl_assert_fail_if(test2 != &j[0]);

    eina_hash_free(hash);
}

TEST(test_hash_int32_fuzze)
{
    Eina_Hash    *hash;
    unsigned int *r, *array;
    unsigned int  i;
    unsigned int  num_loops = 10000;

    hash = eina_hash_int32_new(NULL);

    array = malloc(sizeof(int) * num_loops);
    for (i = 0; i < num_loops; ++i)
    {
        r  = array + i;
        *r = rand() % 10000;
        efl_assert_int_ne(eina_hash_direct_add(hash, r, r), 0);
    }

    for (i = 0; i < num_loops / 2; ++i)
    {
        unsigned int tr = rand() + 10000;
        r               = eina_hash_find(hash, &tr);
        efl_assert_ptr_eq(r, NULL);
    }

    for (i = 0; i < num_loops / 2; ++i)
    {
        unsigned int tr = (rand() % 10000) - (10000 * 2);
        r               = eina_hash_find(hash, &tr);
        efl_assert_ptr_eq(r, NULL);
    }

    eina_hash_free(hash);
    free(array);
}

TEST(test_hash_int64_fuzze)
{
    Eina_Hash *hash;
    uint64_t  *r, *array;
    uint64_t   i;
    uint64_t   num_loops = 10000;

    hash = eina_hash_int64_new(NULL);

    array = malloc(sizeof(int64_t) * num_loops);
    for (i = 0; i < num_loops; ++i)
    {
        r  = array + i;
        *r = rand() % 10000;
        efl_assert_int_ne(eina_hash_direct_add(hash, r, r), 0);
    }

    for (i = 0; i < num_loops / 2; ++i)
    {
        uint64_t tr = rand() + 10000;
        r           = eina_hash_find(hash, &tr);
        efl_assert_ptr_eq(r, NULL);
    }

    for (i = 0; i < num_loops / 2; ++i)
    {
        uint64_t tr = (rand() % 10000) - (10000 * 2);
        r           = eina_hash_find(hash, &tr);
        efl_assert_ptr_eq(r, NULL);
    }

    eina_hash_free(hash);
    free(array);
}

TEST(test_hash_string_fuzze)
{
    Eina_Hash   *hash;
    unsigned int i;
    char        *array;
    unsigned int num_loops = 100;

    hash = eina_hash_string_superfast_new(NULL);

    array = malloc(num_loops * 10);
    efl_assert_ptr_ne(array, NULL);

    for (i = 0; i < num_loops; ++i)
    {
        char *tmp = array + (i * 10);
        eina_convert_itoa(i, tmp);
        efl_assert_fail_if(eina_hash_direct_add(hash, tmp, tmp) != EFL_TRUE);
    }

    for (i = 0; i < num_loops; ++i)
    {
        char  convert[128];
        char *r;

        eina_convert_itoa(rand() + num_loops, convert);
        r = eina_hash_find(hash, convert);
        efl_assert_ptr_eq(r, NULL);
    }

    eina_hash_free(hash);
    free(array);
}

TEST(test_hash_seed)
{
    efl_assert_fail_if(eina_seed == 0);
}

TEST(test_hash_add_del_by_hash)
{
    Eina_Hash *hash    = NULL;
    int        array[] = { 1, 42, 4, 5, 6 };
    int        key_len, key_hash;

    hash = eina_hash_new(EINA_KEY_LENGTH(_eina_string_key_length),
                         EINA_KEY_CMP(_eina_string_key_cmp),
                         EINA_KEY_HASH(eina_hash_crc),
                         NULL,
                         EINA_HASH_BUCKET_SIZE);
    efl_assert_fail_if(hash == NULL);
    efl_assert_fail_if(eina_hash_add(hash, "1", &array[0]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "42", &array[1]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "5", &array[3]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "6", &array[4]) != EFL_TRUE);

    key_len  = _eina_string_key_length("4");
    key_hash = eina_hash_crc("4", key_len);

    efl_assert_fail_if(eina_hash_add_by_hash(hash, "4", key_len, key_hash, &array[2]) !=
            EFL_TRUE);
    efl_assert_fail_if(eina_hash_del_by_hash(hash, "4", key_len, key_hash, &array[2]) !=
            EFL_TRUE);
    efl_assert_fail_if(eina_hash_del_by_hash(hash, "4", key_len, key_hash, &array[2]) !=
            EFL_FALSE);

    key_len  = _eina_string_key_length("42");
    key_hash = eina_hash_crc("42", key_len);
    efl_assert_fail_if(eina_hash_del_by_hash(hash, "42", key_len, key_hash, &array[1]) !=
            EFL_TRUE);

    efl_assert_fail_if(eina_hash_population(hash) != 3);
    eina_hash_free(hash);
}