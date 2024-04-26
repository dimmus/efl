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
cpu     = host_machine.cpu_family()

langs = ['c', 'cpp']
cc  = meson.get_compiler('c')
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

dir_prefix  = get_option('prefix')
dir_sysconf = dir_prefix / get_option('sysconfdir')
dir_bin     = dir_prefix / get_option('bindir')
dir_data    = dir_prefix / get_option('datadir')
dir_include = dir_prefix / get_option('includedir')
dir_lib     = dir_prefix / get_option('libdir')
# dir_libexec = dir_prefix / get_option('libexecdir')
# dir_module_weston = dir_lib / 'weston'
# dir_module_libweston = dir_lib / 'libweston-@0@'.format(libweston_major)
# dir_data_pc = dir_data / 'pkgconfig'
# dir_lib_pc = dir_lib / 'pkgconfig'
# dir_man = dir_prefix / get_option('mandir')
# dir_protocol_libweston = 'libweston-@0@'.format(libweston_major) / 'protocols'
# dir_gitlab_ci = meson.project_source_root() / '.gitlab-ci'

local_lib       = 'src/lib'
local_bin       = 'src/bin'
local_module    = 'src/modules'
# local_scripts   = 'src/scripts'
local_tests     = 'tests'
local_examples  = 'examples'
# local_bindings  = 'bindings'
local_benchmark = 'benchmarks'

# Build environment
summary_dir += {'Build directory':   dir_build}
summary_dir += {'Source directory':  dir_source}
summary_dir += {'Install prefix':    get_option('prefix')}
summary_dir += {'Data directory':    dir_data}
pathsep = sys_windows ? ';' : ':'
summary_dir += {'Binary directory':  dir_bin}
summary_dir += {'Library directory': dir_lib}
summary_dir += {'Module directory':  dir_source / local_module}
summary_dir += {'Libexec directory': get_option('prefix') / get_option('libexecdir')}
summary_dir += {'Include directory': dir_include}
summary_dir += {'Config directory':  get_option('prefix') / get_option('sysconfdir')}
if not sys_windows
  summary_dir += {'Local state directory': 'queried at runtime'}
endif
# summary_dir += {'Doc directory':     get_option('prefix') / get_option('docdir')}
