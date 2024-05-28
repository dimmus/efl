dep_eo_pub = [ shared ]
dep_eo_ext = [ dep_valgrind, dep_libdl, execinfo ]

inc_eo = include_directories('.')

dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

pub_eo_files = [
  'efl_object.eo',
  'efl_class.eo',
  'efl_object_override.eo'
]

src_eo = files(
  'eo.c',
  'eo_ptr_indirection.c',
  'eo_base_class.c',
  'eo_class_class.c',
  'eo_add_fallback.c',
)

pub_eo_types_files = [
  'eina_types.eot'
]

pub_eo_file_target = []
foreach eo_file : pub_eo_types_files
  pub_eo_file_target += custom_target(eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : dir_package_include,
    command :     efl_eo_gen + ['-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-e', 'EO_API',
                           '-gchd', '@INPUT@']
  )
endforeach

eo_header = files('Efl_Eo.h')

foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target(eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : dir_package_include,
    command :     efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-e', 'EO_API',
                           '-gchd', '@INPUT@'])
endforeach

eo_gen_include_directories += ['-I', dir_source]

lib_eo = library('efl_eo',
    src_eo, pub_eo_file_target,
    c_args :            [ package_c_args, '-DEO_BUILD' ],
    dependencies:       [ dep_eo_pub, dep_eo_ext ],
    install:            true,
    version :           version
)

lib_eo_dbg = library('efl_eo_dbg',
    src_eo, pub_eo_file_target,
    c_args :            [ package_c_args, '-DEO_DEBUG', '-DEO_BUILD' ],
    dependencies:       [ dep_eo_pub, dep_eo_ext ],
    install:            true,
    version :           version
)

eo = declare_dependency(
  include_directories:  inc_eo,
  link_with:            lib_eo,
  sources :             pub_eo_file_target,
  dependencies:         dep_eo_pub,
)

eo_dbg = declare_dependency(
  include_directories:  inc_eo,
  link_with:            lib_eo_dbg,
  sources :             pub_eo_file_target,
  dependencies:         dep_eo_pub,
)

# if get_option('install-eo-files')
#   install_data(pub_eo_files + pub_eo_types_files,
#     install_dir: join_paths(eo_include_dir, package_version_name)
#   )
# endif

install_headers(eo_header,
  install_dir :         dir_package_include,
)

dep_eo = dep_eo_pub + dep_eo_ext
dep_eo_one = dep_eo