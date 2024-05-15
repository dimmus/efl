test_source = files(
  'signals_main.c',
  'signals_simple.c',
)

tests += [
	{	
		'name'    : 'eo_signals',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_signals_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]

