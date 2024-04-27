#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(array_simple)
{
    Eina_Array         *ea;
    char               *tmp;
    Eina_Array_Iterator it;
    unsigned int        i;

    ea = eina_array_new(11);
    efl_assert_fail_if(!ea);

    for (i = 0; i < 201; ++i)
    {
        tmp = malloc(sizeof(char) * 10);
        efl_assert_fail_if(!tmp);
        eina_convert_itoa(i, tmp);

        eina_array_push(ea, tmp);
    }

    efl_assert_fail_if(eina_array_data_get(ea, 10) == NULL);
    efl_assert_fail_if(atoi(eina_array_data_get(ea, 10)) != 10);
    tmp = eina_array_pop(ea);
    efl_assert_fail_if(tmp == NULL);
    efl_assert_fail_if(atoi(tmp) != 200);
    free(tmp);

    EINA_ARRAY_ITER_NEXT(ea, i, tmp, it)
    {
        efl_assert_fail_if((unsigned int)atoi(tmp) != i);
        free(tmp);
    }

    efl_assert_fail_if(i != 200);

    eina_array_clean(ea);
    eina_array_flush(ea);
    eina_array_free(ea);
}

TEST(array_static)
{
    Eina_Array          sea;
    char               *tmp;
    Eina_Array_Iterator it;
    unsigned int        i;

    eina_array_step_set(&sea, sizeof(sea), 10);

    for (i = 0; i < 200; ++i)
    {
        tmp = malloc(sizeof(char) * 10);
        efl_assert_fail_if(!tmp);
        eina_convert_itoa(i, tmp);

        eina_array_push(&sea, tmp);
    }

    efl_assert_fail_if(eina_array_data_get(&sea, 10) == NULL);
    efl_assert_fail_if(atoi(eina_array_data_get(&sea, 10)) != 10);

    EINA_ARRAY_ITER_NEXT(&sea, i, tmp, it)
    {
        efl_assert_fail_if((unsigned int)atoi(tmp) != i);
        free(tmp);
    }

    efl_assert_fail_if(i != 200);

    eina_array_clean(&sea);
    eina_array_flush(&sea);
}

static Efl_Bool
keep_int(void *data, void *gdata)
{
    int *tmp = data;

    efl_assert_fail_if(gdata);
    efl_assert_fail_if(!tmp);

    if (*tmp == 0) return EFL_FALSE;

    return EFL_TRUE;
}

TEST(array_remove_stuff)
{
    Eina_Array         *ea;
    int                *tmp;
    Eina_Array_Iterator it;
    unsigned int        i;

    ea = eina_array_new(64);
    efl_assert_fail_if(!ea);

    for (i = 0; i < 1000; ++i)
    {
        tmp = malloc(sizeof(int));
        efl_assert_fail_if(!tmp);
        *tmp = i;

        eina_array_push(ea, tmp);
    }

   // Remove the first 10 items
    for (i = 0; i < 10; ++i)
    {
        tmp = eina_array_data_get(ea, i);
        efl_assert_fail_if(!tmp);
        *tmp = 0;
    }
    efl_assert_fail_if(eina_array_remove(ea, keep_int, NULL) != EFL_TRUE);

    efl_assert_fail_if(eina_array_count(ea) != 990);
    EINA_ARRAY_ITER_NEXT(ea, i, tmp, it)
    efl_assert_fail_if(*tmp == 0);

   // Remove the last items
    for (i = 980; i < 990; ++i)
    {
        tmp = eina_array_data_get(ea, i);
        efl_assert_fail_if(!tmp);
        *tmp = 0;
    }
    eina_array_remove(ea, keep_int, NULL);

   // Remove all items
    efl_assert_fail_if(eina_array_count(ea) != 980);
    EINA_ARRAY_ITER_NEXT(ea, i, tmp, it)
    {
        efl_assert_fail_if(*tmp == 0);
        *tmp = 0;
    }

    eina_array_remove(ea, keep_int, NULL);

    efl_assert_fail_if(eina_array_count(ea) != 0);

    eina_array_free(ea);
}

TEST(array_find_test)
{
    Eina_Array   sea;
    uintptr_t    i;
    unsigned int out = 0;

    efl_assert_fail_if(eina_array_find(NULL, (void *)1, NULL) != EFL_FALSE);

    eina_array_step_set(&sea, sizeof(sea), 5);

    for (i = 1; i < 10; i++)
        eina_array_push(&sea, (void *)i);

    efl_assert_fail_if(eina_array_find(&sea, (void *)15, NULL) != EFL_FALSE);

    efl_assert_fail_if(eina_array_find(&sea, (void *)5, NULL) != EFL_TRUE);
    efl_assert_fail_if(eina_array_find(&sea, (void *)6, &out) != EFL_TRUE);
    efl_assert_fail_if(out != 5);

    eina_array_data_set(&sea, 7, (void *)99);
    efl_assert_fail_if(eina_array_find(&sea, (void *)99, &out) != EFL_TRUE);
    efl_assert_fail_if(out != 7);

    eina_array_flush(&sea);
}
