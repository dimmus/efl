dir_current_source = meson.current_build_dir()
dir_current_build  = meson.current_build_dir()

eo_interfaces_suite_src = [
  'efl_suite.c',
  'efl_suite.h',
  'efl_test_composite_model.c',
  'efl_test_container_model.c',
]

eo_interfaces_suite_bin = executable('efl_eo_interfaces_suite',
  eo_interfaces_suite_src,
  dependencies: [ eo_interfaces, check ],
  c_args : ['-DTESTS_BUILD_DIR="'+dir_current_build+'"',
            '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo_interfaces-suite', eo_interfaces_suite_bin,
  env : test_env,
  timeout : master_timeout
)
