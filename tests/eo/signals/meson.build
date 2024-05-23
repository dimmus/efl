dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

eo_signals_src = files(
  'signals_main.c',
  'signals_simple.c',
  'signals_simple.h'
)

eo_signals = executable('efl_eo_signals',
  eo_signals_src,
  dependencies: [ eo ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"'] )

test('efl_eo-signals', eo_signals,
  env : test_env
)

eo_signals = executable('efl_eo_signals_dbg',
  eo_signals_src,
  dependencies: [ eo_dbg ],
  c_args      : [ '-DTESTS_BUILD_DIR="'+dir_current_build+'"',
                  '-DTESTS_SRC_DIR="'+dir_current_source+'"'] )

test('efl_eo-signals-dbg', eo_signals,
  env : test_env
)
