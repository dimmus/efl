dep_eo = []
dep_eo_pub = [etool]
dep_eo_ext = [valgrind, dl, execinfo]

inc_eo = [include_directories('.')]

pub_eo_files = [
  'efl_object.eo',
  'efl_class.eo',
  'efl_object_override.eo'
]

src_eo = files([
  'eo.c',
  'eo_ptr_indirection.c',
  'eo_ptr_indirection.h',
  'eo_base_class.c',
  'eo_class_class.c',
  'eo_add_fallback.c',
  'eo_add_fallback.h',
  'eo_private.h',
  'eo_internal.h'
])

pub_eo_types_files = [
  'eina_types.eot'
]

pub_eo_file_target = []
priv_eo_file_target = []

foreach eo_file : pub_eo_types_files
  pub_eo_file_target += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    depfile : eo_file + '.d',
    install : true,
    install_dir : dir_package_include,
    command : eolian_gen + [ '-I', dir_source, eolian_include_directories,
                           '-o', 'h:' + join_paths(dir_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_build, eo_file + '.d'),
                           '-e', 'EO_API',
                           '-gchd', '@INPUT@'])
endforeach

eo_header = ['Eo.h']

foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    depfile : eo_file + '.d',
    install : true,
    install_dir : dir_package_include,
    command : eolian_gen + [ '-I', dir_source, eolian_include_directories,
                           '-o', 'h:' + join_paths(dir_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_build, eo_file + '.d'),
                           '-e', 'EO_API',
                           '-gchd', '@INPUT@'])
endforeach

eolian_include_directories += ['-I', dir_source]

lib_eo = library('eo',
    src_eo, pub_eo_file_target,
    c_args : [package_c_args, '-DEO_BUILD'],
    dependencies: [dep_eo, dep_eo_pub, dep_eo_ext],
    install: true,
    version : version
)

lib_eo_dbg = library('eo_dbg',
    src_eo, pub_eo_file_target,
    c_args : [package_c_args, '-DEO_DEBUG', '-DEO_BUILD'],
    dependencies: [dep_eo, dep_eo_pub, dep_eo_ext],
    install: true,
    version : version
)

eo = declare_dependency(
  include_directories: [include_directories('.')],
  link_with: lib_eo,
  sources : pub_eo_file_target + priv_eo_file_target,
  dependencies: dep_eo + dep_eo_pub,
)

eo_dbg = declare_dependency(
  include_directories: [include_directories('.')],
  link_with: lib_eo_dbg,
  sources : pub_eo_file_target + priv_eo_file_target,
  dependencies: dep_eo + dep_eo_pub,
)

# if get_option('install-eo-files')
#   install_data(pub_eo_files + pub_eo_types_files,
#     install_dir: join_paths(eolian_include_dir, package_version_name)
#   )
# endif

install_headers(eo_header,
  install_dir : dir_package_include,
)

dep_eo_one = [dep_eo, dep_eo_pub, dep_eo_ext]