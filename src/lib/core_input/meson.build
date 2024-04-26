dep_core_input     = [ core ]
dep_core_input_pub = [ shared, eo ]
dep_core_input_ext = []

inc_core_input = include_directories('.')

src_core_input_header = [
  'Efl_Core_Input.h'
]

src_core_input = files([
  'ecore_input.c',
  'ecore_input_compose.c',
  'ecore_input_joystick.c',
  'ecore_input_compose.h',
  'ecore_input_private.h'
])

# if get_option('eeze')
#   core_input_deps += eeze
# endif

lib_core_input = library('efl_core_input',
    src_core_input, pub_eo_file_target,
    c_args :              package_c_args,
    dependencies:         dep_core_input_pub + dep_core_input + dep_core_input_ext,
    include_directories : inc_config,
    install:              true,
    version :             version
)

core_input = declare_dependency(
  include_directories:    inc_core_input,
  link_with:              lib_core_input,
  sources :               pub_eo_file_target + priv_eo_file_target,
  dependencies:           dep_core_input_pub,
)

install_headers(src_core_input_header,
  install_dir :           dir_package_include,
)

dep_core_input_one = dep_core_input + dep_core_input_pub + dep_core_input_ext