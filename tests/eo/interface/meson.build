eo_interface_src = files(
  'interface_interface.c',
  'interface_interface.h',
  'interface_interface2.c',
  'interface_interface2.h',
  'interface_main.c',
  'interface_simple.c',
  'interface_simple.h'
)

eo_interface = executable('efl_eo_interface',
  eo_interface_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-interface', eo_interface,
  env : test_env
)

eo_interface = executable('efl_eo_interface_dbg',
  eo_interface_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_eo-interface-dbg', eo_interface,
  env : test_env
)
