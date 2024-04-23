sys_lib_extension = 'so'
sys_exe_extension = ''
sys_mod_extension = 'so'

efl_config.set('_STAT_VER_LINUX', '1')
efl_config.set('HAVE_NOTIFY_KEVENT', '1')

# This is necessary. We MUST use OpenSSL in base as bringing in from ports
# can cause major issues (2 copies of the same library).
crypto = declare_dependency(link_args : [ '-lssl', '-lcrypto'])
if crypto.found()
  efl_config.set('HAVE_OPENSSL', '1')
endif

