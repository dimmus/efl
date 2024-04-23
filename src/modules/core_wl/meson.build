core_wl2_engines = [
  'dmabuf'
]

foreach engine : core_wl2_engines
   mod_install_dir = join_paths(dir_lib, 'core_wl2/engines', engine, version_name)
   subdir(join_paths('engines', engine))
   module_files += join_paths(mod_install_dir, 'libefl_core_wl_engines_'+engine+'.'+sys_mod_extension)
endforeach
