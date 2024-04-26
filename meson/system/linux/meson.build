efl_config.set('_STAT_VER_LINUX', '1')

sys_lib_extension = 'so'
sys_exe_extension = ''
sys_mod_extension = 'so'

pthread_np_header_file = 'pthread.h'
pthread_setname_np_api = 'pthread_setname_np'
pthread_args = '-D_GNU_SOURCE'

if efl_config.has('HAVE_LISTXATTR') and \
   efl_config.has('HAVE_SETXATTR') and \
   efl_config.has('HAVE_GETXATTR')
      efl_config.set10('HAVE_XATTR', true)
endif