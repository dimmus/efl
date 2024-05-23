eo_suite_src = files(
  'eo_test_class_simple.c',
  'eo_test_class_simple.h',
  'eo_test_class_singleton.c',
  'eo_test_class_singleton.h',
  'eo_test_domain.c',
  'eo_test_domain.h',
  'eo_suite.c',
  'eo_suite.h',
  'eo_error_msgs.h',
  'eo_error_msgs.c',
  'eo_test_class_errors.c',
  'eo_test_class_behaviour_errors.c',
  'eo_test_call_errors.c',
  'eo_test_general.c',
  'eo_test_value.c',
  'eo_test_event.c',
  'eo_test_threaded_calls.c',
  'eo_test_init.c',
  'eo_test_lifecycle.c',
  'eo_test_reflection.c'
)

eo_suite = executable('efl_eo_suite',
  eo_suite_src,
  dependencies: [eo, dep_check, valgrind],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-suite', eo_suite,
  env : test_env,
  timeout : master_timeout
)

eo_suite = executable('efl_eo_suite_dbg',
  eo_suite_src,
  dependencies: [eo_dbg, dep_check, valgrind],
  c_args : [
  '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
  '-DEO_DEBUG=1',
  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-suite-dbg', eo_suite,
  env : test_env,
  timeout : master_timeout
)

eo_suite = executable('efl_eo_suite_fallback',
  eo_suite_src,
  dependencies: [eo_dbg, dep_check, valgrind],
  c_args : [
  '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
  '-DEO_DEBUG=1',
  '-D_EO_ADD_FALLBACK_FORCE=1',
  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-suite-fallback', eo_suite,
  env : test_env,
  timeout : master_timeout
)
