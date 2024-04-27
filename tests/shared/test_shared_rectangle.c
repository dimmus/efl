/*
 * Copyright (C) 2007-2008 Cedric BAIL, Carsten Haitzler
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(rectangle_pool)
{
    Eina_Rectangle_Pool *pool;
    Eina_Rectangle      *rects[8][8];
    int                  x;
    int                  y;
    int                  w;
    int                  h;

    pool = eina_rectangle_pool_new(256, 256);
    efl_assert_fail_if(pool == NULL);

    eina_rectangle_pool_data_set(pool, rects);
    efl_assert_fail_if(eina_rectangle_pool_data_get(pool) != rects);

    efl_assert_fail_if(eina_rectangle_pool_request(pool, 1024, 1024) != NULL);

    for (x = 0; x < 8; x++)
        for (y = 0; y < 8; y++)
        {
            rects[x][y] = eina_rectangle_pool_request(pool, 32, 32);
            efl_assert_fail_if(rects[x][y] == NULL);
        }

    efl_assert_fail_if(eina_rectangle_pool_count(pool) != 64);

    efl_assert_fail_if(eina_rectangle_pool_get(rects[0][0]) != pool);

    efl_assert_fail_if(eina_rectangle_pool_geometry_get(pool, &w, &h) !=
                       EFL_TRUE);
    efl_assert_fail_if(w != 256 || h != 256);

    efl_assert_fail_if(eina_rectangle_pool_request(pool, 32, 32) != NULL);
    efl_assert_fail_if(eina_rectangle_pool_request(pool, 1024, 1024) != NULL);

    for (x = 0; x < 8; x++)
        eina_rectangle_pool_release(rects[0][x]);

    efl_assert_fail_if(eina_rectangle_pool_request(pool, 16, 16) == NULL);

    eina_rectangle_pool_free(pool);
}

TEST(rectangle_pool_skyline)
{
    Eina_Rectangle_Pool *pool;
    Eina_Rectangle      *rects[8][8];
    int                  x;
    int                  y;
    int                  w;
    int                  h;

    pool = eina_rectangle_pool_new(256, 256);
    efl_assert_fail_if(pool == NULL);

    eina_rectangle_pool_packing_set(pool,
                                    Eina_Packing_Bottom_Left_Skyline_Improved);

    eina_rectangle_pool_data_set(pool, rects);
    efl_assert_fail_if(eina_rectangle_pool_data_get(pool) != rects);

    efl_assert_fail_if(eina_rectangle_pool_request(pool, 1024, 1024) != NULL);

    for (x = 0; x < 8; x++)
        for (y = 0; y < 8; y++)
        {
            rects[x][y] = eina_rectangle_pool_request(pool, 32, 32);
            efl_assert_fail_if(rects[x][y] == NULL);
        }

    efl_assert_fail_if(eina_rectangle_pool_count(pool) != 64);

    efl_assert_fail_if(eina_rectangle_pool_get(rects[0][0]) != pool);

    efl_assert_fail_if(eina_rectangle_pool_geometry_get(pool, &w, &h) !=
                       EFL_TRUE);
    efl_assert_fail_if(w != 256 || h != 256);

    efl_assert_fail_if(eina_rectangle_pool_request(pool, 32, 32) != NULL);
    efl_assert_fail_if(eina_rectangle_pool_request(pool, 1024, 1024) != NULL);

    for (x = 0; x < 8; x++)
        eina_rectangle_pool_release(rects[0][x]);

    eina_rectangle_pool_free(pool);
}

TEST(rectangle_union_intersect)
{
    Eina_Rectangle r1, r2, r3, r4, r5, r6, r7, r8, rd;

    EINA_RECTANGLE_SET(&r1, 10, 10, 50, 50);
    EINA_RECTANGLE_SET(&r2, 20, 20, 20, 20);
    EINA_RECTANGLE_SET(&r3, 0, 0, 10, 10);
    EINA_RECTANGLE_SET(&r4, 30, 30, 50, 50);
    EINA_RECTANGLE_SET(&r5, 10, 10, 0, 0);
    EINA_RECTANGLE_SET(&r6, 30, 30, 0, 0);
    EINA_RECTANGLE_SET(&r7, 10, 10, 5, 0);
    EINA_RECTANGLE_SET(&r8, 10, 10, 0, 5);

    rd = r1;

    efl_assert_fail_if(eina_rectangle_intersection(&rd, &r3));
    efl_assert_fail_if(!eina_rectangle_intersection(&rd, &r2));

    efl_assert_fail_if(rd.x != r2.x || rd.y != r2.y || rd.w != r2.w ||
                       rd.h != r2.h);

    rd = r1;

    efl_assert_fail_if(!eina_rectangle_intersection(&rd, &r4));

    efl_assert_fail_if(rd.x != 30 || rd.y != 30 || rd.w != 30 || rd.h != 30);

    rd = r1;
    eina_rectangle_union(&rd, &r2);
    efl_assert_fail_if(rd.x != r1.x || rd.y != r1.y || rd.w != r1.w ||
                       rd.h != r1.h);

    rd = r6;
    efl_assert_fail_if(eina_rectangle_intersection(&rd, &r5));

    rd = r7;
    efl_assert_fail_if(eina_rectangle_intersection(&rd, &r3));

    rd = r8;
    efl_assert_fail_if(eina_rectangle_intersection(&rd, &r3));

    rd = r1;
    eina_rectangle_union(&rd, &r3);
    efl_assert_fail_if(rd.x != 0 || rd.y != 0 || rd.w != 60 || rd.h != 60);

    rd = r3;
    eina_rectangle_union(&rd, &r4);
    efl_assert_fail_if(rd.x != 0 || rd.y != 0 || rd.w != 80 || rd.h != 80);

    rd = r5;
    eina_rectangle_union(&rd, &r6);
    efl_assert_fail_if(rd.x != 10 || rd.y != 10 || rd.w != 20 || rd.h != 20);
}

TEST(rectangle_position_test)
{
    Eina_Rectangle middle, top, down, right, left;
    EINA_RECTANGLE_SET(&middle, -1, -1, 2.0, 2.0);
    EINA_RECTANGLE_SET(&top, -1, -2, 2.0, 2.0);
    EINA_RECTANGLE_SET(&right, 0, -1, 2.0, 2.0);
    EINA_RECTANGLE_SET(&left, -2, -1, 2.0, 2.0);
    EINA_RECTANGLE_SET(&down, -1, 0, 2.0, 2.0);

    efl_assert_int_eq(eina_rectangle_outside_position(&middle, &top),
                      EINA_RECTANGLE_OUTSIDE_TOP);
    efl_assert_int_eq(eina_rectangle_outside_position(&middle, &down),
                      EINA_RECTANGLE_OUTSIDE_BOTTOM);
    efl_assert_int_eq(eina_rectangle_outside_position(&middle, &right),
                      EINA_RECTANGLE_OUTSIDE_RIGHT);
    efl_assert_int_eq(eina_rectangle_outside_position(&middle, &left),
                      EINA_RECTANGLE_OUTSIDE_LEFT);
}
