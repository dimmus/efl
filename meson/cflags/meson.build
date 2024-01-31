dev_cflags = []
dev_cflags_try = []

if get_option('buildtype').startswith('debug')
  dev_cflags_try += [
    # '-g3',
    # '-fsanitize-trap', # undefined reference to `__ubsan_handle*
    '-fvisibility=hidden',
    '-Wundef',
    '-Wunused',
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wmissing-parameter-type',
    '-Wmissing-braces',
    '-Wmissing-field-initializers',
    '-Wmissing-include-dirs',
    '-Wpointer-arith',
    '-Wcast-function-type',
    '-Wignored-qualifiers',
    '-Wnonnull',
    '-Wold-style-declaration',
    '-Wold-style-definition',
    '-Woverride-init',
    '-Wsign-compare','-Wno-vla',
    '-Wshift-negative-value',
    '-Wunused-but-set-parameter',
    '-Wunused-parameter',
    '-Wunused-function',
    '-Wunused-command-line-argument',
    '-Wimplicit-function-declaration',
    '-Wempty-body',
    '-Wlogical-op',
    '-Winit-self',
    '-Wfloat-equal',
    '-Wfloat-compare',
    '-Wendif-labels',
    '-Wstrict-aliasing=2',
    '-Woverflow',
    '-Wformat=1',
    '-Wendif-labels',
    '-Wwrite-strings',
    '-Wnested-externs',
    '-Wpacked',
    '-Winline',
    '-Wshadow',
    '-Wattributes',
    '-Wno-switch-enum', # too many in eolian
    '-Wno-conversion', # a lot here !!!
    '-Wno-double-promotion', # a lot here !!!
    '-Wno-discarded-qualifiers',
    '-Wno-missing-declarations',
    '-Wno-missing-prototypes',
    '-Wno-vla' # no in C90 standard

  ]
endif

dev_cflags = cc.get_supported_arguments(dev_cflags_try)