dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

eo_children_src = files(
  'children_main.c',
  'children_simple.c',
  'children_simple.h'
)

eo_children = executable('efl_eo_children',
  eo_children_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-children', eo_children,
  env : test_env
)

eo_children = executable('efl_eo_children_dbg',
  eo_children_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-children-dbg', eo_children,
  env : test_env
)
