src_efl_core_wayland_suite = [
  'efl_core_wayland_suite.c',
  'test_efl_core_wayland_session.c',
  'test_efl_core_wayland_display.c',
  'test_efl_core_wayland_window.c',
  'test_efl_core_wayland_input.c',
  'test_efl_core_wayland_output.c'
]

dep_efl_core_wayland_suite = []

if get_option('opengl') == 'es-egl'
    src_efl_core_wayland_suite += 'efl_core_wayland_tests_helper_egl.h'
    dep_efl_core_wayland_suite += dependency('egl')
    dep_efl_core_wayland_suite += dependency('gl')
endif

efl_core_wayland_suite = executable('efl_core_wayland_suite',
  src_efl_core_wayland_suite,
  dependencies: [ core_wayland, core, core_input, 
                  dep_check, dep_wayland, dependency('wayland-egl'), 
                  dep_efl_core_wayland_suite ],
  c_args : [ '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
             '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"']
)

test('efl_core_wayland-suite', efl_core_wayland_suite,
  env : test_env,
  timeout : master_timeout
)
