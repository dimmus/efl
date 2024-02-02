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

host_os = host_machine.system()
cpu = host_machine.cpu_family()

langs = ['c', 'cpp']
cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')

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

if sys_osx and add_languages('objc', required: get_option('cocoa'), native: false)
  langs += ['objc']
  objc = meson.get_compiler('objc')
  bin_linker_args = ['-pagezero_size', '10000', '-image_base', '100000000']
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

# summary(
#   {
#     'os': host_os,
#     'prefix': dir_prefix,
#     'bindir': dir_bin,
#     'libdir': dir_lib,
#     'includedir': dir_include,
#   },
#   section: 'Directories'
# )

# Build environment
summary_info = {}
summary_info += {'Build directory':   dir_build}
summary_info += {'Source directory':  dir_source}
summary_info += {'Install prefix':    get_option('prefix')}
summary_info += {'Data directory':    dir_data}
pathsep = sys_windows ? ';' : ':'
summary_info += {'Binary directory':  get_option('prefix') / get_option('bindir')}
summary_info += {'Library directory': get_option('prefix') / get_option('libdir')}
summary_info += {'Module directory':  dir_source / local_module}
summary_info += {'Libexec directory': get_option('prefix') / get_option('libexecdir')}
summary_info += {'Include directory': get_option('prefix') / get_option('includedir')}
summary_info += {'Config directory':  get_option('prefix') / get_option('sysconfdir')}
if sys_windows
  summary_info += {'Local state directory': get_option('prefix') / get_option('localstatedir')}
  summary_info += {'Manual directory':      get_option('prefix') / get_option('mandir')}
else
  summary_info += {'Local state directory': 'queried at runtime'}
endif
# summary_info += {'Doc directory':     get_option('prefix') / get_option('docdir')}
summary(summary_info, bool_yn: true, section: 'Directories')