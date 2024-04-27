#include <Efl_Shared.h>

#include "efl-test-runner.h"

static Efl_Bool
eina_accessor_check(EFL_UNUSED const Eina_Array *array, int *data, int *fdata)
{
    efl_assert_fail_if(*fdata > *data);
    *fdata = *data;

    return EFL_TRUE;
}

TEST(accessor_array_simple)
{
    Eina_Accessor *it;
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

    it = eina_array_accessor_new(ea);
    efl_assert_fail_if(!it);

    efl_assert_fail_if(eina_accessor_data_get(it, 100, (void **)&tmp) !=
                       EFL_TRUE);
    efl_assert_fail_if(!tmp);
    efl_assert_fail_if(*tmp != 100);

    i = 50;
    eina_accessor_over(it, EINA_EACH_CB(eina_accessor_check), 50, 100, &i);
    efl_assert_fail_if(i != 99);

    efl_assert_fail_if(eina_accessor_container_get(it) != ea);

    eina_accessor_free(it);

    eina_array_free(ea);
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
eina_accessor_inlist_data_check(EFL_UNUSED const Eina_Inlist *in_list,
                                Eina_Test_Inlist             *data,
                                int                          *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(data->i != 3227);
            break;

        case 1:
            efl_assert_fail_if(data->i != 1664);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

TEST(accessor_inlist_simple)
{
    Eina_Inlist      *lst = NULL;
    Eina_Test_Inlist *tmp;
    Eina_Test_Inlist *prev;
    Eina_Accessor    *it;
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

    tmp = _eina_test_inlist_build(7);
    lst = eina_inlist_append(lst, EINA_INLIST_GET(tmp));

    it = eina_inlist_accessor_new(lst);
    efl_assert_fail_if(!it);
    efl_assert_fail_if(eina_accessor_container_get(it) != lst);

    eina_accessor_over(it,
                       EINA_EACH_CB(eina_accessor_inlist_data_check),
                       2,
                       4,
                       &i);

    efl_assert_fail_if(eina_accessor_data_get(it, 5, (void **)&tmp) !=
                       EFL_TRUE);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&tmp) !=
                       EFL_TRUE);
    efl_assert_fail_if(tmp->i != 1664);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&tmp) !=
                       EFL_TRUE);
    efl_assert_fail_if(tmp->i != 1664);
    efl_assert_fail_if(eina_accessor_data_get(it, 1, (void **)&tmp) !=
                       EFL_TRUE);
    efl_assert_fail_if(tmp->i != 42);

    eina_accessor_free(it);

    efl_assert_fail_if(i != 2);
}

static Efl_Bool
eina_iterator_list_data_check(EFL_UNUSED const Eina_List *list,
                              int                        *data,
                              int                        *fdata)
{
    switch (*fdata)
    {
        case 0:
            efl_assert_fail_if(*data != 9);
            break;

        case 1:
            efl_assert_fail_if(*data != 6);
            break;
    }

    (*fdata)++;

    return EFL_TRUE;
}

TEST(accessor_list_simple)
{
    Eina_List     *list = NULL;
    Eina_Accessor *it;
    int            data[] = { 6, 9, 42, 1, 7, 1337, 81, 1664 };
    int           *j;
    int            i = 0;

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

    it = eina_list_accessor_new(list);
    efl_assert_fail_if(!it);

    eina_accessor_over(it,
                       EINA_EACH_CB(eina_iterator_list_data_check),
                       2,
                       4,
                       &i);

    efl_assert_fail_if(eina_accessor_data_get(it, 5, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 1);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 6);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 6);
    efl_assert_fail_if(eina_accessor_data_get(it, 1, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 7);
    efl_assert_fail_if(eina_accessor_data_get(it, 5, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 1);

    eina_accessor_free(it);
}

TEST(accessor_carray_simple_ptr)
{
    Eina_Accessor *it;
    int            data[] = { 6, 9, 42, 1, 7, 1337, 81, 1664 };
    int            j, c;

    it = EINA_C_ARRAY_ACCESSOR_NEW(data);

    EINA_ACCESSOR_FOREACH(it, c, j)
    {
        efl_assert_int_eq(data[c], j);
    }

    efl_assert_fail_if(eina_accessor_data_get(it, 5, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(j != 1337);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(j != 1);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(j != 1);
    efl_assert_fail_if(eina_accessor_data_get(it, 1, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(j != 9);
    efl_assert_fail_if(eina_accessor_data_get(it, 5, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(j != 1337);

    eina_accessor_free(it);
}

TEST(accessor_carray_simple)
{
    Eina_Accessor *it;
    int            data[] = { 6, 9, 42, 1, 7, 1337, 81, 1664 };
    int           *j, c;

    it = EINA_C_ARRAY_ACCESSOR_PTR_NEW(data);

    EINA_ACCESSOR_FOREACH(it, c, j)
    {
        efl_assert_int_eq(data[c], *j);
    }

    efl_assert_fail_if(eina_accessor_data_get(it, 5, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 1337);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 1);
    efl_assert_fail_if(eina_accessor_data_get(it, 3, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 1);
    efl_assert_fail_if(eina_accessor_data_get(it, 1, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 9);
    efl_assert_fail_if(eina_accessor_data_get(it, 5, (void **)&j) != EFL_TRUE);
    efl_assert_fail_if(*j != 1337);

    eina_accessor_free(it);
}
