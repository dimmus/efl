dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

eo_access_src = files(
  'access_inherit.c',
  'access_inherit.h',
  'access_main.c',
  'access_simple.c',
  'access_simple.h',
  'access_simple_protected.h'
)

eo_access = executable('efl_eo_access',
  eo_access_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-access', eo_access,
  env : test_env
)

eo_access = executable('efl_eo_access_dbg',
  eo_access_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-access-dbg', eo_access,
  env : test_env
)
