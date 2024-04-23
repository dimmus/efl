libdrm = dependency('libdrm', required: true)
subdir('protocols')

# core_wl_deps = [ core, core_input, buildsystem ]
dep_core_wl     = [ core, core_input ]
dep_core_wl_pub = [ core, shared ]
dep_core_wl_ext = [ dependency('wayland-client'), dependency('wayland-server'), dependency('xkbcommon'),
                    wayland_protocol, libdrm, dl, m]
# core_wl_ext_deps = [dependency('wayland-client'), dependency('wayland-server'), dependency('xkbcommon'),
#   wayland_protocol, libdrm, buildsystem_simple, dl, m]

inc_core_wl = include_directories('.')

src_core_wl_header = [
  'Ecore_Wl.h'
]

src_core_wl = files([
  'ecore_wl_internal.h',
  'ecore_wl_subsurf.c',
  'ecore_wl_dnd.c',
  'ecore_wl_window.c',
  'ecore_wl_input.c',
  'ecore_wl_output.c',
  'ecore_wl_display.c',
  'ecore_wl.c',
  'ecore_wl_private.h',
  'ecore_wl_buffer.c',
  'ecore_wl_surface.c'
])

lib_core_wl = library('efl_core_wl',
    src_core_wl, pub_eo_file_target,
    dependencies:         [ dep_core_wl, dep_core_wl_pub, dep_core_wl_ext ],
    include_directories : inc_config,
    c_args :              package_c_args,
    install:              true,
    version :             version
)

core_wl = declare_dependency(
  include_directories:    inc_core_wl,
  link_with:              lib_core_wl,
  sources :               pub_eo_file_target + priv_eo_file_target,
  dependencies:           dep_core_wl_pub,
)

core_wl_internal_headers = declare_dependency(
  include_directories:    inc_core_wl,
  dependencies:           [ wayland_protocol_simple, libdrm, \
                            dependency('wayland-client'), \
                            dependency('wayland-server'), \
                            dependency('xkbcommon') ],
)

install_headers(src_core_wl_header,
  install_dir :           dir_package_include,
)

dep_core_wl_one = dep_core_wl + dep_core_wl_pub + dep_core_wl_ext