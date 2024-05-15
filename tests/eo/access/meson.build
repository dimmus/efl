test_source = files(
  'access_inherit.c',
  'access_inherit.h',
  'access_main.c',
  'access_simple.c',
  'access_simple.h',
  'access_simple_protected.h'
)

tests += [
	{	
		'name'    : 'eo_access',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_access_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]