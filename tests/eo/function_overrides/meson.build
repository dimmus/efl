test_source = files(
  'function_overrides_simple.c',
  'function_overrides_inherit.c',
  'function_overrides_inherit2.c',
  'function_overrides_inherit3.c',
  'function_overrides_main.c',
)

tests += [
	{	
		'name'    : 'eo_function_overrides',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_function_overrides_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]
