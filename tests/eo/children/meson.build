test_source = files(
  'children_main.c',
  'children_simple.c',
  'children_simple.h'
)

tests += [
	{	
		'name'    : 'eo_children',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_children_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]