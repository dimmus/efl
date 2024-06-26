dep_libdrm         = dependency('libdrm', required: true)
dep_libxkb         = dependency('xkbcommon')
dep_wayland_server = dependency('wayland-server', native: true, version: '>= 1.20.0')
dep_wayland_client = dependency('wayland-client', native: true, version: '>= 1.20.0')

efl_config.set('EFL_WAYLAND_VERSION', dep_wayland_server.version().split('.')[1])

dep_wayland = [ dep_wayland_client, dep_wayland_server ]

subdir('protocols')
dep_wayland += wayland_protocol

dep_core_wayland     = [ core, core_input ]
dep_core_wayland_pub = [ core, shared ]
dep_core_wayland_ext = [ dep_wayland, dep_libxkb, dep_libdrm, dep_libdl, dep_libm]

inc_core_wayland = include_directories('.')

src_core_wayland_header = [
  'Efl_Core_Wl.h'
]

src_core_wayland = files(
  'core_wayland_subsurf.c',
  'core_wayland_dnd.c',
  'core_wayland_window.c',
  'core_wayland_input.c',
  'core_wayland_output.c',
  'core_wayland_display.c',
  'core_wayland.c',
  'core_wayland_buffer.c',
  'core_wayland_surface.c'
)

lib_core_wayland = library('efl_core_wayland',
  src_core_wayland, pub_eo_file_target,
  dependencies:         [ dep_core_wayland, dep_core_wayland_pub, dep_core_wayland_ext ],
  include_directories : inc_config,
  c_args :              package_c_args,
  install:              true,
  version :             version
)

core_wayland = declare_dependency(
  include_directories:  inc_core_wayland,
  link_with:            lib_core_wayland,
  sources :             pub_eo_file_target + priv_eo_file_target,
  dependencies:         dep_core_wayland_pub,
)

install_headers(src_core_wayland_header,
  install_dir :         dir_package_include,
)

dep_core_wayland_one = dep_core_wayland + dep_core_wayland_pub + dep_core_wayland_ext