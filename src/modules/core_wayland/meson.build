core_wl_engines = [
  'dmabuf'
]

foreach engine : core_wl_engines
   mod_install_dir = join_paths(dir_lib, 'core_wayland/engines', engine, version_name)
   subdir(join_paths('engines', engine))
   module_files += join_paths(mod_install_dir, 'lib'+engine+'.'+sys_mod_extension)
endforeach
