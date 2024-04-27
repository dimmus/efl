/*
 * Copyright (C) 2012 ProFUSION embedded systems
 */
#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(inarray_test_simple)
{
    const int     test_members = 5;
    Eina_Inarray *array;
    int           i, pos, *member;

    const struct spec
    {
        int pos, value;
    } *s, specs[] = {
        {test_members,   1234},
        {           5, 0x1337},
        {           0, 0xbeef},
        {          -1,     -1}
    };

    array = eina_inarray_new(sizeof(int), 2);
    assert(array != NULL);

    for (i = 0; i < test_members; i++)
    {
        pos = eina_inarray_push(array, &i);
        assert(pos == i);
    }
    assert(eina_inarray_count(array) == (unsigned)test_members);

    for (i = 0; i < test_members; i++)
    {
        member = eina_inarray_nth(array, i);
        assert(*member == i);
    }

    for (s = specs; s->pos >= 0; s++)
    {
        assert(eina_inarray_insert_at(array, s->pos, &s->value));

        for (i = 0; i < s->pos; i++)
        {
            member = eina_inarray_nth(array, i);
            assert(*member == i);
        }
        member = eina_inarray_nth(array, s->pos);
        assert(*member == s->value);
        for (i = s->pos + 1; i < test_members + 1; i++)
        {
            member = eina_inarray_nth(array, i);
            assert(*member == i - 1);
        }

        assert(eina_inarray_remove_at(array, s->pos));
        for (i = 0; i < test_members; i++)
        {
            member = eina_inarray_nth(array, i);
            assert(*member == i);
        }
    }

    eina_inarray_free(array);
}

TEST(inarray_test_alloc_at)
{
    Eina_Inarray *array;
    int          *member;
    int           i;

    array = eina_inarray_new(sizeof(int), 2);
    assert(array != NULL);

    member = eina_inarray_alloc_at(array, 0, 4);
    assert(member != NULL);
    assert(eina_inarray_count(array) == 4);

    for (i = 0; i < 4; i++)
        member[i] = i + 2;

    member = eina_inarray_alloc_at(array, 0, 2);
    assert(member != NULL);
    assert(eina_inarray_count(array) == 6);
    for (i = 0; i < 2; i++)
        member[i] = i;

    member = eina_inarray_alloc_at(array, 6, 2);
    assert(member != NULL);
    assert(eina_inarray_count(array) == 8);
    for (i = 0; i < 2; i++)
        member[i] = i + 6;

    member = array->members;
    for (i = 0; i < 8; i++)
        assert(member[i] == i);

    eina_inarray_free(array);
}

static const short rand_numbers[] = { 9, 0, 2, 3, 6, 5, 4, 7, 8, 1, 10 };
static const int numbers_count = sizeof(rand_numbers) / sizeof(rand_numbers[0]);

static void
show_sort_array(const Eina_Inarray *array)
{
    int i, len = eina_inarray_count(array);
    printf("array with %d members:\n", len);
    for (i = 0; i < len; i++)
    {
        short *member = eina_inarray_nth(array, i);
        printf("\tarray[%2d]=%5hd\n", i, *member);
    }
}

static Efl_Bool
check_short_sorted(const Eina_Inarray *array)
{
    int i;
    for (i = 0; i < numbers_count; i++)
    {
        short *member = eina_inarray_nth(array, i);
        if (*member != (short)i)
        {
            show_sort_array(array);
            fprintf(stderr, "not sorted at %d: %hd\n", i, *member);
            return EFL_FALSE;
        }
    }
    return EFL_TRUE;
}

static int
short_cmp(const void *pa, const void *pb)
{
    const short *a = pa, *b = pb;
    return *a - *b;
}

