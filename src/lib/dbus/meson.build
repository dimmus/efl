dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

dep_dbus     = [ core ]
dep_dbus_pub = [ shared, eo, eo_interfaces]
dep_dbus_ext = [ dependency('dbus-1') ]

inc_dbus = include_directories('.')

pub_eo_files = [
  'efl_dbus_model_arguments.eo',
  'efl_dbus_model_connection.eo',
  'efl_dbus_model_method.eo',
  'efl_dbus_model_object.eo',
  'efl_dbus_model_proxy.eo',
  'efl_dbus_model_signal.eo',
  'efl_dbus_model.eo',
]

pub_eo_file_target = []
foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target(eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : dir_package_include,
    command:      efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

pub_eo_types_files = [
  'efl_dbus_types.eot'
]

foreach eo_file : pub_eo_types_files
  eo_file_target += custom_target(eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    depfile : eo_file + '.d',
    install : true,
    install_dir : dir_package_include,
    command : efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-ghd', '@INPUT@'])
endforeach


eo_gen_include_directories += ['-I', dir_current_source]

src_dbus_headers = files(
  'Efl_Dbus.h',
  'Efl_Dbus_Model.h',
  'efl_dbus_connection.h',
  'efl_dbus_freedesktop.h',
  'efl_dbus_message.h',
  'efl_dbus_object.h',
  'efl_dbus_pending.h',
  'efl_dbus_proxy.h',
  'efl_dbus_service.h',
  'efl_dbus_signal_handler.h',
  'efl_dbus_message_helper.h',
  'efl_dbus_introspection.h',
  'efl_dbus_message_eina_value.h'
)

src_dbus = files(
  'efl_dbus_proxy.c',
  'efl_dbus_core.c',
  'efl_dbus_message.c',
  'efl_dbus_object.c',
  'efl_dbus_pending.c',
  'efl_dbus_freedesktop.c',
  'efl_dbus_service.c',
  'efl_dbus_signal_handler.c',
  'efl_dbus_message_helper.c',
  'efl_dbus_message_to_eina_value.c',
  'efl_dbus_message_from_eina_value.c',
  'efl_dbus_model.c',
  'efl_dbus_model_connection.c',
  'efl_dbus_model_object.c',
  'efl_dbus_model_proxy.c',
  'efl_dbus_model_method.c',
  'efl_dbus_model_arguments.c',
  'efl_dbus_model_signal.c',
  'efl_dbus_introspection.c'
)

lib_dbus = library('efl_dbus',
    src_dbus, pub_eo_file_target,
    c_args :              package_c_args,
    dependencies:         dep_dbus_pub + dep_dbus + dep_dbus_ext,
    include_directories : inc_config,
    install:              true,
    version :             version
)

dbus = declare_dependency(
  include_directories:  inc_dbus,
  link_with:            lib_dbus,
  sources :             pub_eo_file_target + priv_eo_file_target,
  dependencies:         dep_dbus_pub,
)

#
# Only enable that again when the namespace problems are fixed. ref T8648
#
#if get_option('install-eo-files')
#  install_data(pub_eo_files + pub_eo_types_files,
#    install_dir: join_paths(eolian_include_dir, package_version_name)
#  )
#endif

install_headers(src_dbus_headers,
  install_dir : dir_package_include,
)

dep_dbus_one = dep_dbus_ext + dep_dbus_pub + dep_dbus