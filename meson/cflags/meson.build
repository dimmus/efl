dev_ldflags = []

if get_option('buildtype').startswith('debug')
  if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
    dev_cflags_common_try = [
      # '-g3',
      # '-fsanitize-trap', # undefined reference to `__ubsan_handle*
      '-fvisibility=hidden',
      '-Wundef',
      '-Wunused',
      '-Wmissing-prototypes',
      '-Wmissing-braces',
      '-Wmissing-field-initializers',
      '-Wmissing-include-dirs',
      '-Wpointer-arith',
      '-Wcast-function-type',
      '-Wignored-qualifiers',
      '-Wnonnull',
      '-Wsign-compare','-Wno-vla',
      '-Wshift-negative-value',
      '-Wunused-but-set-parameter',
      '-Wunused-parameter',
      '-Wunused-function',
      '-Wunused-command-line-argument',
      '-Wempty-body',
      '-Wlogical-op',
      '-Winit-self',
      '-Wfloat-equal',
      '-Wfloat-compare',
      '-Wendif-labels',
      '-Wstrict-aliasing=2',
      '-Woverflow',
      '-Wformat-zero-length',
      '-Wendif-labels',
      '-Wwrite-strings',
      # '-Wpacked', # get warning in libdrm (i915_drm.h)
      '-Winline',
      '-Wshadow',
      '-Wattributes',
      '-Wduplicated-branches',
      '-Wimplicit-fallthrough',
      '-Wmisleading-indentation',
      '-Wduplicated-cond',
      '-Wformat-nonliteral',
      '-Wformat-security',
      # '-Wmissing-format-attribute',
      '-Wmissing-include-dirs',
      '-Wmissing-noreturn',
      '-Wpointer-arith',
      '-Wuninitialized',

      '-Wno-switch-enum', # too many in eolian
      '-Wno-switch-default',
      '-Wno-conversion', # a lot here !!!
      '-Wno-double-promotion', # a lot here !!!
      '-Wno-missing-prototypes',
      '-Wno-vla', # no in C90 standard
      '-Wno-pedantic', # due to function casts through (void*) we cannot support -Wpedantic:
      '-Wno-variadic-macros',
      # '-Werror=format=2',
      '-Werror=init-self',
      '-Werror=missing-include-dirs',
      '-Werror=pointer-arith',
      '-Werror=unused-result',
    ]
    dev_cflags_cc_try = dev_cflags_common_try + [
      '-Wstrict-prototypes',
      '-Wmissing-parameter-type',
      '-Wold-style-declaration',
      '-Wold-style-definition',
      '-Woverride-init',
      '-Wimplicit-function-declaration',
      '-Wnested-externs',
      '-Wstring-plus-int',
      '-Wno-discarded-qualifiers',
      '-Wno-missing-declarations',
      # Due to pervasive use of things like _eo_log_obj_report, we do not support
      # building with -Wbad-function-cast.
      '-Wno-bad-function-cast',
      '-Werror=implicit-function-declaration',
      # '-Werror=missing-prototypes', # a lot here in #define ...
      '-Werror=pointer-sign',
    ]
  endif
  dev_cflags_cpp = cxx.get_supported_arguments(dev_cflags_common_try)
  dev_cflags_cc = cc.get_supported_arguments(dev_cflags_cc_try)
  if 'objc' in langs
    dev_cflags_objc = objc.get_supported_arguments(dev_cflags_cc_try)
  endif
else
  dev_cflags_cpp = []
  dev_cflags_cc = []
  if 'objc' in langs
    dev_cflags_objc = []
  endif
endif

dev_cflags = dev_cflags_cpp + dev_cflags_cc
if 'objc' in langs
  dev_cflags += dev_cflags_objc
endif

if not get_option('b_pie')
  dev_cflags += cc.get_supported_arguments('-fno-pie', '-no-pie')
endif

if get_option('prefer_static')
  dev_ldflags += get_option('b_pie') ? '-static-pie' : '-static'
endif

if not get_option('stack_protector').disabled()
  stack_protector_probe = '''
    int main(int argc, char *argv[])
    {
      char arr[64], *p = arr, *c = argv[argc - 1];
      while (*c) {
          *p++ = *c++;
      }
      return 0;
    }'''
  have_stack_protector = false
  foreach arg : ['-fstack-protector-strong', '-fstack-protector-all']
    # We need to check both a compile and a link, since some compiler
    # setups fail only on a .c->.o compile and some only at link time
    if cc.compiles(stack_protector_probe, args: ['-Werror', arg]) and \
       cc.links(stack_protector_probe, args: ['-Werror', arg])
      have_stack_protector = true
      dev_cflags += arg
      dev_ldflags += arg
      break
    endif
  endforeach
  get_option('stack_protector') \
    .require(have_stack_protector, error_message: 'Stack protector not supported')
endif