TEST(inarray_test_insert_sort)
{
    Eina_Inarray *array;
    int           i, pos;

    array = eina_inarray_new(sizeof(short), 1);
    assert(array != NULL);

   /* insert sorted and already sorted sequence */
    for (i = 0; i < numbers_count; i++)
    {
        short val = i;
        pos       = eina_inarray_insert(array, &val, short_cmp);
        assert(pos == (int)val);
    }
    assert(check_short_sorted(array));
    eina_inarray_flush(array);

    for (i = 0; i < numbers_count; i++)
    {
        short val = i;
        pos       = eina_inarray_insert_sorted(array, &val, short_cmp);
        assert(pos == (int)val);
    }
    assert(check_short_sorted(array));
    eina_inarray_flush(array);

   /* insert sorted the reverse sequence */
    for (i = 0; i < numbers_count; i++)
    {
        short val = numbers_count - i - 1;
        pos       = eina_inarray_insert(array, &val, short_cmp);
        assert(pos == 0);
    }
    assert(check_short_sorted(array));
    eina_inarray_flush(array);

    for (i = 0; i < numbers_count; i++)
    {
        short val = numbers_count - i - 1;
        pos       = eina_inarray_insert_sorted(array, &val, short_cmp);
        assert(pos == 0);
    }
    assert(check_short_sorted(array));
    eina_inarray_flush(array);

   /* insert sorted random numbers */
    for (i = 0; i < numbers_count; i++)
    {
        short val = rand_numbers[i];
        eina_inarray_insert(array, &val, short_cmp);
    }
    assert(check_short_sorted(array));
    eina_inarray_flush(array);

    for (i = 0; i < numbers_count; i++)
    {
        short val = rand_numbers[i];
        eina_inarray_insert_sorted(array, &val, short_cmp);
    }
    assert(check_short_sorted(array));
    eina_inarray_flush(array);

    eina_inarray_free(array);
}

TEST(inarray_test_sort)
{
    Eina_Inarray *array;
    int           i;

    array = eina_inarray_new(sizeof(short), 1);
    assert(array != NULL);

    for (i = 0; i < numbers_count; i++)
    {
        short val = rand_numbers[i];
        eina_inarray_push(array, &val);
    }
    eina_inarray_sort(array, short_cmp);
    assert(check_short_sorted(array));
    eina_inarray_free(array);
}

TEST(inarray_test_reverse)
{
    Eina_Inarray *array;
    int           i;

    array = eina_inarray_new(sizeof(short), 1);
    assert(array != NULL);

    for (i = 0; i < numbers_count; i++)
    {
        short val = i;
        eina_inarray_push(array, &val);
    }
    eina_inarray_reverse(array);

    for (i = 0; i < numbers_count; i++)
    {
        short *member = eina_inarray_nth(array, i);
        assert(*member == (numbers_count - i - 1));
    }

    eina_inarray_free(array);
}

static Efl_Bool
array_foreach(const void *array EFL_UNUSED, void *p, void *user_data EFL_UNUSED)
{
    short *member = p;
    int   *i      = user_data;
    assert(*i == *member);
    (*i)++;
    return EFL_TRUE;
}

static Efl_Bool
array_foreach_stop_2nd(const void *array EFL_UNUSED,
                       void             *p,
                       void *user_data   EFL_UNUSED)
{
    short *member = p;
    int   *i      = user_data;
    assert(*i == *member);
    if (*i == 1) return EFL_FALSE;
    (*i)++;
    return EFL_TRUE;
}

TEST(inarray_test_itr)
{
    Eina_Inarray  *array;
    Eina_Iterator *it;
    Eina_Accessor *ac;
    short         *member;
    int            i;

    array = eina_inarray_new(sizeof(short), 1);
    assert(array != NULL);

    for (i = 0; i < numbers_count; i++)
    {
        short val = i;
        eina_inarray_push(array, &val);
    }
    i = 0;
    EINA_INARRAY_FOREACH(array, member)
    {
        assert(*member == i);
        i++;
    }
    assert(i == numbers_count);

    i--;
    EINA_INARRAY_REVERSE_FOREACH(array, member)
    {
        assert(*member == i);
        i--;
    }
    assert(i == -1);

    i = 0;
    assert(eina_inarray_foreach(array, array_foreach, &i));
    assert(i == numbers_count);

    i = 0;
    efl_assert_fail_if(eina_inarray_foreach(array, array_foreach_stop_2nd, &i));
    assert(i == 1);

    it = eina_inarray_iterator_new(array);
    assert(it != NULL);
    i = 0;
    EINA_ITERATOR_FOREACH(it, member)
    {
        assert(*member == i);
        i++;
    }
    assert(i == numbers_count);
    eina_iterator_free(it);

    it = eina_inarray_iterator_reversed_new(array);
    assert(it != NULL);
    i--;
    EINA_ITERATOR_FOREACH(it, member)
    {
        assert(*member == i);
        i--;
    }
    assert(i == -1);
    eina_iterator_free(it);

    ac = eina_inarray_accessor_new(array);
    assert(ac != NULL);
    for (i = 0; i < numbers_count; i++)
    {
        assert(eina_accessor_data_get(ac, i, (void **)&member));
        assert(*member == i);
    }
    assert(i == numbers_count);
    eina_accessor_free(ac);

    eina_inarray_free(array);
}

