eo_gen_src = files(
  'main.c',
  'main.h',
  'types.c',
  'types.h',
  'headers.c',
  'headers.h',
  'sources.c',
  'sources.h',
  'docs.c',
  'docs.h'
)

eo_gen_bin = executable('efl_eo_gen',
  eo_gen_src,
  dependencies: [ eo_gen, shared ],
  install:      true,
  c_args :      package_c_args,
)

eo_gen_path = eo_gen_bin.full_path()

if meson.is_cross_build()
  _eo_gen_bin = find_program('efl_eo_gen', native : true)
  eo_gen_path = _eo_gen_bin.path()
else
  _eo_gen_bin = eo_gen_bin
  eo_gen_path = _eo_gen_bin.full_path()
endif

efl_eo_gen = [ _eo_gen_bin, '-S' ]

summary(
  {
    'path': eo_gen_path,
  },
  section: 'Eolian'
)