add_project_arguments('-D_POSIX_C_SOURCE=200809L', language: langs)
add_project_arguments('-DDLL_EXPORT=1', language: langs)
#   if get_option('windows-version') == 'vista'
#     add_project_arguments('-DWINVER=0x060', language: langs)
#     add_project_arguments('-D_WIN32_WINNT=0x0600', language: langs)
#   elif get_option('windows-version') == 'win7'
#     add_project_arguments('-DWINVER=0x0601', language: langs)
#     add_project_arguments('-D_WIN32_WINNT=0x0601', language: langs)
#   elif get_option('windows-version') == 'win8'
#     add_project_arguments('-DWINVER=0x0602', language: langs)
#     add_project_arguments('-D_WIN32_WINNT=0x0602', language: langs)
#   elif get_option('windows-version') == 'win81'
#     add_project_arguments('-DWINVER=0x0603', language: langs)
#     add_project_arguments('-D_WIN32_WINNT=0x0603', language: langs)
#   elif get_option('windows-version') == 'win10'
#     add_project_arguments('-DWINVER=0x0A00', language: langs)
#     add_project_arguments('-D_WIN32_WINNT=0x0A00', language: langs)
#   else
#     error('Version of targetted Windows incorrect')
#   endif
add_project_arguments('-D__USE_MINGW_ANSI_STDIO', language: langs)
add_global_arguments('-define:WIN32', language: 'cs')

dev_ldflags += cc.get_supported_link_arguments('-Wl,--no-seh', '-Wl,--nxcompat')
dev_ldflags += cc.get_supported_link_arguments('-Wl,--dynamicbase', '-Wl,--high-entropy-va')

sys_lib_extension = 'dll'
sys_exe_extension = 'exe'
sys_mod_extension = 'dll' 

efl_config.set('HAVE_NOTIFY_WIN32', '1')
efl_config.set10('HAVE_DDRAW_H', true)

regexp = cc.find_library('regex',
                        has_headers: ['regex.h'],
                        required: true)
if not regexp.found()
    error('regex can not be found')
endif

bcrypt = cc.find_library('bcrypt', required: true)
dep_shared += [bcrypt]
dep_shared += [evil]

summary_dir += {'Local state directory': get_option('prefix') / get_option('localstatedir')}
summary_dir += {'Manual directory':      get_option('prefix') / get_option('mandir')}
