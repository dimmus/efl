test_source = files(
  'constructors_main.c',
  'constructors_mixin.c',
  'constructors_mixin.h',
  'constructors_simple.c',
  'constructors_simple.h',
  'constructors_simple2.c',
  'constructors_simple2.h',
  'constructors_simple3.c',
  'constructors_simple3.h',
  'constructors_simple4.c',
  'constructors_simple4.h',
  'constructors_simple5.c',
  'constructors_simple5.h',
  'constructors_simple6.c',
  'constructors_simple6.h',
  'constructors_simple7.c',
  'constructors_simple7.h'
)

tests += [
	{	
		'name'    : 'eo_constructors',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_constructors_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]