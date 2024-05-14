# eo generation
dir_current_source = meson.current_build_dir()
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

# test
test_source = files(
	'eolian_aux.c',
	'eolian_generated_future.c',
	# 'eolian_generation.c', # TODO: bug here
	'eolian_parsing.c',
	'eolian_static.c'
)

tests += [
	{	
		'name'    : 'eo_gen',
		'sources' : test_source + priv_eo_file_target,
		'dep_objs': [ eo_gen, eo ],
		'inc_dirs': [ inc_shared, inc_eo_gen ],
		'c_args'  : [
			'-DEOLIAN_GEN="' + eo_gen_path + '"',
			'-DEO_SRC_DIR="' + join_paths(meson.source_root(), 'src/lib') + '"',
		]
	},
]