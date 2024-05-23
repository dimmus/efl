dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

eo_mixin_src = files(
  'mixin_inherit.c',
  'mixin_inherit.h',
  'mixin_main.c',
  'mixin_mixin.c',
  'mixin_mixin.h',
  'mixin_mixin2.c',
  'mixin_mixin2.h',
  'mixin_mixin3.c',
  'mixin_mixin3.h',
  'mixin_mixin4.c',
  'mixin_mixin4.h',
  'mixin_simple.c',
  'mixin_simple.h'
)

eo_mixin = executable('efl_eo_mixin',
  eo_mixin_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-mixins', eo_mixin,
  env : test_env
)

eo_mixin = executable('efl_eo_mixin_dbg',
  eo_mixin_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-mixins-dbg', eo_mixin,
  env : test_env
)
