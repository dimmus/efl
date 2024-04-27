/* 
 * Copyright (C) 2015 Subhransu Mohanty <sub.mohanty@samsung.com>
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <math.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(bezier_test_values)
{
    Eina_Bezier b;
    double      sx, sy, cx1, cy1, cx2, cy2, ex, ey;

    eina_bezier_values_set(&b, 1, 2, 3, 4, 5, 6, 7, 8);
    eina_bezier_values_get(&b, &sx, &sy, &cx1, &cy1, &cx2, &cy2, &ex, &ey);
    efl_assert_fail_if(!EINA_DBL_EQ(sx, 1) || !EINA_DBL_EQ(sy, 2) ||
                       !EINA_DBL_EQ(cx1, 3) || !EINA_DBL_EQ(cy1, 4) ||
                       !EINA_DBL_EQ(cx2, 5) || !EINA_DBL_EQ(cy2, 6) ||
                       !EINA_DBL_EQ(ex, 7) || !EINA_DBL_EQ(ey, 8));
}

TEST(bezier_test_angle)
{
    Eina_Bezier b;
    double      angle;

    eina_bezier_values_set(&b, 1, 1, 3, 1, 5, 1, 7, 1);
    angle = eina_bezier_angle_at(&b, 0.5);

    efl_assert_fail_if(EINA_DBL_NONZERO(angle));

    eina_bezier_values_set(&b, 1, 2, 1, 4, 1, 6, 1, 8);
    angle = eina_bezier_angle_at(&b, 0.5);
    efl_assert_fail_if(!EINA_DBL_EQ(floor(angle), 90));
}

TEST(bezier_test_length)
{
    Eina_Bezier b;
    double      length;

    eina_bezier_values_set(&b, 1, 1, 3, 1, 5, 1, 7, 1);
    length = eina_bezier_length_get(&b);
    efl_assert_fail_if(!EINA_DBL_EQ(floor(length), 6));

    eina_bezier_values_set(&b, 1, 1, 1, 1, 1, 1, 1, 1);
    length = eina_bezier_length_get(&b);
    efl_assert_fail_if(!EINA_DBL_EQ(length, 0));
}

TEST(bezier_test_t_at)
{
    Eina_Bezier b;
    double      length, t;

    eina_bezier_values_set(&b, 1, 1, 3, 1, 5, 1, 7, 1);
    length = eina_bezier_length_get(&b);
    t      = eina_bezier_t_at(&b, 0);
    efl_assert_fail_if(!EINA_DBL_EQ(floor(t), 0));

    t = eina_bezier_t_at(&b, length);
    efl_assert_fail_if(!EINA_DBL_EQ(t, 1));
}

TEST(bezier_test_point_at)
{
    Eina_Bezier b;
    double      x, y;

    eina_bezier_values_set(&b, 1, 2, 3, 4, 5, 6, 7, 8);
    eina_bezier_point_at(&b, 0, &x, &y);
    efl_assert_fail_if(!EINA_DBL_EQ(x, 1) || !EINA_DBL_EQ(y, 2));

    eina_bezier_point_at(&b, 1, &x, &y);

    efl_assert_fail_if(!EINA_DBL_EQ(x, 7) || !EINA_DBL_EQ(y, 8));
}

TEST(bezier_test_split_at_length)
{
    Eina_Bezier b, l, r;
    double      len, len1, len2;

    eina_bezier_values_set(&b, 1, 2, 3, 4, 5, 6, 7, 8);
    len = eina_bezier_length_get(&b);
    eina_bezier_split_at_length(&b, len / 3, &l, &r);
    len1 = eina_bezier_length_get(&l);
    len2 = eina_bezier_length_get(&r);

    efl_assert_fail_if(!EINA_DBL_EQ(len, (len1 + len2)));
}

TEST(bezier_test_bounds_get)
{
    Eina_Bezier b;
    double      x, y, w, h;

    eina_bezier_values_set(&b, 0, 0, 100, 0, 0, 100, 100, 100);

    eina_bezier_bounds_get(&b, &x, &y, &w, &h);

    efl_assert_fail_if(!EINA_DBL_EQ(x, 0) || !EINA_DBL_EQ(y, 0) ||
                       !EINA_DBL_EQ(w, 100) || !EINA_DBL_EQ(h, 100));
}

TEST(bezier_test_on_interval)
{
    Eina_Bezier b, res;
    double      px1, px2, py1, py2;

    eina_bezier_values_set(&b, 0, 0, 10, 20, 20, 90, 30, 0);
    eina_bezier_point_at(&b, 0.25, &px1, &py1);
    eina_bezier_on_interval(&b, 0.25, 0.5, &res);
    eina_bezier_point_at(&res, 0, &px2, &py2);

    efl_assert_fail_if(!EINA_DBL_EQ(px1, px2) || !EINA_DBL_EQ(py1, py2));
}
