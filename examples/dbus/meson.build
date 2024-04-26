dbus_example_files = [
  'connman-list-services',
  'ofono-dial',
  'banshee',
  'complex-types',
  'complex-types-server',
  'server',
  'client',
  'simple-signal-emit',
  'complex-types-client-eina-value',
  'connect-address',
  'dbusmodel',
]

foreach example_file : dbus_example_files
  executable(example_file, example_file + '.c', dependencies: [ dbus, core ])
endforeach
