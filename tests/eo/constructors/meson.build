dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

eo_constructors_src = files(
  'constructors_main.c',
  'constructors_mixin.c',
  'constructors_mixin.h',
  'constructors_simple.c',
  'constructors_simple.h',
  'constructors_simple2.c',
  'constructors_simple2.h',
  'constructors_simple3.c',
  'constructors_simple3.h',
  'constructors_simple4.c',
  'constructors_simple4.h',
  'constructors_simple5.c',
  'constructors_simple5.h',
  'constructors_simple6.c',
  'constructors_simple6.h',
  'constructors_simple7.c',
  'constructors_simple7.h'
)

eo_constructors = executable('efl_eo_constructors',
  eo_constructors_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"'] )

test('efl_eo-constructors', eo_constructors,
  env : test_env
)

eo_constructors = executable('efl_eo_constructors_dbg',
  eo_constructors_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"'] )

test('efl_eo-constructors-dbg', eo_constructors,
  env : test_env
)
