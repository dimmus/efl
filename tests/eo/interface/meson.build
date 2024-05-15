test_source = files(
  'interface_interface.c',
  'interface_interface2.c',
  'interface_main.c',
  'interface_simple.c',
)

tests += [
	{	
		'name'    : 'eo_interface',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_interfaces_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]
