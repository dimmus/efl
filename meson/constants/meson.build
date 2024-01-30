dir_source = meson.project_source_root()
dir_build  = meson.project_build_root()

pkgconfig = import('pkgconfig')

name = meson.project_name()

version       = meson.project_version()
version_arr   = version.split('.')
version_major = version_arr[0]
version_minor = version_arr[1]
version_micro = version_arr[2]
version_name  = 'v-' + version_major + '.' + version_minor

cc = meson.get_compiler('c')
host_os = host_machine.system()

langs = ['c', 'cpp']

sys_linux   = ['linux'].contains(host_os)
sys_bsd     = ['bsd', 'freebsd', 'dragonfly', 'netbsd', 'openbsd'].contains(host_os)
sys_windows = ['windows', 'cygwin'].contains(host_os)
sys_osx     = ['darwin'].contains(host_os)
sys_sun     = ['sunos'].contains(host_os)

if sys_linux
  if cc.has_header_symbol('features.h', '__UCLIBC__')
    host_os = 'linux-uclibc'
  elif cc.has_header_symbol('features.h', '__dietlibc__')
    host_os = 'linux-dietlibc'
  else
    host_os = 'linux-gnu'
  endif
endif

dir_prefix    = get_option('prefix')
dir_sysconf   = join_paths(dir_prefix, get_option('sysconfdir'))
dir_bin       = join_paths(dir_prefix, get_option('bindir'))
dir_data      = join_paths(dir_prefix, get_option('datadir'))
dir_include   = join_paths(dir_prefix, get_option('includedir'))
dir_lib       = join_paths(dir_prefix, get_option('libdir'))

local_lib       = 'src/lib'
local_bin       = 'src/bin'
local_module    = 'src/modules'
# local_scripts   = 'src/scripts'
local_tests     = 'tests'
local_examples  = 'examples'
# local_bindings  = 'bindings'
local_benchmark = 'benchmarks'

summary(
  {
    'os': host_os,
    'prefix': dir_prefix,
    'bindir': dir_bin,
    'libdir': dir_lib,
    'includedir': dir_include,
  },
  section: 'Directories'
)