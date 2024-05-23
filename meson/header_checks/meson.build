if get_option('native-arch-optimization')
  check_native_header = true
  if cpu_family.startswith('x86')
    native_header = 'immintrin.h'
  elif cpu_family == 'arm'
    native_header = 'arm_neon.h'
  elif cpu_family == 'aarch64'
    native_header = 'arm_neon.h'
  elif cpu_family == 'ppc' or cpu_family == 'ppc64'
    native_header = 'altivec.h'
  else
    check_native_header = false
  endif

  if check_native_header
    if not cc.has_header(native_header)
      error('Error, header '+native_header+' is required')
    endif
    efl_config.set10('HAVE_'+native_header.underscorify().to_upper(), true)
  endif
endif

header_checks = [
  'alloca.h',
  'asm/hwcap.h',
  'bsd/string.h',
  'dirent.h',
  'execinfo.h',
  'mcheck.h',
  'netinet/in.h',
  'netinet/ssl.h',
  'netinet/tcp.h',
  'netinet/udp.h',
  'net/if.h',
  'stdlib.h',
  'sys/auxv.h',
  'sys/inotify.h',
  'sys/ioctl.h',
  'sys/mman.h',
  'sys/types.h',
  'sys/socket.h',
  'sys/filio.h',
  'arpa/inet.h',
  'sys/epoll.h',
  'sys/un.h',
  'sys/wait.h',
  'sys/resource.h',
  'sys/times.h',
  'longinfo.h',
  'ieeefp.h',
  'sys/timerfd.h',
  'features.h',
  'langinfo.h',
  'locale.h',
  'crt_externs.h',
  'pthread.h'
]

