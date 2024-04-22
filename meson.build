project('efl', ['c','cpp'],
  version: '2.0.99',
  default_options : [
    'buildtype=release', # plain,debug,debugoptimized,release,minsize,custom
    'warning_level=2',   # 0,1,2,3
    # 'optimization=g',  # 0,g,1,2,3,s
    # 'b_sanitize=none   # none,address,thread,undefined,memory,leak,address,undefined',
    # 'b_coverage=true',
    # 'b_lto=false',
    # 'b_lto_threads=32',
    # 'b_staticpic=true', 
    # 'b_pie=false',
    # 'b_lundef=true',
    'c_std=gnu99'
  ],
  meson_version : '>=0.63'
)

subdir('meson/arrays')
subdir('meson/constants')
subdir('meson/cflags')
subdir('meson/arguments')

# env = find_program('env', native: true)

efl_config = configuration_data()
efl_config.set_quoted('EFL_MODULE_ARCH'  , version_name)
efl_config.set_quoted('EFL_VERSION'      , version)
efl_config.set_quoted('EFL_BIN_DIR'      , dir_bin)
efl_config.set_quoted('EFL_LIB_DIR'      , dir_lib)
efl_config.set_quoted('EFL_BUILD_DIR'    , dir_build)
efl_config.set_quoted('EFL_SRC_DIR'      , dir_source)
efl_config.set_quoted('EFL_BUILD_ID'     , '1')
efl_config.set10     ('EFL_HAVE_THREADS' , true)
efl_config.set       ('EFL_VERSION_MAJOR', version_major)
efl_config.set       ('EFL_VERSION_MINOR', version_minor)
efl_config.set       ('EFL_VERSION_MICRO', version_micro)

# efl_config.set_quoted('EFL_NOLEGACY_API_SUPPORT', '1')

# efl_config.set_quoted('PACKAGE'            , name)
# efl_config.set_quoted('VERSION'            , version)
# efl_config.set_quoted('LOCALE_DIR'         , join_paths([dir_prefix, 'share/locale']))
# efl_config.set_quoted('PACKAGE_URL'        , 'https://www.enlightenment.org')
# efl_config.set_quoted('PACKAGE_TARNAME'    , name)
# efl_config.set_quoted('PACKAGE_BUGREPORT'  , 'enlightenment-devel@lists.sourceforge.net')
# efl_config.set_quoted('PACKAGE_STRING'     , name + ' ' + version)
# efl_config.set_quoted('PACKAGE_NAME'       , name)
# efl_config.set_quoted('PACKAGE_BIN_DIR'    , dir_bin)
# efl_config.set_quoted('PACKAGE_SRC_DIR'    , meson.project_source_root())
# efl_config.set_quoted('PACKAGE_SYSCONF_DIR', dir_sysconf)
# efl_config.set_quoted('BINDIR'             , dir_bin)
# efl_config.set10('SLOPPY_SPEC'     , true)

## have to get compiler again for this to work
# code = '''#define _GNU_SOURCE 1
# #include <unistd.h>
# #include <stdio.h>

# extern char **environ;

# void func(void) { printf("%p\n", environ); }
# '''
# if cc.compiles(code, args : '-lc', name : 'environ check')
#   efl_config.set10('HAVE_ENVIRON', true)
# endif

subdir('meson/optimizations')
subdir('meson/header_checks')
subdir('meson/lua')

if sys_linux
  subdir('meson/system/linux')
elif sys_bsd
  subdir('meson/system/bsd')
elif sys_osx
  subdir('meson/system/osx')
elif sys_sun
  subdir('meson/system/sun')
elif sys_windows
  subdir('meson/system/win')
else
  error('System ' + host_os + ' not known')
endif

subdir('po')

inc_config = [ include_directories('.') ]

if get_option('crypto') == 'openssl'
  crypto = dependency('openssl')
endif
if crypto.found()
  efl_config.set('HAVE_OPENSSL', '1')
endif
if get_option('crypto') != ''
  efl_config.set('HAVE_CIPHER', '1')
  efl_config.set('HAVE_SIGNATURE', '1')
endif

