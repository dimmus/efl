#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>

#include <Core_Input.h>

#include "efl_core_suite.h"

#define MAX_ITER 10

EFL_START_TEST(core_test_core_input_init)
{
  int ret, i, j;

  for (i = 1; i <= MAX_ITER; i++)
  {
    ret = core_event_init();
    fail_if(ret != i);
  }

  for (j = MAX_ITER - 1; j >= 0; j--)
  {
    ret = core_event_shutdown();
    fail_if(ret != j);
  }
}

EFL_END_TEST

void
core_test_core_input(TCase *tc)
{
  tcase_add_test(tc, core_test_core_input_init);
}
