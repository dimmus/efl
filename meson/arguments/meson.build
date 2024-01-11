add_project_arguments(dev_cflags, language: 'c')
add_project_arguments(dev_cflags, language: 'cpp')

add_project_arguments('-DHAVE_CONFIG_H=1', language: langs)
add_project_arguments('-D_GNU_SOURCE=1', language: langs)
add_project_arguments('-DEFL_BETA_API_SUPPORT=1', language: langs)
# add_project_arguments('-DNEED_RUN_IN_TREE=1', language: langs)
# add_project_arguments('-DELM_INTERNAL_API_ARGESFSDFEFC=1', language: langs)

if sys_windows
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
endif

if sys_sun
# for getpwuid_r()
  add_global_arguments('-D_POSIX_PTHREAD_SEMANTICS', language: 'c')
endif