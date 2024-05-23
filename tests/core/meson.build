dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

test_efl_core_src = files(
  'efl_core_suite.c',
  'test_efl_core_main_loop.c',
  'test_efl_core_idle.c',
  'test_efl_core_timer.c',
  'test_efl_core_animator.c',
  'test_efl_core_thread_eina_thread_queue.c',
  'test_efl_core_job.c',
  'test_efl_core_args.c',
  'test_efl_core_pipe.c',
  # 'test_efl_core_input.c',
  # 'test_efl_core_imf.c',
  # 'test_efl_core_file.c',
  # 'test_efl_core_evas.c',
  # 'test_efl_core_evas_selection.c',
)

test_efl_core_deps = [ dep_libm ]
test_efl_core_deps += core
# test_efl_core_deps += core_file
# test_efl_core_deps += core_evas
# test_efl_core_deps += core_input
# test_efl_core_deps += core_imf

test_eo_files = [
  'efl_core_audio_out_test.eo',
]

test_eo_file_target = []
foreach eo_file : test_eo_files
    test_eo_file_target += custom_target('eo_gen_' + eo_file,
    input   : eo_file,
    output  : eo_file + '.h',
    depfile : eo_file + '.d',
    install : false,
    command : efl_eo_gen + ['-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

# if get_option('audio')
#   test_efl_core_deps += core_audio
#   test_efl_core_src += 'core_test_core_audio.c'
#   test_efl_core_src += test_eo_file_target
# endif

# if get_option('fb')
#   test_efl_core_deps += core_fb
#   test_efl_core_src += 'core_test_core_fb.c'
# endif

# if get_option('drm')
#   test_efl_core_deps += core_drm2
# endif

# if get_option('x11')
#   test_efl_core_src += 'core_test_core_x.c'
#   test_efl_core_deps += core_x
# endif

core_suite = executable('efl_core_suite',
  test_efl_core_src,
  dependencies: [ test_efl_core_deps, dep_check ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
                  '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"']
)

test_efl_app_src = [
  'efl_app_suite.c',
  'test_efl_app_loop.c',
  'test_efl_app_loop_fd.c',
  'test_efl_app_loop_timer.c',
  'test_efl_app_promise.c',
  'test_efl_app_env.c',
  'test_efl_app_cml.c',
]

priv_eo_files = [
   'efl_app_test_cml.eo',
]

priv_eo_file_target = []
foreach eo_file : priv_eo_files
  priv_eo_file_target += custom_target('eo_gen_' + eo_file,
    input   : eo_file,
    output  : eo_file + '.h',
    depfile : eo_file + '.d',
    command : efl_eo_gen + ['-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

test_efl_app_deps  = [ dep_libm ]
test_efl_app_deps += core

efl_app_suite = executable('efl_app_suite',
  test_efl_app_src, priv_eo_file_target,
  dependencies: [ test_efl_app_deps, dep_check ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_core-suite', core_suite,
  env     : test_env,
  timeout : master_timeout
)

test('efl_app-suite', efl_app_suite,
  env : test_env,
  timeout : master_timeout
)
