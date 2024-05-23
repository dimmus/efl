#ifndef EINA_SUITE_X
#define EINA_SUITE_X

#define EINA_TEST_START(Func)                   \
  START_TEST(Func)                              \
  {                                             \
    eina_init();                                \
    do                                          \
      {


#define EINA_TEST_END                           \
    } while(0);                                 \
    eina_shutdown();                            \
  }                                             \
  END_TEST

#define EINA_TEST_CODE

#include "test_efl_shared_stringshare.c"
#include "test_efl_shared_util.c"

#undef EINA_TEST_START
#undef EINA_TEST_END
#undef EINA_TEST_CODE

#define EINA_TEST_START(Func)                   \
  tcase_add_test(tc, Func);                     \
  while (0)                                     \
    {

#define EINA_TEST_END                           \
  }

static void
eina_test_stringshare(TCase *tc)
{
#include "test_efl_shared_stringshare.c"
}

static void
eina_test_util(TCase *tc)
{
#include "test_efl_shared_util.c"
}

#endif
