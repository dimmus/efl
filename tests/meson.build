lib_test_runner = static_library(
	'test-runner',
	'efl-test-runner.c',
	dependencies: [
                dep_libdl,
                dep_threads,
				shared
	],
	include_directories: [ inc_config, include_directories('.'), inc_shared ],
	install: false,
)

dep_test_runner = declare_dependency(
	dependencies: shared,
	link_with: lib_test_runner
)

# test_config_h = configuration_data()
# test_config_h.set_quoted('WESTON_TEST_REFERENCE_PATH', meson.current_source_dir() + '/reference')
# test_config_h.set_quoted('WESTON_MODULE_MAP', env_modmap)
# test_config_h.set_quoted('WESTON_DATA_DIR', join_paths(meson.current_source_dir(), '..', 'data'))
# test_config_h.set_quoted('TESTSUITE_PLUGIN_PATH', exe_plugin_test.full_path())
# # test_config_h.set10('WESTON_TEST_SKIP_IS_FAILURE', get_option('test-skip-is-failure'))
# configure_file(output: 'test-config.h', configuration: test_config_h)

test_env = environment()
test_env.set('EFL_RUN_IN_TREE', '1')

# there are some leaks in fontconfig we can't fix; 
# if get_option('b_sanitize') in ['address', 'address,undefined' ]
#   test_env += { 'LSAN_OPTIONS': 'suppressions=@0@'.format(dir_gitlab_ci / 'leak-sanitizer.supp') }
# endif

if get_option('b_sanitize') == 'address'
	test_env.set('ASAN_OPTIONS', 'detect_leaks=0:detect_odr_violation=0')
endif

tests = []

foreach dir : dir_test
	# package_c_args = [
	# 	'-DPACKAGE_DATA_DIR="'+ join_paths(dir_data, dir)+'"',
	# 	'-DNEED_RUN_IN_TREE=1',
	# ]
	subdir(dir)
endforeach

# tests = [
	# { 'name': 'assert', },
	# { 'name': 'color-metadata-errors', 'dep_objs': dep_libexec_weston, },
	# { 'name': 'constraints', 'sources': [ 'constraints-test.c',
	# 									  pointer_constraints_unstable_v1_protocol_c, ]},  
	# {	'name': 'drm-smoke', 'run_exclusive': true },
	# {
	# 	'name': 'output-capture-protocol',
	# 	'sources': [
	# 		'output-capture-protocol-test.c',
	# 		weston_output_capture_protocol_c,
	# 		weston_output_capture_client_protocol_h,
	# 	],
	# 	'dep_objs': [ dep_libdrm_headers ],
	# },
# ]

# tests_standalone = [
# 	['config-parser', [], [ dep_zucmain ]],
# 	['timespec', [], [ dep_zucmain ]],
# 	['zuc',
# 		[
# 			'../tools/zunitc/test/fixtures_test.c',
# 			'../tools/zunitc/test/zunitc_test.c'
# 		],
# 		[ dep_zucmain ]
# 	],
# ]

# if get_option('shell-ivi')
# 	ivi_layout_test_plugin = shared_library(
# 		'test-ivi-layout',
# 		[
# 			'ivi-layout-test-plugin.c',
# 			weston_test_server_protocol_h,
# 			weston_test_protocol_c,
# 		],
# 		include_directories: common_inc,
# 		dependencies: [ dep_libweston_private, dep_libexec_weston ],
# 		name_prefix: '',
# 		install: false,
# 	)
# 	env_modmap += 'test-ivi-layout.so=' + ivi_layout_test_plugin.full_path() + ';'

# 	tests += [
# 		{
# 			'name': 'ivi-layout-client',
# 			'sources': [
# 				'ivi-layout-test-client.c',
# 				ivi_application_client_protocol_h,
# 				ivi_application_protocol_c,
# 			],
# 			'test_deps': [ ivi_layout_test_plugin ],
# 		},
# 		{	'name': 'ivi-layout-internal', },
# 		{
# 			'name': 'ivi-shell-app',
# 			'sources': [
# 				'ivi-shell-app-test.c',
# 				ivi_application_client_protocol_h,
# 				ivi_application_protocol_c,
# 			],
# 		},
# 	]
# endif

foreach t : tests
	t_name = 'efl_' + t.get('name')
	# t_sources = t.get('sources', [t.get('name') + '-test.c'])
	t_sources = t.get('sources')
	# t_sources += weston_test_client_protocol_h

	t_deps = [ dep_test_runner ]
	t_deps += t.get('dep_objs', [])

	run_exclusive = t.get('run_exclusive', false)

	message('NAME: @0@, SOURCES: @1@, INC: @2@'.format(t_name, t_sources, t.get('inc_dirs')))

	t_exe = executable(
		t_name,
		t_sources,
		c_args: [
			'-DTEST_NAME="' + t_name + '"',
			'-DTESTS_BUILD_DIR="' + meson.current_build_dir()  + '/' + t.get('name') + '"',
        	'-DTESTS_SRC_DIR="'   + meson.current_source_dir() + '/' + t.get('name') + '"',
		] + t.get('c_args', []),
		build_by_default: true,
		include_directories: [ include_directories('.'), inc_config, t.get('inc_dirs') ],
		dependencies: t_deps,
		link_with: t.get('link_with', []),
		install: false,
	)

	test(
		t.get('name'),
		t_exe,
        env: test_env,
		depends: t.get('test_deps', []),
		timeout: 120,
		protocol: 'tap',
		is_parallel: not run_exclusive
	)
endforeach
