basic_link_flags_try = [
  '-fstack-protector',
  '-Wl,--fatal-warnings',
  # Detect support for PT_GNU_RELRO + DT_BIND_NOW.
  # The combination is known as "full relro", because .got.plt is read-only too.  
  '-Wl,-z,now',
  '-Wl,-z,relro',
  '-Wl,-z,nodelete',
  '-Wl,-Bsymbolic-functions'
]

# Those generate many false positives, 
# and we do not want to change the code to avoid them.
basic_disabled_warnings_try = [
  '-Wno-missing-field-initializers',
  '-Wno-missing-prototypes',
  '-Wno-unused-parameter',
  '-Wno-nonnull-compare',
  '-Wno-array-bounds', # TODO: 'outside array bounds' warning in eina_inline_value.x when compile with -O3 (buildtype=release)
  '-Wno-clobbered' # TODO: 'clobbered by longjmp or vfork' warning in eo_lexer.c when compile with -O3 (buildtype=release)
]

basic_c_flags_try = []

dev_link_flags_try = []

if get_option('buildtype').startswith('debug')
  if cc.get_id() == 'gcc' or cc.get_id() == 'clang'
    dev_disabled_warnings_try = [
      '-Wno-packed',            # get warning in libdrm (i915_drm.h)
      '-Wno-missing-format-attribute',
      '-Wno-switch-enum',       # too many in eolian
      '-Wno-switch-default',
      '-Wno-conversion',        # a lot here !!!
      '-Wno-double-promotion',  # a lot here !!!
      '-Wno-vla',               # no in C90 standard
      '-Wno-pedantic',          # due to function casts through (void*) we cannot support -Wpedantic:
      '-Wno-variadic-macros',   # a lot here !!!
      '-Wno-missing-declarations',
      # Due to pervasive use of things like _eo_log_obj_report, we do not support
      # building with -Wbad-function-cast.
      '-Wno-bad-function-cast',
    ]

    dev_c_flags_try = [
      # '-g3',
      # '-fsanitize-trap', # undefined reference to `__ubsan_handle*
      # '-Werror=format=2',
      '-fvisibility=hidden',
      '-Wundef',
      '-Wunused',
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
      '-Wempty-body', 
      '-Winit-self',
      '-Wfloat-equal',
      # '-Wfloat-compare', # no in gcc and clang
      '-Wendif-labels',
      '-Wstrict-aliasing=2',
      '-Woverflow',
      '-Wformat-zero-length',
      '-Wendif-labels',
      '-Wwrite-strings',
      '-Winline',
      '-Wshadow',
      '-Wattributes',     
      '-Wimplicit-fallthrough',
      '-Wmisleading-indentation',
      '-Wformat-nonliteral',
      '-Wformat-security',
      '-Wmissing-include-dirs',
      '-Wmissing-noreturn',
      '-Wpointer-arith',
      '-Wuninitialized',
      '-Werror=init-self',
      '-Werror=missing-include-dirs',
      '-Werror=pointer-arith',
      '-Werror=unused-result',
      '-Wstrict-prototypes',     
      '-Wold-style-definition',
      '-Woverride-init',
      '-Wimplicit-function-declaration',
      '-Wnested-externs',         
      '-Werror=implicit-function-declaration',
      '-Werror=pointer-sign',
    ]

    if cc.get_id() == 'gcc'
      dev_disabled_warnings_try += [
        '-Wno-discarded-qualifiers',
      ]
      dev_c_flags_try += [
        '-Wlogical-op',
        '-Wduplicated-branches',
        '-Wduplicated-cond',
        '-Wmissing-parameter-type',
        '-Wold-style-declaration',
      ]
    endif

    if cc.get_id() == 'clang'
      dev_disabled_warnings_try += [
        # negative arguments are correctly detected starting with meson 0.46.
        '-Wno-error=#warnings',
        '-Wno-string-plus-int',
        '-Wno-typedef-redefinition',
        '-Wno-gnu-variable-sized-type-not-at-end',
      ]

      dev_c_flags_try += [
        '-Wunused-command-line-argument',
        '-Wstring-plus-int',
      ]
    endif
  endif

  if get_option('sanitizers')
    if cc.has_argument('-fsanitize=address')
      dev_c_flags_try += ['-fsanitize=address']
      dev_link_flags_try += ['-fsanitize=address']
    endif

    # Detect static linking issue with ubsan - https://gcc.gnu.org/bugzilla/show_bug.cgi?id=84285
    if cc.links('int main(int argc, char **argv) { return argc + 1; }',
                args: [dev_link_flags_try, '-fsanitize=undefined'])
      dev_c_flags_try += ['-fsanitize=undefined']
      dev_link_flags_try += ['-fsanitize=undefined']
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
    dev_c_flags_try += ['-fsanitize=thread']
    dev_link_flags_try += ['-fsanitize=thread']
  endif

  if get_option('b_sanitize') == 'none'
    dev_link_flags_try += '-Wl,--warn-common'
  endif
endif  

c_args = get_option('c_args')
# Disable -Wmaybe-uninitialized when compiling with -Os/-O1/-O3/etc. There are
# too many false positives with gcc >= 8. Effectively, we only test with -O0
# and -O2; this should be enough to catch most important cases without too much
# busywork.
if cc.get_id() == 'gcc' and (not '02'.contains(get_option('optimization')) or
                             cc.version().version_compare('<10') or
                             '-Os' in c_args or
                             '-O1' in c_args or
                             '-O3' in c_args or
                             '-Og' in c_args)
        basic_disabled_warnings_try += '-Wno-maybe-uninitialized'
endif

# Disable -Wno-unused-result with gcc, see
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66425.
if cc.get_id() == 'gcc'
  basic_disabled_warnings_try += '-Wno-unused-result'
endif

## do we realy need this?
# if get_option('prefer_static')
#   basic_link_flags_try += get_option('b_pie') ? '-static-pie' : '-static'
# endif

if not get_option('b_pie')
  # eina_btlog cannot be compiled without dynamic section size
  basic_c_flags_try += ['-fPIE']
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
      basic_c_flags_try += arg
      basic_link_flags_try += arg
      break
    endif
  endforeach
  get_option('stack_protector') \
    .require(have_stack_protector, error_message: 'Stack protector not supported')
endif

if get_option('buildtype').startswith('debug')
  user_c_flags =  cc.get_supported_arguments(dev_disabled_warnings_try + dev_c_flags_try)
else
  user_c_flags  = cc.get_supported_arguments(basic_disabled_warnings_try + basic_c_flags_try)
endif

user_c_ld_args = cc.get_supported_link_arguments(basic_link_flags_try + dev_link_flags_try)

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

add_project_arguments(user_c_flags,   native: false, language: langs)
add_global_link_arguments(user_c_ld_args, native: false, language: langs)

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

summary_env += {'C compiler':        ''.join(meson.get_compiler('c').cmd_array(),' (',cc.get_id(),')')}
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