dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

eo_composite_objects_src = files(
  'composite_objects_comp.c',
  'composite_objects_comp.h',
  'composite_objects_main.c',
  'composite_objects_simple.c',
  'composite_objects_simple.h'
)

eo_composite_objects = executable('efl_eo_composite_objects',
  eo_composite_objects_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"'] )

test('efl_eo-composite-objects', eo_composite_objects,
  env : test_env
)

eo_composite_objects = executable('efl_eo_composite_objects_dbg',
  eo_composite_objects_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"'] )

test('efl_eo-composite-objects-dbg', eo_composite_objects,
  env : test_env
)
