if get_option('efl-one')
  lib_efl_one = shared_library('efl-one',
    link_whole : efl_one_parts,
    install : true,
    version : version,
  )

  efl_one = declare_dependency(
    link_with: lib_efl_one,
    include_directories : inc_efl_one,
    dependencies : [thread_dep, intl] + dep_efl_one_eo,
    version: version
  )

  pkgconfig.generate(
    name : 'efl-one',
    subdirs : sub_efl_one,
    description: 'Configureation for efl in one big fat .so',
    libraries : [efl_one] + dep_etool, #eina is a special case here which drags in m dl & threads
  )

  #overwrite all the dependencies of subprojects with efl-one, in order to link the modules and binaries to the correct .so
  foreach package : subprojects
    package_name = package[0]
    if package[1].length() == 0 or get_option(package[1][0])
      if package[3]
        if package[8] and get_option('efl-one')
          set_variable(package_name, efl_one)
        endif
      endif
    endif
  endforeach
endif