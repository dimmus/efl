dir_current_source = meson.current_build_dir()
dir_current_build  = meson.current_build_dir()

core_suite_src = [
  'ecore_suite.c',
  'ecore_test_ecore.c',
  'ecore_test_idle.c',
  'ecore_test_timer.c',
  'ecore_test_animator.c',
  'ecore_test_ecore_thread_eina_thread_queue.c',
  'ecore_test_job.c',
  'ecore_test_args.c',
  'ecore_test_pipe.c',
  'ecore_suite.h'
]

core_suite_src += [
  # 'ecore_test_ecore_imf.c', # need ecore_imf
  # 'ecore_test_ecore_evas.c', # need ecore_evas
  # 'ecore_test_ecore_evas_selection.c', # need ecore_evas
  # 'ecore_test_ecore_input.c', # need ecore_input
  # 'ecore_test_ecore_file.c', # need ecore_file
]

core_suite_deps = [ m ]
core_suite_deps += core
# core_suite_deps += ecore_file
# core_suite_deps += ecore_evas
# core_suite_deps += ecore_input
# core_suite_deps += ecore_imf

test_eo_files = [
  'ecore_audio_out_test.eo',
]
test_eo_file_target = []

foreach eo_file : test_eo_files
    test_eo_file_target += custom_target('eo_gen_' + eo_file,
    input :   eo_file,
    output :  eo_file + '.h',
    depfile : eo_file + '.d',
    install : false,
    command : efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

# if get_option('audio')
#   ecore_suite_deps += ecore_audio
#   ecore_suite_src += 'ecore_test_ecore_audio.c'
#   ecore_suite_src += test_eo_file_target
# endif

# if get_option('fb')
#   ecore_suite_deps += ecore_fb
#   ecore_suite_src += 'ecore_test_ecore_fb.c'
# endif

# if get_option('drm')
#   ecore_suite_deps += ecore_drm2
# endif

# if get_option('x11')
#   ecore_suite_src += 'ecore_test_ecore_x.c'
#   ecore_suite_deps += ecore_x
# endif

core_suite = executable('efl_core_suite',
  core_suite_src,
  dependencies: [ core_suite_deps, check ],
  c_args : ['-DTESTS_BUILD_DIR="'+dir_current_build+'"',
            '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

efl_app_suite_src = [
  'efl_app_suite.c',
  'efl_app_suite.h',
  'efl_app_test_loop.c',
  'efl_app_test_loop_fd.c',
  'efl_app_test_loop_timer.c',
  'efl_app_test_promise.c',
  'efl_app_test_env.c',
  'efl_app_test_cml.c',
]

priv_eo_files = [
   'efl_app_test_cml.eo',
]

priv_eo_file_target = []
foreach eo_file : priv_eo_files
  priv_eo_file_target += custom_target('eo_gen_' + eo_file,
    input :   eo_file,
    output :  eo_file + '.h',
    depfile : eo_file + '.d',
    command : efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

efl_app_suite_deps = [ m ]
efl_app_suite_deps += core

efl_app_suite = executable('efl_app_suite',
  efl_app_suite_src, priv_eo_file_target,
  dependencies: [ efl_app_suite_deps, check ],
  c_args : ['-DTESTS_BUILD_DIR="'+dir_current_build+'"',
            '-DTESTS_SRC_DIR="'+dir_current_source+'"']
)

test('efl_core-suite', core_suite,
  env : test_env,
  timeout : master_timeout
)

test('efl-app', efl_app_suite,
  env : test_env,
  timeout : master_timeout
)