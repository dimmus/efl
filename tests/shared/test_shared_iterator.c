/*
 * Copyright (C) 2008 Cedric Bail
 */
#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

struct Number
{
    int number;
};

static Efl_Bool
_filter(const void *it EFL_UNUSED, void *data, void *fdata)
{
    struct Number *numb = data;

    efl_assert_ptr_eq(fdata, (void *)1);
    return numb->number % 2;
}

TEST(iterator_filter_simple)
{
    Eina_Iterator *it, *filtered;
    Eina_Array    *ea;

    ea = eina_array_new(11);
    efl_assert_fail_if(!ea);

    for (int i = 0; i <= 10; i++)
    {
        struct Number *number;

        number         = calloc(1, sizeof(struct Number));
        number->number = i;

        eina_array_push(ea, number);
    }

    it       = eina_array_iterator_new(ea);
    filtered = eina_iterator_filter_new(it, _filter, NULL, (void *)1);
    for (int i = 1; i <= 10; i += 2)
    {
        struct Number *numb;
        Efl_Bool       ret;

        ret = eina_iterator_next(filtered, (void **)&numb);

        efl_assert_fail_if(!ret);
        efl_assert_int_eq(numb->number, i);
    }

    {
        struct Number *numb;
        Efl_Bool       ret;

        ret = eina_iterator_next(filtered, (void **)&numb);
        efl_assert_fail_if(ret);
    }

    eina_iterator_free(it);

    eina_array_free(ea);
}

struct Free_test
{
    Efl_Bool got_freed;
};

static Efl_Bool
_filter_free(const void *it EFL_UNUSED,
             void *data     EFL_UNUSED,
             void *fdata    EFL_UNUSED)
{
    return EFL_TRUE;
}

static void
_free(void *data)
{
    struct Free_test *test = data;
    test->got_freed        = EFL_TRUE;
}

TEST(iterator_filter_free)
{
    Eina_Iterator    *it, *filtered;
    Eina_Array       *ea;
    struct Free_test *data;

    data = calloc(1, sizeof(struct Free_test));
    ea   = eina_array_new(11);
    efl_assert_fail_if(!ea);

    it       = eina_array_iterator_new(ea);
    filtered = eina_iterator_filter_new(it, _filter_free, _free, data);
    eina_iterator_free(filtered);

    eina_array_free(ea);

    efl_assert_int_ne(data->got_freed, 0);

    free(data);
}
static Efl_Bool
eina_iterator_array_check(EFL_UNUSED const Eina_Array *array,
                          int                         *data,
                          int                         *fdata)
{
    efl_assert_fail_if(*fdata > *data);
    *fdata = *data;

    return EFL_TRUE;
}

TEST(iterator_array_simple)
{
    Eina_Iterator *it;
    Eina_Array    *ea;
    int           *tmp;
    int            i;

    ea = eina_array_new(11);
    efl_assert_fail_if(!ea);

    for (i = 0; i < 200; ++i)
    {
        tmp = malloc(sizeof(int));
        efl_assert_fail_if(!tmp);
        *tmp = i;

        eina_array_push(ea, tmp);
    }

    it = eina_array_iterator_new(ea);
    efl_assert_fail_if(!it);

    i = -1;
    eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_array_check), &i);
    efl_assert_fail_if(i != 199);

    efl_assert_fail_if(eina_iterator_container_get(it) != ea);
    efl_assert_fail_if(eina_iterator_next(it, (void **)&tmp) != EFL_FALSE);

    eina_iterator_free(it);

    eina_array_free(ea);
}

static Efl_Bool
eina_iterator_hash_key_check(const Eina_Hash *hash,
                             const char      *key,
                             EFL_UNUSED void *fdata)
{
    efl_assert_fail_if(eina_hash_find(hash, key) == NULL);

    return EFL_TRUE;
}

static Efl_Bool
eina_iterator_hash_data_check(const Eina_Hash *hash,
                              int             *data,
                              EFL_UNUSED void *fdata)
{
    char tmp[10];

    snprintf(tmp, 10, "%i", *data);
    efl_assert_fail_if(eina_hash_find(hash, tmp) != data);

    return EFL_TRUE;
}

static Efl_Bool
eina_iterator_hash_tuple_check(EFL_UNUSED const Eina_Hash *hash,
                               Eina_Hash_Tuple            *tuple,
                               EFL_UNUSED void            *fdata)
{
    efl_assert_fail_if(atoi((char *)tuple->key) != *((int *)tuple->data));

    return EFL_TRUE;
}

