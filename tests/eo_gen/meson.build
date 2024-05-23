dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

priv_eo_files = [
   'generated_future.eo'
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
                           '-e', 'EOTEST_API',
                           '-gchd', '@INPUT@'])
endforeach

test_efl_eo_gen_src = files(
  'efl_eo_gen_suite.c',
  'test_efl_eo_gen_parsing.c',
  'test_efl_eo_gen_aux.c',
  'test_efl_eo_gen_generation.c',
  'test_efl_eo_gen_generated_future.c',
  'test_efl_eo_gen_static.c'
)

efl_eo_gen_suite = executable('efl_eo_gen_suite',
  test_efl_eo_gen_src, priv_eo_file_target,
  dependencies: [ eo_gen, eo, dep_check],
  c_args      : [ '-DEOLIAN_GEN="'+eo_gen_path+'"',
                  '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DEO_SRC_DIR="'+join_paths(meson.project_source_root(), 'src/lib')+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"' ]
)

test('efl_eo_gen-suite', efl_eo_gen_suite,
  env : test_env
)
