if get_option('build-tests')
  check = dependency('check', version: '!= 0.15.1')

  test_env = environment()
  test_env.set('EFL_RUN_IN_TREE', '1')

  if get_option('b_sanitize') == 'address'
    test_env.set('ASAN_OPTIONS', 'detect_leaks=0:detect_odr_violation=0')
  endif

  subdir('tests')
  foreach test : dir_test
    package_c_args = [
      '-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, test)+'"',
      '-DNEED_RUN_IN_TREE=1',
    ]
    subdir(join_paths(local_tests, test))
  endforeach
endif

if get_option('build-examples')
  foreach example : dir_example
    package_c_args = [
      '-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, example)+'"',
      '-DNEED_RUN_IN_TREE=1',
    ]
    subdir(join_paths(local_examples, example))
  endforeach
endif