static int
cmp(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

static int
compare(const void *a, const void *b)
{
    return *(int *)a > *(int *)b;
}

static Efl_Bool
match_cb(const void *array EFL_UNUSED, void *p, void *user_data)
{
    int *member = p;
    int *i      = user_data;

    if (*i == *member) return EFL_TRUE;
    else return EFL_FALSE;
}

TEST(inarray_test_search)
{
    Eina_Inarray *iarr;
    int           i, ret, temp = 92, ret1;
    int           arr[]    = { 91, 95, 97, 93, 92, 94 };
    int           arr_size = sizeof(arr) / sizeof(arr[0]);
    unsigned int  curr_len;
    Efl_Bool      rval;

    iarr = eina_inarray_new(sizeof(int), 0);

   //search in empty array
    ret = eina_inarray_search(iarr, &temp, cmp);
    efl_assert_fail_if(ret != -1);

   //search in single element array
    temp = 91;
    eina_inarray_push(iarr, &arr[0]);
    ret = eina_inarray_search(iarr, &temp, cmp);
    efl_assert_fail_if(ret != 0);

    temp = 92;
    ret  = eina_inarray_search(iarr, &temp, cmp);
    efl_assert_fail_if(ret != -1);

    for (i = 1; i < arr_size; ++i)
    {
        ret = eina_inarray_push(iarr, &arr[i]);
        assert(ret == i);
    }
    temp = 92;
    ret  = eina_inarray_search(iarr, &temp, cmp);
    efl_assert_fail_if(ret != 4);

    temp = 100;
    ret  = eina_inarray_search(iarr, &temp, cmp);
    efl_assert_fail_if(ret != -1);

    rval = eina_inarray_replace_at(iarr, 3, &temp);
    efl_assert_fail_if(rval != EFL_TRUE);
    ret = eina_inarray_search(iarr, &temp, cmp);
    efl_assert_fail_if(ret != 3);

    EXPECT_ERROR_START;
    rval = eina_inarray_replace_at(iarr, arr_size + 1, &temp);
    efl_assert_fail_if(rval != EFL_FALSE);
    EXPECT_ERROR_END;

    ret = eina_inarray_remove(iarr, &temp);
    efl_assert_fail_if(ret != 3);

    temp = 101;
    ret  = eina_inarray_remove(iarr, &temp);
    efl_assert_fail_if(ret != -1);

    eina_inarray_sort(iarr, compare);

    temp = 92;
    ret  = eina_inarray_search(iarr, &temp, cmp);
    ret1 = eina_inarray_search_sorted(iarr, &temp, cmp);
    efl_assert_fail_if(ret != ret1);

    temp = 100;
    ret  = eina_inarray_search(iarr, &temp, cmp);
    ret1 = eina_inarray_search_sorted(iarr, &temp, cmp);
    efl_assert_fail_if(ret != ret1);

    temp     = 105;
    ret      = eina_inarray_push(iarr, &temp);
    ret      = eina_inarray_push(iarr, &temp);
    ret      = eina_inarray_push(iarr, &temp);
    curr_len = iarr->len;
    rval     = eina_inarray_foreach_remove(iarr, match_cb, &temp);
    efl_assert_fail_if(iarr->len != curr_len - 3);

    eina_inarray_free(iarr);
}
