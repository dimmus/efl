if cc.has_header_symbol('libkern/OSAtomic.h', 'OSSpinLockTry')
    shared_config.set('EINA_HAVE_OSX_SPINLOCK', 1)
endif
if cc.has_header_symbol('mach/task.h', 'semaphore_create')
    shared_config.set('EINA_HAVE_OSX_SEMAPHORE', 1)
endif

sys_lib_extension = 'dylib'
sys_exe_extension = ''
sys_mod_extension = 'so'

efl_config.set('environ', '(*_NSGetEnviron())')
efl_config.set('HAVE_NOTIFY_COCOA', '1')

# if sys_osx and get_option('lua-interpreter') == 'luajit'
#   # luajit on macro is broken, this means we need to generate our own
#   # dependency with our arguments, a library later still needs to link to
#   # luajit for the pagesize argument thingy
#   lua = declare_dependency(
#     include_directories: include_directories(lua.get_pkgconfig_variable('includedir')),
#     link_args: ['-L' + lua.get_pkgconfig_variable('libdir'), '-l' + lua.get_pkgconfig_variable('libname')]
#   )
# endif