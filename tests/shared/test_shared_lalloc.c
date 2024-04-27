/*
 * Copyright (C) 2008 Cedric Bail
 */
#include <stdlib.h>
#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

typedef struct _Eina_Lazy_Allocator_Test Eina_Lazy_Allocator_Test;

struct _Eina_Lazy_Allocator_Test
{
    void *data;
    int   num;
};

static Efl_Bool
_test_alloc(Eina_Lazy_Allocator_Test *elat, int num)
{
    if (elat->num == 10 && num == 1) return EFL_FALSE;

    if (elat->num == 122 && num == 128) return EFL_FALSE;

    elat->num += num;
    elat->data = realloc(elat->data, elat->num);

    return EFL_TRUE;
}

static void
_test_free(Eina_Lazy_Allocator_Test *elat)
{
    free(elat->data);
    elat->data = NULL;
    elat->num  = 0;
}

TEST(lalloc_simple)
{
    Eina_Lazy_Allocator_Test *elat;
    Eina_Lalloc              *test;
    int                       i;

    elat = calloc(1, sizeof(Eina_Lazy_Allocator_Test));
    efl_assert_fail_if(!elat);

    test = eina_lalloc_new(elat,
                           EINA_LALLOC_ALLOC(_test_alloc),
                           EINA_LALLOC_FREE(_test_free),
                           10);
    efl_assert_fail_if(!test);

    for (i = 0; i < 10; ++i)
        efl_assert_fail_if(eina_lalloc_element_add(test) != EFL_TRUE);

    efl_assert_fail_if(eina_lalloc_element_add(test) != EFL_FALSE);
    efl_assert_fail_if(eina_lalloc_elements_add(test, 5) != EFL_TRUE);

    for (i = 0; i < 21; ++i)
        efl_assert_fail_if(eina_lalloc_element_add(test) != EFL_TRUE);

    efl_assert_fail_if(eina_lalloc_elements_add(test, 50) != EFL_FALSE);

    eina_lalloc_free(test);
}