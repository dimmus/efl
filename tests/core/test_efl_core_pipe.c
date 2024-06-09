#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>
#include "efl_core_suite.h"

static void
_pipe_handler(void *data, void *buf EFL_UNUSED, unsigned int len EFL_UNUSED)
{
  Efl_Bool *bob = data;

  *bob = EFL_TRUE;
  core_main_loop_quit();
}

EFL_START_TEST(core_test_pipe)
{
  Core_Pipe *pipe;
  Efl_Bool   bob = EFL_FALSE;

  pipe = core_pipe_add(_pipe_handler, &bob);
  fail_if(!pipe);

  core_pipe_write(pipe, &bob, sizeof(Efl_Bool));
  core_main_loop_begin();
  ck_assert_int_eq(bob, EFL_TRUE);

  bob = EFL_FALSE;
  core_pipe_write(pipe, NULL, 0);
  core_main_loop_begin();
  ck_assert_int_eq(bob, EFL_TRUE);

  core_pipe_del(pipe);
}

EFL_END_TEST

void
core_test_core_pipe(TCase *tc)
{
  tcase_add_test(tc, core_test_pipe);
}
