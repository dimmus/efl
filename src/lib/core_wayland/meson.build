libdrm = dependency('libdrm', required: true)

subdir('protocols')

dep_core_wayland     = [ core, core_input ]
dep_core_wayland_pub = [ core, shared ]
dep_core_wayland_ext = [ dependency('wayland-client'), dependency('wayland-server'), dependency('xkbcommon'),
                         wayland_protocol, libdrm, dep_libdl, dep_libm]

inc_core_wayland = include_directories('.')

src_core_wayland_header = [
  'Efl_Core_Wl.h'
]

src_core_wayland = files(
  'core_wl_subsurf.c',
  'core_wl_dnd.c',
  'core_wl_window.c',
  'core_wl_input.c',
  'core_wl_output.c',
  'core_wl_display.c',
  'core_wl.c',
  'core_wl_buffer.c',
  'core_wl_surface.c'
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
  include_directories:    inc_core_wayland,
  link_with:              lib_core_wayland,
  sources :               pub_eo_file_target + priv_eo_file_target,
  dependencies:           dep_core_wayland_pub,
)

core_wl_internal_headers = declare_dependency(
  include_directories:    inc_core_wayland,
  dependencies:           [ wayland_protocol_simple, libdrm, \
                            dependency('wayland-client'), \
                            dependency('wayland-server'), \
                            dependency('xkbcommon') ],
)

install_headers(src_core_wayland_header,
  install_dir :           dir_package_include,
)

dep_core_wayland_one = dep_core_wayland + dep_core_wayland_pub + dep_core_wayland_ext