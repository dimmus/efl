/*
 * Copyright (C) 2008 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Efl_Shared.h>

#include "efl-test-runner.h"

static void
_eina_mempool_test(Eina_Mempool *mp,
                   Eina_Bool with_realloc, Eina_Bool with_gc, Eina_Bool accurate_from)
{
   Eina_Iterator *it;
   int *tbl[512];
   int *ptr;
   int i;

   efl_assert_fail_if(!mp);

   for (i = 0; i < 512; ++i)
     {
        tbl[i] = eina_mempool_malloc(mp, sizeof (int));
        efl_assert_fail_if(!tbl[i]);
        if (accurate_from)
          efl_assert_fail_if(eina_mempool_from(mp, tbl[i]) != EINA_TRUE);
        *tbl[i] = i;
     }

   for (i = 0; i < 512; ++i)
        efl_assert_fail_if(*tbl[i] != i);

   for (i = 0; i < 256; ++i)
     {
        eina_mempool_free(mp, tbl[i]);
        if (accurate_from)
          efl_assert_fail_if(eina_mempool_from(mp, tbl[i]) != EINA_FALSE);
     }

   it = eina_mempool_iterator_new(mp);
   EINA_ITERATOR_FOREACH(it, ptr)
     {
        ck_assert_int_gt(*ptr, 255);
        *ptr = 0;
     }
   eina_iterator_free(it);

   if (it) // Only check if the mempool support iterator
     {
        for (; i < 512; ++i)
          ck_assert_int_eq(*tbl[i], 0);
     }

   if (with_realloc)
      efl_assert_fail_if(eina_mempool_realloc(mp, tbl[500], 25) == NULL);
   else
      efl_assert_fail_if(eina_mempool_realloc(mp, tbl[500], 25) != NULL);

   if (with_gc)
     {
        eina_mempool_gc(mp);
        eina_mempool_statistics(mp);
     }

   eina_mempool_del(mp);
}

#ifdef EINA_BUILD_CHAINED_POOL
TEST(mempool_chained_mempool)
{
   Eina_Mempool *mp;

   mp = eina_mempool_add("chained_mempool", "test", NULL, sizeof (int), 256);
   _eina_mempool_test(mp, EINA_FALSE, EINA_FALSE, EINA_TRUE);
}
#endif

#ifdef EINA_BUILD_PASS_THROUGH
TEST(mempool_pass_through)
{
   Eina_Mempool *mp;

   mp = eina_mempool_add("pass_through", "test", NULL, sizeof (int), 8, 0);
   _eina_mempool_test(mp, EINA_TRUE, EINA_FALSE, EINA_FALSE);
}
#endif

#ifdef EINA_BUILD_ONE_BIG
TEST(mempool_one_big)
{
   Eina_Mempool *mp;

   mp = eina_mempool_add("one_big", "test", NULL, sizeof (int), 384);
   _eina_mempool_test(mp, EINA_FALSE, EINA_FALSE, EINA_TRUE);
}
#endif

void
eina_test_mempool(TCase *tc)
{
#ifdef EINA_BUILD_CHAINED_POOL
   tcase_add_test(tc, eina_mempool_chained_mempool);
#endif
#ifdef EINA_BUILD_PASS_THROUGH
   tcase_add_test(tc, eina_mempool_pass_through);
#endif
#ifdef EINA_BUILD_ONE_BIG
   tcase_add_test(tc, eina_mempool_one_big);
#endif
}
