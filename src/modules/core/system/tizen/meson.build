core_systemd_mod = shared_module('tizen',
    'ecore_system_tizen.c',
    dependencies: [ core, eldbus ],
    install:      true,
    install_dir:  mod_install_dir,
    name_suffix : sys_mod_extension
)
module_files += join_paths(mod_install_dir, 'libtizen.' + sys_mod_extension)