efl_config.set_quoted('SHARED_LIB_SUFFIX', '.' + sys_lib_extension) # eina_module
efl_config.set_quoted('MOD_SUFFIX', '.' + sys_mod_extension) # ecore
# if sys_exe_extension == ''
#   efl_config.set_quoted('EXE_SUFFIX', '')
# else
#   efl_config.set_quoted('EXE_SUFFIX', '.' + sys_exe_extension)
# endif

#we have to do that first, eina modules are required by eina
#the other modules require theire package
subdir(join_paths(local_module,'shared'))

subprojects = [
  # name                |   option             | mod   | lib  | bin | bench | tests | examples | true if build in efl-one | pkg-config options | name of static libs
  # ['evil'             ,[]                    , false,  true, false,  false,  false,     false,  true, [], []],
  ['shared'             ,[]                    , false,  true,  true,   true,   true,      true,  true, [], []],
  ['eo_gen'             ,[]                    , false,  true,  true,  false,  false,     false, false, ['shared'], []],
  ['eo'                 ,[]                    , false,  true, false,   true,   true,     false,  true, ['shared'], []],
  ['eo_interfaces'      ,[]                    , false,  true, false,  false,   true,     false,  true, ['eo'], []],
  ['core'               ,[]                    , false,  true, false,  false,  false,     false,  true, ['shared', 'eo', 'eo_interfaces'], []],
  ['dbus'               ,[]                    , false,  true,  true,  false,   true,      true,  true, ['shared', 'eo', 'eo_interfaces'], []], #core modules depend on dbus
  ['core'               ,[]                    ,  true, false, false,  false,   true,      true,  true, ['shared', 'eo', 'eo_interfaces'], []], 
  ['core_input'         ,[]                    , false,  true, false,  false,  false,     false,  true, ['shared', 'eo'], []],
  ['core_wl'            ,['wl']                ,  true,  true, false,  false,   true,     false,  true, ['shared'], ['libdrm']],
  # ['emile'            ,[]                    , false,  true, false,  false,   true,      true,  true, ['eina', 'efl'], ['lz4', 'rg_etc']],
  # ['eet'              ,[]                    , false,  true,  true,  false,   true,      true,  true, ['eina', 'emile', 'efl'], []],
  # ['ecore_audio'      ,['audio']             , false,  true, false,  false,  false,     false,  true, ['eina', 'eo'], []],
  # ['ecore_avahi'      ,['avahi']             , false,  true, false,  false,  false,      true, false, ['eina', 'ecore'], []],
  # ['ecore_con'        ,[]                    , false,  true,  true,  false,   true,     false,  true, ['eina', 'eo', 'efl', 'ecore'], ['http-parser']],
  # ['ecore_file'       ,[]                    , false,  true, false,  false,  false,     false,  true, ['eina'], []],
  # ['eeze'             ,['eeze']              ,  true,  true,  true,  false,   true,     false,  true, ['eina', 'efl'], []],
  # ['ecore_x'          ,['x11']               , false,  true, false,  false,  false,     false,  true, ['eina', 'efl'], []],
  # ['ecore_fb'         ,['fb']                , false,  true, false,  false,  false,     false,  true, ['eina'], []],
  # ['ecore_sdl'        ,['sdl']               , false,  true, false,  false,  false,     false,  true, ['eina'], []],
  # ['ecore_win32'      ,[]                    , false,  true, false,  false,  false,     false,  true, ['eina'], []],
  # ['ecore_ipc'        ,[]                    , false,  true, false,  false,  false,     false,  true, ['eina'], []],
  # ['ecore_buffer'     ,['buffer']            ,  true,  true,  true,  false,  false,     false,  true, ['eina'], []],
  # ['ector'            ,[]                    , false,  true, false,  false,   true,     false,  true, ['eina', 'efl'], ['draw', 'triangulator', 'freetype']],
  # ['elput'            ,['input']             , false,  true, false,  false,   true,     false,  true, ['eina', 'eldbus'], []],
  # ['ecore_drm2'       ,['drm']               , false,  true, false,  false,  false,     false,  true, ['ecore'], ['libdrm']],
  # ['ecore_cocoa'      ,['cocoa']             , false,  true, false,  false,  false,     false,  true, ['eina'], []],
  # ['evas'             ,[]                    ,  true,  true, false,  false,   true,      true,  true, ['eina', 'efl', 'eo'], ['vg_common', 'libunibreak']],
  # ['efreet'           ,[]                    , false,  true, false,  false,   true,     false,  true, ['eina', 'efl', 'eo'], []],
  # ['ecore_input_evas' ,[]                    , false,  true, false,  false,  false,     false,  true, ['eina', 'evas'], []],
  # ['ecore_evas'       ,[]                    ,  true,  true,  true,  false,  false,     false,  true, ['evas', 'ector'], []],
  # ['ecore_imf'        ,[]                    ,  true,  true, false,  false,  false,     false,  true, ['eina'], []],
  # ['embryo'           ,[]                    , false,  true,  true,  false,  false,     false,  true, ['eina', 'efl', 'eo'], []],
  # ['eio'              ,[]                    , false,  true, false,  false,   true,      true,  true, ['eina', 'eet'], []],
  # ['efreet'           ,[]                    , false, false,  true,  false,  false,     false,  true, ['eina', 'efl', 'eo'], []],
  # ['ecore_imf_evas'   ,[]                    , false,  true, false,  false,  false,     false,  true, ['eina', 'efl', 'eo'], []],
  # ['ephysics'         ,['physics']           , false,  true, false,  false,  false,     false,  true, ['eina', 'efl', 'eo'], []],
  # ['edje'             ,[]                    , false,  true,  true,  false,   true,      true,  true, ['evas', 'eo', 'efl', lua_pc_name], []],
  # ['emotion'          ,[]                    ,  true,  true, false,  false,   true,      true,  true, ['eina', 'efl', 'eo'], []],
  # ['ethumb'           ,[]                    ,  true,  true,  true,  false,  false,     false,  true, ['eina', 'efl', 'eo'], []],
  # ['ethumb_client'    ,[]                    , false,  true,  true,  false,  false,      true,  true, ['eina', 'efl', 'eo', 'ethumb'], []],
  # ['elementary'       ,[]                    ,  true,  true,  true,   true,   true,      true,  true, ['eina', 'efl', 'eo', 'eet', 'evas', 'ecore', 'ecore-evas', 'ecore-file', 'ecore-input', 'edje', 'ethumb-client', 'emotion', 'ecore-imf', 'ecore-con', 'eldbus', 'efreet', 'efreet-mime', 'efreet-trash', 'eio'], ['atspi']],
  # ['efl_canvas_wl'    ,['wl']                , false,  true,  true,  false,  false,     false,  true, ['eina', 'efl', 'eo', 'evas', 'ecore'], []],
  # ['elua'             ,['elua']              , false,  true,  true,  false,   true,     false, false, ['eina', lua_pc_name], []],
  # ['ecore_wayland'    ,['wl-deprecated']     , false,  true, false,  false,  false,     false, false, ['eina'], []],
  # ['ecore_drm'        ,['drm-deprecated']    , false,  true, false,  false,  false,     false, false, ['eina'], []],
  # ['exactness'        ,[]                    , false,  false,  true, false,  false,     false, false, ['eina, evas, eet'], []],
]

