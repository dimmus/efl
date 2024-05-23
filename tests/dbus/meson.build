test_efl_dbus_src = files(
  'efl_dbus_suite.c',
  'test_efl_dbus_fake_server.c',
  'test_efl_dbus_init.c',
  'test_efl_dbus_model.c',
  'test_efl_dbus_model_connection.c',
  'test_efl_dbus_model_object.c',
  'test_efl_dbus_model_proxy.c',
  'test_efl_dbus_model_proxy_fake_server.c',
  'test_efl_dbus_model_method.c',
  'test_efl_dbus_model_signal.c',
  'test_efl_dbus_object.c',
  'test_efl_dbus_message.c',
  'test_efl_dbus_signal_handler.c',
  'test_efl_dbus_proxy.c',
  'test_efl_dbus_pending_cancel.c',
)

efl_dbus_suite = executable('efl_dbus_suite',
  test_efl_dbus_src,
  dependencies: [ dbus, core, dep_check ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+meson.current_build_dir()+'"',
                  '-DTESTS_SRC_DIR="'+meson.current_source_dir()+'"' ]
)

test('efl_dbus-suite', efl_dbus_suite,
  env     : test_env,
  timeout : master_timeout
)
