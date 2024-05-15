test_source = files(
  'eo_error_msgs.c',
  'eo_test_call_errors.c',
  'eo_test_class_behaviour_errors.c',
  'eo_test_class_errors.c',
  'eo_test_class_simple.c',
  'eo_test_class_singleton.c',
  'eo_test_domain.c',
  'eo_test_event.c',
  'eo_test_general.c',
  'eo_test_init.c',
  'eo_test_lifecycle.c',
  'eo_test_reflection.c',
  'eo_test_threaded_calls.c',
  'eo_test_value.c',
)

tests += [
	{	
		'name'    : 'eo_suite',
		'sources' : test_source,
		'dep_objs': [ shared, eo, valgrind ],
		'inc_dirs': inc_eo,
	},
]

# Bug here with '-DEO_DEBUG=1'
# tests += [
# 	{	
# 		'name'    : 'eo_suite_dbg',
# 		'sources' : test_source,
# 		'dep_objs': [ shared, eo_dbg, valgrind ],
# 		'inc_dirs': inc_eo,
#     'c_args'  : [ '-DEO_DEBUG=1' ]
# 	},
# ]

# tests += [
# 	{	
# 		'name'    : 'eo_suite_fallback',
# 		'sources' : test_source,
# 		'dep_objs': [ eo_dbg, valgrind ],
# 		'inc_dirs': inc_eo,
#     'c_args'  : [
#       '-DEO_DEBUG=1',
#       '-D_EO_ADD_FALLBACK_FORCE=1',
#     ]
# 	},
# ]