TEST(iterator_hash_simple)
{
    Eina_Iterator *it;
    Eina_Hash     *hash;
    int            array[] = { 1, 42, 7, 8, 6 };

    hash = eina_hash_string_superfast_new(NULL);
    efl_assert_fail_if(hash == NULL);

    efl_assert_fail_if(eina_hash_add(hash, "1", &array[0]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "42", &array[1]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "7", &array[2]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "8", &array[3]) != EFL_TRUE);
    efl_assert_fail_if(eina_hash_add(hash, "6", &array[4]) != EFL_TRUE);

    it = eina_hash_iterator_key_new(hash);
    eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_hash_key_check), NULL);
    eina_iterator_free(it);

    it = eina_hash_iterator_data_new(hash);
    eina_iterator_foreach(it,
                          EINA_EACH_CB(eina_iterator_hash_data_check),
                          NULL);
    eina_iterator_free(it);

    it = eina_hash_iterator_tuple_new(hash);
    eina_iterator_foreach(it,
                          EINA_EACH_CB(eina_iterator_hash_tuple_check),
                          NULL);
    eina_iterator_free(it);

    eina_hash_free(hash);
}

typedef struct _Eina_Test_Inlist Eina_Test_Inlist;

struct _Eina_Test_Inlist
{
    EINA_INLIST;
    int i;
};

static Eina_Test_Inlist *
_eina_test_inlist_build(int i)
{
    Eina_Test_Inlist *tmp;

    tmp = malloc(sizeof(Eina_Test_Inlist));
    efl_assert_fail_if(!tmp);
    tmp->i = i;

    return tmp;
}

