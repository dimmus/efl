# for getpwuid_r()
add_global_arguments('-D_POSIX_PTHREAD_SEMANTICS', language: 'c')

sys_lib_extension = 'so'
sys_exe_extension = ''
sys_mod_extension = 'so'

efl_config.set('_STAT_VER_LINUX', '1')

# Exclude --warn-common with TSan to suppress warnings from the TSan libraries.
if not get_option('tsan')
  dev_ldflags += cc.get_supported_link_arguments('-Wl,--warn-common')
endif

malloc = cc.find_library('malloc', required: true)
socket = cc.find_library('socket', required: true)

dep_core += [malloc]
dep_shared += [socket]