# tmp_empty = declare_dependency() # need for static libs

foreach package : subprojects
  package_name = package[0]
  package_version_name = '-'.join(package_name.split('_')) + '-' + version_major
  automatic_pkgfile = true
  if package[1].length() == 0 or get_option(package[1][0])
    efl_config.set('HAVE_' + package_name.to_upper().underscorify(), '1')

    dir_package_include = join_paths(dir_include, package_version_name)
    dir_package_modules = join_paths(dir_lib, package_name, 'modules')

    # ensure that we really dont copy the eo file targets from a previous
    # library Those are the variables that can be used to reflect the libraries
    # speical handlings -> at the end is used to indicate where to find this
    # variable outside of this for loop

    # public eo and eot files - which probebly have to be used later for bindings
    pub_eo_files = []           # -> package_name + '_eo_files'
    pub_eo_types_files = []     # -> package_name + '_eot_files'
    # All subdirs where eo files that are listed in the pub_* variables can be
    # found
    # For every element != '' a variable called package_name + '_' + subir + '_eot_files' and package_name + '_' + subir + '_eo_files' must exist.
    package_eo_subdirs = ['']     # -> package_name + '_eo_subdirs'
    # All subdirs that should be included in order to include every requried header
    package_header_subdirs = [] # -> package_name + '_include_subdirs'
    # eo file targets, this list of targets can be used to ensure the files are created before accessed
    pub_eo_file_target = []
    # private eo files target - never use this :)
    priv_eo_file_target = []
    # use this variable to store custom variables in that should be placed in the .pc file
    package_pc_variables = []

    # foreach static_lib : package[10]
    #   if get_variable(static_lib, tmp_empty) == tmp_empty
    #     subdir(join_paths('src/static', static_lib))
    #   endif
    # endforeachs

    #package_c_args definition for lib and module
    package_c_args = [
      '-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, package_name)+'"',
      '-DNEED_RUN_IN_TREE=1',
      '-DEFL_BUILD=1',
    ]
    
    if package[3]
      subdir(join_paths(local_lib, package_name))
      set_variable(package_name + '_eo_files', pub_eo_files)
      set_variable(package_name + '_eot_files', pub_eo_types_files)
      set_variable(package_name + '_header_subdirs', package_header_subdirs)
      set_variable(package_name + '_eo_subdirs', package_eo_subdirs)
      if package[8] and get_option('efl-one')
        src = get_variable('src_' + package_name)
        dep_efl_one = get_variable('dep_' + package_name + '_one')
        inc_efl_one_package = get_variable('inc_' + package_name)
        # inc_efl_one += [include_directories('.'), include_directories(join_paths(local_lib, package_name))]
        inc_efl_one += [include_directories('.'), inc_efl_one_package]

        foreach subdirs : package_eo_subdirs
          inc_efl_one += include_directories(join_paths(local_lib, package_name))
        endforeach

        tmp = static_library('efl_one_part_'+package_name,
            src, pub_eo_file_target, priv_eo_file_target,
            include_directories: inc_efl_one,
            dependencies: dep_efl_one + dep_efl_one_eo,
            c_args : package_c_args,
        )
        # dependency for all the .eo file targets
        dep_efl_one_eo += declare_dependency(
          sources: pub_eo_file_target + priv_eo_file_target, #this here *needs* to be public and private, because our binaries and modules do depend on internal headers
        )
        efl_one_parts += tmp
      endif
    endif
    
    #special case for eo_gen, this is never efl-one, but will be required in the library
    if package_name == 'eo_gen'
      package_c_args = [
        '-DPACKAGE_DATA_DIR="'+join_paths(dir_data, package_name)+'"',
        '-DNEED_RUN_IN_TREE=1',
      ]
      if package[4]
         subdir(join_paths(local_bin, package_name))
      endif
    endif
    
    if package[6]
       dir_test += [package_name]
    endif
    
    if package[7]
       dir_example += [package_name]
    endif

    set_variable('build_' + package_name.underscorify(), true)

    # generate automatic pc files for libraries
    if automatic_pkgfile and package[3]
      tmp_lib = get_variable('lib_' + package_name)
      # tmp_deps = get_variable('dep_' + package_name)
      tmp_pub_deps = get_variable('dep_' + package_name)
      tmp_package_subdirs = []

      foreach subdir : package_header_subdirs
        tmp_package_subdirs += join_paths(package_version_name, subdir)
      endforeach
      
      if package[8] and get_option('efl-one')
        sub_efl_one += [package_version_name] + tmp_package_subdirs
      endif
      
      pkgconfig.generate(tmp_lib,
        name : '-'.join(package_name.split('_')),
        subdirs : [package_version_name] + tmp_package_subdirs,
        version : version_major + '.' + version_minor + '.' + version_micro,
        libraries : tmp_pub_deps,
        requires : package[9],
        variables : package_pc_variables
      )
    endif
  else
    message(package_name+' disabled!')
  endif