function_checks = [
  { 'name' : 'alloca',                'head' : ['alloca.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'backtrace',             'head' : ['execinfo.h'   ],   'deps' : [],          'args' : [] },
  { 'name' : 'backtrace_symbols',     'head' : ['execinfo.h'   ],   'deps' : [],          'args' : [] },
  { 'name' : 'chown',                 'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'clock_gettime',         'head' : ['time.h'       ],   'deps' : [],          'args' : [] },
  { 'name' : 'dirfd',                 'head' : ['dirent.h',    
                                                'sys/types.h'  ],   'deps' : [],          'args' : [] },
  { 'name' : 'fchmod',                'head' : ['sys/stat.h'   ],   'deps' : [],          'args' : [] },
  { 'name' : 'fcntl',                 'head' : ['fcntl.h'      ],   'deps' : [],          'args' : [] },
  { 'name' : 'fork',                  'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'fpathconf',             'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'geteuid',               'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'getpagesize',           'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'getpwent',              'head' : ['sys/types.h'   ,
                                                'pwd.h'        ],   'deps' : [],          'args' : [] },
  { 'name' : 'getuid',                'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'getxattr',              'head' : ['sys/types.h'   ,
                                                'sys/xattr.h'  ],   'deps' : [],          'args' : [] },
  { 'name' : 'iconv',                 'head' : ['iconv.h'      ],   'deps' : [],          'args' : [] },
  { 'name' : 'listxattr',             'head' : ['sys/types.h'   ,
                                                'sys/xattr.h'  ],   'deps' : [],          'args' : [] },
  { 'name' : 'malloc_info',           'head' : ['malloc.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'malloc_usable_size',    'head' : ['malloc.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'mkdirat',               'head' : ['sys/stat.h'   ],   'deps' : [],          'args' : [] },
  { 'name' : 'mmap',                  'head' : ['sys/mman.h'   ],   'deps' : [],          'args' : [] },
  { 'name' : 'mtrace',                'head' : ['mcheck.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'prctl',                 'head' : ['sys/prctl.h'  ],   'deps' : [],          'args' : [] },
  { 'name' : 'procctl',               'head' : ['sys/procctl.h'],   'deps' : [],          'args' : [] },
  { 'name' : 'realpath',              'head' : ['stdlib.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'setxattr',              'head' : ['sys/types.h'   ,
                                                'sys/xattr.h'  ],   'deps' : [],          'args' : [] },
  { 'name' : 'siglongjmp',            'head' : ['setjmp.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'strerror_r',            'head' : ['string.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'gettimeofday',          'head' : ['sys/time.h'   ],   'deps' : [],          'args' : [] },
  { 'name' : 'execvp',                'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'pause',                 'head' : ['unistd.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'isfinite',              'head' : ['math.h'       ],   'deps' : [],          'args' : [] },
  { 'name' : 'strlcpy',               'head' : ['string.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'siginfo_t',             'head' : ['signal.h'     ],   'deps' : [],          'args' : [] },
  { 'name' : 'timerfd_create',        'head' : ['sys/timerfd.h'],   'deps' : [],          'args' : [] },
  { 'name' : 'kevent',                'head' : ['sys/types.h'   , 
                                                'sys/event.h'   ,  
                                                'sys/time.h'   ],   'deps' : [],          'args' : [] },
  { 'name' : 'dlopen',                'head' : ['dlfcn.h'      ],   'deps' : ['dl'],      'args' : [] },
  { 'name' : 'dlsym',                 'head' : ['dlfcn.h'      ],   'deps' : ['dl'],      'args' : [] },
  { 'name' : 'lround',                'head' : ['math.h'       ],   'deps' : ['m' ],      'args' : [] },
  { 'name' : 'mallinfo2',             'head' : ['malloc.h'     ],   'deps' : ['malloc'],  'args' : [] },
  { 'name' : 'mallinfo',              'head' : ['malloc.h'     ],   'deps' : ['malloc'],  'args' : [] },
  { 'name' : 'shm_open',              'head' : ['sys/mman.h'    , 
                                                'sys/stat.h'    , 
                                                'fcntl.h'      ],   'deps' : ['rt'],      'args' : [] },
  { 'name' : 'splice',                'head' : ['fcntl.h'      ],   'deps' : [],          'args' : ['-D_GNU_SOURCE=1']},
  { 'name' : 'sched_getcpu',          'head' : ['sched.h'      ],   'deps' : [],          'args' : ['-D_GNU_SOURCE=1']},
  { 'name' : 'dladdr',                'head' : ['dlfcn.h'      ],   'deps' : ['dl'],      'args' : ['-D_GNU_SOURCE=1']},
  { 'name' : 'pthread_getcpuclockid', 'head' : ['pthread.h'     ,
                                                'time.h'        ],  'deps' : [],          'args' : [] },
]

open_cloexec = cc.compiles('''#include <sys/types.h>
                              #include <sys/stat.h>
                              #include <fcntl.h>
                              int main(int argc, char **argv) {
                                int res = open(argv[0], O_RDONLY | O_CLOEXEC);
                                if (res < 0) return 1;
                                return 0;
                              }
                           ''',
                           name : 'open works with O_CLOEXEC')
if open_cloexec
  efl_config.set10('HAVE_OPEN_CLOEXEC', true)
endif

strerror_r_char_p = cc.compiles('''#define _GNU_SOURCE
                  #include <string.h>
                  int func (void)
                    {
                       char error_string[256];
                       char *ptr = strerror_r (-2, error_string, 256);
                       char c = *strerror_r (-2, error_string, 256);
                       return c != 0 && ptr != (void*) 0L;
                    }
                 ''',
                 name : 'strerror_r() returns char *')

if strerror_r_char_p
  efl_config.set('STRERROR_R_CHAR_P', '1')
endif

dep_libm = cc.find_library('m')
#just keep this here as required false, if it is not inplace the library rt will just be linked as NOP
dep_libdl = cc.find_library('dl', required: false)
dep_librt = cc.find_library('rt', required: false)

dep_threads = dependency('threads')

#check for the headers
foreach header : header_checks
  if cc.has_header(header)
    efl_config.set10('HAVE_'+header.underscorify().to_upper(), true)
  endif
endforeach

foreach function : function_checks
  function_name = function['name']
  headers_to_search = function['head']
  args = function['args']
  dependencies = []

  # if there is a library, make sure they exist
  if function['deps'].length() > 0
    foreach library : function['deps']
      lib = cc.find_library(library, required : false)
      if lib.found()
        dependencies += lib
      endif
    endforeach
  endif

  # Only check the header if the dependencies are ready
  foreach header : headers_to_search
    if cc.has_header_symbol(header, function_name,
        dependencies : dependencies,
        args : args)
      efl_config.set10('HAVE_'+function_name.to_upper(), true)
    endif
  endforeach
endforeach

# The next checks are manually for now due to the fact that some names are not within the default pattern
if cc.has_header_symbol('sys/stat.h', 'fstatat')
  efl_config.set10('HAVE_ATFILE_SOURCE', true)
endif

# regexp = []
if not cc.has_header_symbol('regex.h', 'regcomp')
  error('regcomp can not be found')
endif

efl_config.set('VMAJ', version_major)
efl_config.set('VMIN', version_minor)
efl_config.set('VMIC', version_micro)
efl_config.set('VREV', '0')

#jpeg detection ... life is a bit more complex there

jpeg = dependency('libjpeg', required: false)
if jpeg.found()
  jpeg = cc.find_library('jpeg')
endif

efl_config.set('SIZEOF_INT', cc.sizeof('int'))
efl_config.set('SIZEOF_LONG', cc.sizeof('long'))

# Memory configuration
have_aligned_malloc = cc.has_function('_aligned_malloc', prefix: '#include <malloc.h>')
if have_aligned_malloc
  efl_config.set('HAVE__ALIGNED_MALLOC', 1)
endif

have_aligned_calloc = host_os != 'windows' and cc.has_function('aligned_alloc', prefix: '#include <stdlib.h>')
if have_aligned_calloc
  efl_config.set('HAVE_ALIGNED_ALLOC', 1)
endif
have_posix_memalign = host_os != 'windows' and cc.has_function('posix_memalign', prefix: '#include <stdlib.h>')
if have_posix_memalign
  efl_config.set('HAVE_POSIX_MEMALIGN', 1)
endif

summary_mem += {'Have aligned malloc': have_aligned_malloc}
summary_mem += {'Have aligned calloc': have_aligned_calloc}
summary_mem += {'Have posix memalign': have_posix_memalign}