test_source = files(
  'composite_objects_comp.c',
  'composite_objects_comp.h',
  'composite_objects_main.c',
  'composite_objects_simple.c',
  'composite_objects_simple.h'
)

tests += [
	{	
		'name'    : 'eo_composite_objects',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_composite_objects_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]
