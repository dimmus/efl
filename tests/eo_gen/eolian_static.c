#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Eo_Gen.h>

#include "efl-test-runner.h"

TEST(eolian_static_check)
{
    Eolian_State *eos1 = eolian_state_new();
    efl_assert_fail_if(!eolian_state_directory_add(eos1, TESTS_SRC_DIR "/data"));
    efl_assert_fail_if(!eolian_state_all_eot_files_parse(eos1));
    eolian_state_free(eos1);

    Eolian_State *eos2 = eolian_state_new();
    efl_assert_fail_if(!eolian_state_directory_add(eos2, TESTS_SRC_DIR "/data_aux"));
    efl_assert_fail_if(!eolian_state_all_eo_files_parse(eos2));

      /* too many failures to enable this yet */
    efl_assert_fail_if(!eolian_state_check(eos2));

    eolian_state_free(eos2);
}