endforeach

#delayed build of bin, modules, and benchmarks
foreach package : subprojects
  package_name = package[0]
  if package[1].length() == 0 or get_option(package[1][0])
    dir_package_include = join_paths(dir_include, package_version_name)
    dir_package_modules = join_paths(dir_lib, package_name, 'modules')
    package_c_args = [
      '-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, package_name)+'"',
      '-DNEED_RUN_IN_TREE=1',
      '-DEFL_BUILD=1',
    ]
    if package[2]
      subdir(join_paths(local_module, package_name))
    endif
    package_c_args = [
      '-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, package_name)+'"',
      '-DNEED_RUN_IN_TREE=1',
    ]
    if package[4] and package_name != 'eo_gen'
       subdir(join_paths(local_bin, package_name))
    endif
    if package[5]
       subdir(join_paths(local_benchmark, package_name))
    endif
  endif
endforeach

# List of dependency objects that might be disabled due to configurations
# If they are enabled, the object gets overwritten by the library file.
# ecore_audio = declare_dependency()

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
    libraries : [efl_one] + dep_shared, #eina is a special case here which drags in m dl & threads
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

# subdir('src/bin/efl') #build this later, as the debug services are depending on ecore
# subdir('src/generic/evas')
# subdir('cmakeconfig')
# subdir('src/bindings')
# subdir('src/edje_external')
# subdir('data')

