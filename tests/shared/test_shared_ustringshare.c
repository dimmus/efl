/*
 * Copyright (C) 2008 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

static const Eina_Unicode TEST0[] = { 't', 'e', 's', 't', '/', '0', 0 };
static const Eina_Unicode TEST1[] = { 't', 'e', 's', 't', '/', '1', 0 };

TEST(ustringshare_simple)
{
    const Eina_Unicode *t0;
    const Eina_Unicode *t1;

    t0 = eina_ustringshare_add(TEST0);
    t1 = eina_ustringshare_add(TEST1);

    efl_assert_fail_if(t0 == NULL);
    efl_assert_fail_if(t1 == NULL);
    efl_assert_fail_if(eina_unicode_strcmp(t0, TEST0) != 0);
    efl_assert_fail_if(eina_unicode_strcmp(t1, TEST1) != 0);
    efl_assert_fail_if((int)eina_unicode_strlen(TEST0) !=
                       eina_ustringshare_strlen(t0));
    efl_assert_fail_if((int)eina_unicode_strlen(TEST1) !=
                       eina_ustringshare_strlen(t1));

    t0 = eina_ustringshare_ref(t0);
    efl_assert_fail_if(t0 == NULL);
    efl_assert_fail_if((int)strlen((char *)TEST0) !=
                       eina_stringshare_strlen((const char *)t0));

    eina_ustringshare_del(t0);
    eina_ustringshare_del(t0);
    eina_ustringshare_del(t1);
}

TEST(ustringshare_test_share)
{
    const Eina_Unicode *t0;
    const Eina_Unicode *t1;

    t0 = eina_ustringshare_add(TEST0);
    t1 = eina_ustringshare_add(TEST0);

    efl_assert_fail_if(t0 == NULL);
    efl_assert_fail_if(t1 == NULL);
    efl_assert_fail_if(eina_unicode_strcmp(t0, TEST0) != 0);
    efl_assert_fail_if(eina_unicode_strcmp(t1, TEST0) != 0);
    efl_assert_fail_if(t0 != t1);
    efl_assert_fail_if((int)eina_unicode_strlen(TEST0) !=
                       eina_ustringshare_strlen(t0));
    efl_assert_fail_if((int)eina_unicode_strlen(TEST0) !=
                       eina_ustringshare_strlen(t1));

    eina_ustringshare_del(t0);
    eina_ustringshare_del(t1);
}

TEST(ustringshare_putstuff)
{
    const Eina_Unicode *tmp;
    int                 i;

    for (i = 100; i > 0; --i)
    {
        Eina_Unicode string_[] = { 's', 't', 'r', 'i', 'n', 'g', '_', 0 };
        Eina_Unicode build[64];
        eina_unicode_strcpy(build, string_);

        build[7] = i;
        build[8] = 0;
        tmp      = eina_ustringshare_add(build);
        efl_assert_fail_if(tmp != eina_ustringshare_add(build));
        efl_assert_fail_if((int)eina_unicode_strlen(build) !=
                           eina_ustringshare_strlen(tmp));
    }
}
