core_wl_suite_src = [
  'core_wl_suite.c',
  'core_wl_suite.h',
  'core_wl_tests_helpers.h',
  'core_wl_test_ecore_wl2.c',
  'core_wl_test_display.c',
  'core_wl_test_window.c',
  'core_wl_test_input.c',
  'core_wl_test_output.c'
]

wl_test_gl_deps = []

if get_option('opengl') == 'es-egl'
    core_wl_suite_src += 'core_wl_tests_helper_egl.h'
    wl_test_gl_deps += dependency('egl')
    wl_test_gl_deps += dependency('gl')
endif

core_wl_suite = executable('efl_core_wl_suite',
  core_wl_suite_src,
  dependencies: [ core_wl, core, core_input, check, wayland_protocol, dependency('wayland-client'), dependency('wayland-egl'), wl_test_gl_deps],
  c_args : ['-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
            '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"']
)

test('efl_core_wl-suite', core_wl_suite,
  env : test_env,
  timeout : master_timeout
)