if get_option('build-tests')
  check = dependency('check', version: '!= 0.15.1')

  test_env = environment()
  test_env.set('EFL_RUN_IN_TREE', '1')

  if get_option('b_sanitize') == 'address'
    test_env.set('ASAN_OPTIONS', 'detect_leaks=0:detect_odr_violation=0')
  endif

  subdir('tests')
  foreach test : dir_test
    package_c_args = [
      '-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, test)+'"',
      '-DNEED_RUN_IN_TREE=1',
    ]
    subdir(join_paths(local_tests, test))
  endforeach
endif

if get_option('build-examples')
  foreach example : dir_example
    package_c_args = [
      '-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, example)+'"',
      '-DNEED_RUN_IN_TREE=1',
    ]
    subdir(join_paths(local_examples, example))
  endforeach
endif

# foreach evas_loader_map_inst : evas_loader_map
#   evas_loader_original = evas_loader_map_inst[0]
#   evas_loader_link_types = evas_loader_map_inst[1]

#   meson.add_install_script('meson/evas_loader_conf.sh', evas_loader_original, evas_loader_link_types)
# endforeach

if get_option('docs')
  doxygen = find_program('doxygen', required : false)
  if doxygen.found()
    subdir('doc/api')
  endif
endif
summary_info += {'Documentation': get_option('docs') and doxygen.found()}

#
# Configure files
#

# efl_one_config_h = configuration_data()
# efl_one_config_h.set('EFL_VERSION_MAJOR', version_major)
# efl_one_config_h.set('EFL_VERSION_MINOR', version_minor)
# efl_one_config_h.set('EFL_VERSION_MICRO', version_micro)

# FIXME placeholder
# efl_one_config_h.set('EFL_API_LEGACY_DEF', '#define EFL_API_LEGACY_DEF "FIXME NOT IMPLEMENTED"')
# efl_one_config_h.set('EFL_MAX_FD_SIZE', get_option('max-fd-size'))

configure_file(
  output: 'efl_config.h',
  configuration: efl_config
)

# configure_file(
#   input: 'src/lib/efl/Efl_One_Config.h.in',
#   output: 'Efl_One_Config.h',
#   install_dir : join_paths(dir_include,'eina-'+version_major),
#   configuration: efl_one_config_h
# )

# configure_file(
#   input: 'elm_intro.h.in',
#   output: 'elm_intro.h',
#   configuration: efl_config,
#   install_dir : join_paths(dir_include,'elementary-'+version_major)
# )

subdir('services')

#output the three new efl-* .pc files
# efl_20_pc_files = {
#   'efl-ui' : ['elementary'],
#   'efl-core' : ['ecore', 'efl', 'emile'],
#   'efl-net' : ['ecore', 'ecore-con', 'emile'],
# }

# foreach name, libraries : efl_20_pc_files
#   pkgconfig.generate(
#     name : '-'.join(name.split('_')),
#     description: name+' configutation file',
#     requires : libraries,
#   )
# endforeach

summary(summary_dir,  bool_yn: true, section: 'Directories')
summary(summary_env,  bool_yn: true, section: 'Build environment')
summary(summary_mem,  bool_yn: true, section: 'Memory configuration')
summary(summary_opt,  bool_yn: true, section: 'Optimization')
summary(summary_efl,  bool_yn: true, section: 'EFL tools')
summary(summary_info, bool_yn: true, section: 'Information')