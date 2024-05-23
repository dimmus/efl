dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

eo_function_overrides_src = files(
  'function_overrides_inherit.c',
  'function_overrides_inherit.h',
  'function_overrides_inherit2.c',
  'function_overrides_inherit2.h',
  'function_overrides_inherit3.c',
  'function_overrides_inherit3.h',
  'function_overrides_main.c',
  'function_overrides_simple.c',
  'function_overrides_simple.h'
)

eo_function_overrides = executable('efl_eo_function_overrides',
  eo_function_overrides_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-function-overrides', eo_interface,
  env : test_env
)

eo_function_overrides = executable('efl_eo_function_overrides_dbg',
  eo_function_overrides_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-function-overrides-dbg', eo_interface,
  env : test_env
)