static Efl_Bool
eina_iterator_inlist_data_check(EFL_UNUSED const Eina_Inlist *in_list,
                                Eina_Test_Inlist             *data,
                                int                          *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(data->i != 27);
            break;

        case 1:
            efl_assert_fail_if(data->i != 42);
            break;

        case 2:
            efl_assert_fail_if(data->i != 3227);
            break;

        case 3:
            efl_assert_fail_if(data->i != 1664);
            break;

        case 4:
            efl_assert_fail_if(data->i != 81);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

TEST(iterator_inlist_simple)
{
    Eina_Inlist      *lst = NULL;
    Eina_Test_Inlist *tmp;
    Eina_Test_Inlist *prev;
    Eina_Iterator    *it;
    int               i = 0;

    tmp = _eina_test_inlist_build(42);
    lst = eina_inlist_append(lst, EINA_INLIST_GET(tmp));
    efl_assert_fail_if(!lst);

    tmp = _eina_test_inlist_build(1664);
    lst = eina_inlist_append_relative(lst, EINA_INLIST_GET(tmp), lst);
    efl_assert_fail_if(!lst);
    efl_assert_fail_if(((Eina_Test_Inlist *)lst)->i != 42);

    prev = tmp;
    tmp  = _eina_test_inlist_build(3227);
    lst  = eina_inlist_prepend_relative(lst,
                                       EINA_INLIST_GET(tmp),
                                       EINA_INLIST_GET(prev));
    efl_assert_fail_if(!lst);
    efl_assert_fail_if(((Eina_Test_Inlist *)lst)->i != 42);

    tmp = _eina_test_inlist_build(27);
    lst = eina_inlist_prepend_relative(lst, EINA_INLIST_GET(tmp), NULL);

    tmp = _eina_test_inlist_build(81);
    lst = eina_inlist_append_relative(lst, EINA_INLIST_GET(tmp), NULL);

    it = eina_inlist_iterator_new(lst);
    efl_assert_fail_if(!it);

    eina_iterator_foreach(it,
                          EINA_EACH_CB(eina_iterator_inlist_data_check),
                          &i);
    eina_iterator_free(it);

    efl_assert_fail_if(i != 5);
}

static Efl_Bool
eina_iterator_list_data_check(EFL_UNUSED const Eina_List *list,
                              int                        *data,
                              int                        *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(*data != 81);
            break;

        case 1:
            efl_assert_fail_if(*data != 7);
            break;

        case 2:
            efl_assert_fail_if(*data != 9);
            break;

        case 3:
            efl_assert_fail_if(*data != 6);
            break;

        case 4:
            efl_assert_fail_if(*data != 42);
            break;

        case 5:
            efl_assert_fail_if(*data != 1);
            break;

        case 6:
            efl_assert_fail_if(*data != 1337);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

TEST(iterator_list_simple)
{
    Eina_List     *list = NULL;
    Eina_Iterator *it;
    int            data[] = { 6, 9, 42, 1, 7, 1337, 81, 1664 };
    int            i      = 0;

    list = eina_list_append(list, &data[0]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_prepend(list, &data[1]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_append(list, &data[2]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_append(list, &data[3]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_prepend(list, &data[4]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_append(list, &data[5]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_prepend(list, &data[6]);
    efl_assert_fail_if(list == NULL);

    it = eina_list_iterator_new(list);
    efl_assert_fail_if(!it);

    eina_iterator_foreach(it, EINA_EACH_CB(eina_iterator_list_data_check), &i);
    eina_iterator_free(it);
}

static Efl_Bool
eina_reverse_iterator_list_data_check(EFL_UNUSED const Eina_List *list,
                                      int                        *data,
                                      int                        *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(*data != 1337);
            break;

        case 1:
            efl_assert_fail_if(*data != 1);
            break;

        case 2:
            efl_assert_fail_if(*data != 42);
            break;

        case 3:
            efl_assert_fail_if(*data != 6);
            break;

        case 4:
            efl_assert_fail_if(*data != 9);
            break;

        case 5:
            efl_assert_fail_if(*data != 7);
            break;

        case 6:
            efl_assert_fail_if(*data != 81);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

TEST(reverse_iterator_list_simple)
{
    Eina_List     *list = NULL;
    Eina_Iterator *it;
    int            data[] = { 6, 9, 42, 1, 7, 1337, 81, 1664 };
    int            i      = 0;

    list = eina_list_append(list, &data[0]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_prepend(list, &data[1]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_append(list, &data[2]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_append(list, &data[3]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_prepend(list, &data[4]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_append(list, &data[5]);
    efl_assert_fail_if(list == NULL);

    list = eina_list_prepend(list, &data[6]);
    efl_assert_fail_if(list == NULL);

    it = eina_list_iterator_reversed_new(list);
    efl_assert_fail_if(!it);

    eina_iterator_foreach(it,
                          EINA_EACH_CB(eina_reverse_iterator_list_data_check),
                          &i);
    eina_iterator_free(it);
}

typedef struct _Eina_Rbtree_Int Eina_Rbtree_Int;

struct _Eina_Rbtree_Int
{
    Eina_Rbtree node;
    int         value;
};

static Eina_Rbtree_Direction
eina_rbtree_int_cmp(const Eina_Rbtree_Int *left,
                    const Eina_Rbtree_Int *right,
                    void *data             EFL_UNUSED)
{
    efl_assert_fail_if(!left);
    efl_assert_fail_if(!right);

    if (left->value < right->value) return EINA_RBTREE_LEFT;

    return EINA_RBTREE_RIGHT;
}

static Eina_Rbtree *
_eina_rbtree_int_new(int value)
{
    Eina_Rbtree_Int *it;

    it = malloc(sizeof(Eina_Rbtree_Int));
    efl_assert_fail_if(!it);

    it->value = value;

    return &it->node;
}

static Efl_Bool
eina_iterator_rbtree_data_check_sorted(EFL_UNUSED const Eina_List *list,
                                       Eina_Rbtree_Int            *data,
                                       int                        *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(data->value != 10);
            break;

        case 1:
            efl_assert_fail_if(data->value != 27);
            break;

        case 2:
            efl_assert_fail_if(data->value != 42);
            break;

        case 3:
            efl_assert_fail_if(data->value != 69);
            break;

        case 4:
            efl_assert_fail_if(data->value != 1337);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

static Efl_Bool
eina_iterator_rbtree_data_check_prefix(EFL_UNUSED const Eina_List *list,
                                       Eina_Rbtree_Int            *data,
                                       int                        *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(data->value != 27);
            break;

        case 1:
            efl_assert_fail_if(data->value != 10);
            break;

        case 2:
            efl_assert_fail_if(data->value != 69);
            break;

        case 3:
            efl_assert_fail_if(data->value != 42);
            break;

        case 4:
            efl_assert_fail_if(data->value != 1337);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

static Efl_Bool
eina_iterator_rbtree_data_check_postfix(EFL_UNUSED const Eina_List *list,
                                        Eina_Rbtree_Int            *data,
                                        int                        *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(data->value != 10);
            break;

        case 1:
            efl_assert_fail_if(data->value != 42);
            break;

        case 2:
            efl_assert_fail_if(data->value != 1337);
            break;

        case 3:
            efl_assert_fail_if(data->value != 69);
            break;

        case 4:
            efl_assert_fail_if(data->value != 27);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

TEST(iterator_rbtree_simple)
{
    Eina_Rbtree   *root = NULL;
    Eina_Iterator *it;
    int            i;

    root =
        eina_rbtree_inline_insert(NULL,
                                  _eina_rbtree_int_new(10),
                                  EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
                                  NULL);
    efl_assert_fail_if(!root);

    root =
        eina_rbtree_inline_insert(root,
                                  _eina_rbtree_int_new(1337),
                                  EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
                                  NULL);
    efl_assert_fail_if(!root);

    root =
        eina_rbtree_inline_insert(root,
                                  _eina_rbtree_int_new(27),
                                  EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
                                  NULL);
    efl_assert_fail_if(!root);

    root =
        eina_rbtree_inline_insert(root,
                                  _eina_rbtree_int_new(69),
                                  EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
                                  NULL);
    efl_assert_fail_if(!root);

    root =
        eina_rbtree_inline_insert(root,
                                  _eina_rbtree_int_new(42),
                                  EINA_RBTREE_CMP_NODE_CB(eina_rbtree_int_cmp),
                                  NULL);
    efl_assert_fail_if(!root);

    i  = 0;
    it = eina_rbtree_iterator_prefix(root);
    efl_assert_fail_if(!it);

    eina_iterator_foreach(it,
                          EINA_EACH_CB(eina_iterator_rbtree_data_check_prefix),
                          &i);
    eina_iterator_free(it);

   /* This will return the item sorted. */
    i  = 0;
    it = eina_rbtree_iterator_infix(root);
    efl_assert_fail_if(!it);

    eina_iterator_foreach(it,
                          EINA_EACH_CB(eina_iterator_rbtree_data_check_sorted),
                          &i);
    eina_iterator_free(it);

    i  = 0;
    it = eina_rbtree_iterator_postfix(root);
    efl_assert_fail_if(!it);

    eina_iterator_foreach(it,
                          EINA_EACH_CB(eina_iterator_rbtree_data_check_postfix),
                          &i);
    eina_iterator_free(it);
}

TEST(iterator_carray_length)
{
    unsigned int   array[] = { 1, 4, 9, 16 };
    Eina_Iterator *it;
    unsigned int   j = 1;
    unsigned int   i = 0;

    it = EINA_C_ARRAY_ITERATOR_NEW(array);
    EINA_ITERATOR_FOREACH(it, i)
    {
        efl_assert_fail_if(i != j * j);
        j++;
    }
    efl_assert_fail_if(j < EINA_C_ARRAY_LENGTH(array));
    eina_iterator_free(it);
}

TEST(iterator_multi)
{
    unsigned int   array1[] = { 1, 4, 9, 16 };
    unsigned int   array2[] = { 25, 36, 49, 64 };
    unsigned int   array3[] = { 81, 100, 121, 144 };
    Eina_Iterator *it;
    unsigned int   i;
    unsigned int   j = 1;

    it = eina_multi_iterator_new(EINA_C_ARRAY_ITERATOR_NEW(array1),
                                 EINA_C_ARRAY_ITERATOR_NEW(array2),
                                 EINA_C_ARRAY_ITERATOR_NEW(array3));
    EINA_ITERATOR_FOREACH(it, i)
    {
        efl_assert_fail_if(i != j * j);
        j++;
    }
    efl_assert_fail_if(j < EINA_C_ARRAY_LENGTH(array1) + EINA_C_ARRAY_LENGTH(array2) +
                    EINA_C_ARRAY_LENGTH(array3));
    eina_iterator_free(it);
}

static void *
_return_x(const void *container EFL_UNUSED, void *data, void *fdata)
{
    Eina_Rectangle *rect = data;
    efl_assert_int_eq(*((int *)fdata), 1337);

    return &rect->x;
}

static void
_free_cb(void *data)
{
    int *free_data = data;

    *free_data = 0;
}

TEST(iterator_process)
{
    Eina_Inarray  *rects      = eina_inarray_new(sizeof(Eina_Rectangle), 5);
    Eina_Rectangle rect_arr[] = {
        { 0,  1,  2,  3},
        { 4,  5,  6,  7},
        { 8,  9, 10, 11},
        {12, 13, 14, 15},
        {16, 17, 18, 19}
    };
    Eina_Iterator *it;
    int            free = 1337;
    int           *a, i = 0;

    eina_inarray_push(rects, &rect_arr[0]);
    eina_inarray_push(rects, &rect_arr[1]);
    eina_inarray_push(rects, &rect_arr[2]);
    eina_inarray_push(rects, &rect_arr[3]);
    eina_inarray_push(rects, &rect_arr[4]);

    it = eina_iterator_processed_new(eina_inarray_iterator_new(rects),
                                     _return_x,
                                     _free_cb,
                                     &free);
    EINA_ITERATOR_FOREACH(it, a)
    {
        efl_assert_int_eq(*a, i * 4);
        i++;
    }
    efl_assert_int_eq(i, 5);
    eina_iterator_free(it);
    efl_assert_int_eq(free, 0);
}
