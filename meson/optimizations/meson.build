cpu_sse3 = false
cpu_neon = false
cpu_neon_intrinsics = false
native_arch_opt_c_args = [ ]

if host_machine.endian() == 'big'
  efl_config.set10('WORDS_BIGENDIAN', true)
endif

cpu_family = host_machine.cpu_family()
with_optimization = get_option('native-arch-optimization')
if with_optimization
  if cpu_family.startswith('x86')
    cpu_sse3 = true
    efl_config.set10('BUILD_MMX', true)
    efl_config.set10('BUILD_SSE3', true)
    if cc.get_id() != 'msvc'
      native_arch_opt_c_args = ['-msse4.1', '-mstackrealign']
      add_project_arguments('-DUSE_SSE41', language: langs)
      message(cpu_family + ' build - MMX + SSE4 enabled')
    else
      native_arch_opt_c_args = [ '-msse3' ]
      message(cpu_family + ' build - MMX + SSE3 enabled')
    endif
    if cpu_family == 'x86'
      native_arch_opt_c_args = [ '-msse2', '-mstackrealign', '-mfpmath=sse']
      message(cpu_family + ' build - MMX + SSE2 enabled')
    endif
  elif cpu_family == 'arm'
    cpu_neon = true
    efl_config.set10('BUILD_NEON', true)
    add_project_arguments('-mfpu=neon', language: 'c')
    add_project_arguments('-ftree-vectorize', language: 'c')
    message('ARM build - NEON enabled')
  elif cpu_family == 'aarch64'
    cpu_neon = true
    cpu_neon_intrinsics = true
    efl_config.set10('BUILD_NEON', true)
    efl_config.set10('BUILD_NEON_INTRINSICS', true)
    add_project_arguments('-ftree-vectorize', language: 'c')
    native_arch_opt_c_args = [ '-ftree-vectorize' ]
    message('ARM64 build - NEON + intrinsics enabled')
  elif cpu_family == 'ppc' or cpu_family == 'ppc64'
    efl_config.set10('BUILD_ALTIVEC', true)
    add_project_arguments('-ftree-vectorize', language: 'c')
    add_project_arguments('-maltivec', language: 'c')
    message('PPC/POWER build - ALTIVEC enabled')
  endif
endif

summary_opt +=  {'Enabled': with_optimization}
summary_opt += {'Platform': cpu_family}
summary_opt += {'Native cargs': native_arch_opt_c_args}