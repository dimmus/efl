test_efl_eo_interfaces_src = files(
  'efl_eo_interfaces_suite.c',
  'test_efl_eo_interfaces_composite_model.c',
  'test_efl_eo_interfaces_container_model.c',
)

efl_eo_interfaces_suite = executable('efl_eo_interfaces_suite',
  test_efl_eo_interfaces_src,
  dependencies: [ eo_interfaces, core, dep_check],
  c_args      : [ '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
                  '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"' ]
)

test('efl_eo_interfaces-suite', efl_eo_interfaces_suite,
  env     : test_env,
  timeout : master_timeout
)