if get_option('sanitizers')
  if cc.has_argument('-fsanitize=address')
    dev_cflags += ['-fsanitize=address']
    dev_ldflags += ['-fsanitize=address']
  endif

  # Detect static linking issue with ubsan - https://gcc.gnu.org/bugzilla/show_bug.cgi?id=84285
  if cc.links('int main(int argc, char **argv) { return argc + 1; }',
              args: [dev_ldflags, '-fsanitize=undefined'])
    dev_cflags += ['-fsanitize=undefined']
    dev_ldflags += ['-fsanitize=undefined']
  endif
endif

# Thread sanitizer is, for now, much noisier than the other sanitizers;
# keep it separate until that is not the case.
# !!! Consider using the built-in option for sanitizers instead of using "-fsanitize=thread"
if get_option('tsan')
  if get_option('sanitizers')
    error('TSAN is not supported with other sanitizers')
  endif
  if not cc.has_function('__tsan_create_fiber',
                         args: '-fsanitize=thread',
                         prefix: '#include <sanitizer/tsan_interface.h>')
    error('Cannot enable TSAN due to missing fiber annotation interface')
  endif
  dev_cflags += ['-fsanitize=thread']
  dev_ldflags += ['-fsanitize=thread']
endif

# if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
  # Detect support for PT_GNU_RELRO + DT_BIND_NOW.
  # The combination is known as "full relro", because .got.plt is read-only too.
  dev_ldflags += cc.get_supported_link_arguments('-Wl,-z,relro', '-Wl,-z,now', '-Wl,-z,nodelete', '-Wl,-Bsymbolic-functions')
# endif

# Malloc tests
malloc = []
if get_option('malloc') == 'system'
  has_malloc_trim = \
    get_option('malloc_trim').allowed() and \
    cc.has_function('malloc_trim', prefix: '#include <malloc.h>')
else
  has_malloc_trim = false
  malloc = cc.find_library(get_option('malloc'), required: true)
endif
if not has_malloc_trim and get_option('malloc_trim').enabled()
  if get_option('malloc') == 'system'
    error('malloc_trim not available on this platform.')
  else
    error('malloc_trim not available with non-libc memory allocator')
  endif
endif

debug = get_option('debug')
# optimization = get_option('optimization')
if debug
  dev_cflags += '-DDEBUG'
endif

# Compilation information
summary_env = {}
summary_env += {'Host system':       host_os}
summary_env += {'Host CPU':          cpu}
summary_env += {'Host endianness':   build_machine.endian()}

if build_machine.system() != host_machine.system()
  summary_env += {'Build CPU':        build_machine.cpu_family()}
  summary_env += {'Build endianness': build_machine.endian()}
  summary_env += {'Build system':     build_machine.system()}
endif

summary_env += {'C compiler':        ' '.join(meson.get_compiler('c').cmd_array())}
summary_env += {'Host C compiler':   ' '.join(meson.get_compiler('c', native: true).cmd_array())}

if 'cpp' in langs
  summary_env += {'C++ compiler':    ' '.join(meson.get_compiler('cpp').cmd_array())}
else
  summary_env += {'C++ compiler':      false}
endif

if 'objc' in langs
  summary_env += {'Objective-C compiler': ' '.join(meson.get_compiler('objc').cmd_array())}
else
  summary_env += {'Objective-C compiler': false}
endif

option_cflags = (get_option('debug') ? ['-g'] : [])

if get_option('optimization') != 'plain'
  option_cflags += ['-O' + get_option('optimization')]
endif

summary_env += {'CFLAGS':            ' '.join(get_option('c_args') + option_cflags)}

if 'cpp' in langs
  summary_env += {'CXXFLAGS':        ' '.join(get_option('cpp_args') + option_cflags)}
endif

if 'objc' in langs
  summary_env += {'OBJCFLAGS':       ' '.join(get_option('objc_args') + option_cflags)}
endif

link_args = get_option('c_link_args')
if link_args.length() > 0
  summary_env += {'LDFLAGS':         ' '.join(link_args)}
endif

summary_env += {'LTO':               get_option('b_lto')}
summary_env += {'PIE':               get_option('b_pie')}
summary_env += {'Static build':      get_option('prefer_static')}
summary_env += {'Stack protector':   have_stack_protector}
# summary_env += {'debug graph lock':  get_option('debug_graph_lock')}
# summary_env += {'debug stack usage': get_option('debug_stack_usage')}
# summary_env += {'mutex debugging':   get_option('debug_mutex')}
# summary_env += {'avx2 optimization': config_host_data.get('CONFIG_AVX2_OPT')}
# summary_env += {'avx512bw optimization': config_host_data.get('CONFIG_AVX512BW_OPT')}
# summary_env += {'avx512f optimization': config_host_data.get('CONFIG_AVX512F_OPT')}
summary_env += {'GCov':              get_option('b_coverage')}
summary_env += {'Download dependencies': get_option('wrap_mode') != 'nodownload'}
summary_env += {'Debugging': get_option('debug')}

summary_mem += {'Memory allocator':  get_option('malloc')}
summary_mem += {'Malloc trim support': has_malloc_trim}