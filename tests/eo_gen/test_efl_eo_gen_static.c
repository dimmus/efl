#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <Efl_Eo_Gen.h>

#include "efl_eo_gen_suite.h"

EFL_START_TEST(eolian_static_check)
{
   Eolian_State *eos1 = eolian_state_new();
   fail_if(!eolian_state_directory_add(eos1, TESTS_SRC_DIR "/data"));
   fail_if(!eolian_state_all_eot_files_parse(eos1));
   eolian_state_free(eos1);

   Eolian_State *eos2 = eolian_state_new();
   fail_if(!eolian_state_directory_add(eos2, TESTS_SRC_DIR "/data_aux"));
   fail_if(!eolian_state_all_eo_files_parse(eos2));

   /* too many failures to enable this yet */
   fail_if(!eolian_state_check(eos2));

   eolian_state_free(eos2);
}
EFL_END_TEST

void eolian_static_test(TCase *tc)
{
   tcase_add_test(tc, eolian_static_check);
}
