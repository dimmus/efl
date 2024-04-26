dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

dep_dbus     = [ core ]
dep_dbus_pub = [ shared, eo, eo_interfaces]
dep_dbus_ext = [ dependency('dbus-1') ]

inc_dbus = include_directories('.')

pub_eo_files = [
  'eldbus_model_connection.eo',
  'eldbus_model_object.eo',
  'eldbus_model_proxy.eo',
  'eldbus_model_method.eo',
  'eldbus_model_arguments.eo',
  'eldbus_model_signal.eo',
  'eldbus_model.eo',
]

pub_eo_file_target = []
foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target('eo_gen_' + eo_file,
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
  'eldbus_types.eot'
]

foreach eo_file : pub_eo_types_files
  eo_file_target += custom_target('eo_gen_' + eo_file,
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

src_dbus_headers = [
  'Efl_Dbus.h',
  'eldbus_connection.h',
  'eldbus_freedesktop.h',
  'eldbus_message.h',
  'eldbus_object.h',
  'eldbus_pending.h',
  'eldbus_proxy.h',
  'eldbus_service.h',
  'eldbus_signal_handler.h',
  'eldbus_message_helper.h',
  'eldbus_introspection.h',
  'Eldbus_Model.h',
  'eldbus_message_eina_value.h'
]

src_dbus = files([
  'eldbus_private.h',
  'eldbus_private_types.h',
  'eldbus_model_private.h',
  'eldbus_model_proxy_private.h',
  'eldbus_model_object_private.h',
  'eldbus_model_arguments_private.h',
  'eldbus_model_connection_private.h',
  'eldbus_model_signal_private.h',
  'eldbus_model_method_private.h',
  'eldbus_proxy.c',
  'eldbus_core.c',
  'eldbus_message.c',
  'eldbus_object.c',
  'eldbus_pending.c',
  'eldbus_freedesktop.c',
  'eldbus_service.c',
  'eldbus_signal_handler.c',
  'eldbus_message_helper.c',
  'eldbus_message_to_eina_value.c',
  'eldbus_message_from_eina_value.c',
  'eldbus_model.c',
  'eldbus_model_connection.c',
  'eldbus_model_object.c',
  'eldbus_model_proxy.c',
  'eldbus_model_method.c',
  'eldbus_model_arguments.c',
  'eldbus_model_signal.c',
  'eldbus_introspection.c'
])

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