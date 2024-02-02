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
      '-Wpacked',
      '-Winline',
      '-Wshadow',
      '-Wattributes',
      '-Wduplicated-branches',
      '-Wimplicit-fallthrough',
      '-Wmisleading-indentation',
      '-Wno-switch-enum', # too many in eolian
      '-Wno-conversion', # a lot here !!!
      '-Wno-double-promotion', # a lot here !!!
      '-Wno-missing-prototypes',
      '-Wno-vla', # no in C90 standard
      # Due to function casts through (void*) we cannot support -Wpedantic:
      '-Wno-pedantic',
      # We explicitly require variadic macros
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
    dev_cflags = ['-fsanitize=address'] + dev_cflags
    dev_ldflags = ['-fsanitize=address'] + dev_ldflags
  endif

  # Detect static linking issue with ubsan - https://gcc.gnu.org/bugzilla/show_bug.cgi?id=84285
  if cc.links('int main(int argc, char **argv) { return argc + 1; }',
              args: [dev_ldflags, '-fsanitize=undefined'])
    dev_cflags = ['-fsanitize=undefined'] + dev_cflags
    dev_ldflags = ['-fsanitize=undefined'] + dev_ldflags
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
  dev_cflags = ['-fsanitize=thread'] + dev_cflags
  dev_ldflags = ['-fsanitize=thread'] + dev_ldflags
endif

# if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
  # Detect support for PT_GNU_RELRO + DT_BIND_NOW.
  # The combination is known as "full relro", because .got.plt is read-only too.
  dev_ldflags += cc.get_supported_link_arguments('-Wl,-z,relro', '-Wl,-z,now', '-Wl,-z,nodelete', '-Wl,-Bsymbolic-functions')
# endif

if sys_windows
  dev_ldflags += cc.get_supported_link_arguments('-Wl,--no-seh', '-Wl,--nxcompat')
  dev_ldflags += cc.get_supported_link_arguments('-Wl,--dynamicbase', '-Wl,--high-entropy-va')
endif

# Exclude --warn-common with TSan to suppress warnings from the TSan libraries.
if sys_sun and not get_option('tsan')
  dev_ldflags += cc.get_supported_link_arguments('-Wl,--warn-common')
endif

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

# Compilation information
summary_info = {}
summary_info += {'Host CPU':          cpu}
summary_info += {'Host endianness':   build_machine.endian()}
summary_info += {'Host system':       host_os}
if build_machine.system() != host_os
  summary_info += {'Build CPU':        build_machine.cpu_family()}
  summary_info += {'Build endianness': build_machine.endian()}
  summary_info += {'Build system':     build_machine.system()}
endif
summary_info += {'C compiler':        ' '.join(meson.get_compiler('c').cmd_array())}
summary_info += {'Host C compiler':   ' '.join(meson.get_compiler('c', native: true).cmd_array())}
if 'cpp' in langs
  summary_info += {'C++ compiler':    ' '.join(meson.get_compiler('cpp').cmd_array())}
else
  summary_info += {'C++ compiler':      false}
endif
if 'objc' in langs
  summary_info += {'Objective-C compiler': ' '.join(meson.get_compiler('objc').cmd_array())}
else
  summary_info += {'Objective-C compiler': false}
endif
option_cflags = (get_option('debug') ? ['-g'] : [])
if get_option('optimization') != 'plain'
  option_cflags += ['-O' + get_option('optimization')]
endif
summary_info += {'CFLAGS':            ' '.join(get_option('c_args') + option_cflags)}
if 'cpp' in langs
  summary_info += {'CXXFLAGS':        ' '.join(get_option('cpp_args') + option_cflags)}
endif
if 'objc' in langs
  summary_info += {'OBJCFLAGS':       ' '.join(get_option('objc_args') + option_cflags)}
endif
link_args = get_option('c_link_args')
if link_args.length() > 0
  summary_info += {'LDFLAGS':         ' '.join(link_args)}
endif
summary_info += {'LTO':               get_option('b_lto')}
summary_info += {'PIE':               get_option('b_pie')}
summary_info += {'Static build':      get_option('prefer_static')}
summary_info += {'Stack protector':   have_stack_protector}
summary_info += {'Malloc trim support': has_malloc_trim}
# summary_info += {'debug graph lock':  get_option('debug_graph_lock')}
# summary_info += {'debug stack usage': get_option('debug_stack_usage')}
# summary_info += {'mutex debugging':   get_option('debug_mutex')}
summary_info += {'Memory allocator':  get_option('malloc')}
# summary_info += {'avx2 optimization': config_host_data.get('CONFIG_AVX2_OPT')}
# summary_info += {'avx512bw optimization': config_host_data.get('CONFIG_AVX512BW_OPT')}
# summary_info += {'avx512f optimization': config_host_data.get('CONFIG_AVX512F_OPT')}
summary_info += {'GCov':              get_option('b_coverage')}
summary_info += {'Download dependencies': get_option('wrap_mode') != 'nodownload'}
summary(summary_info, bool_yn: true, section: 'Build environment')