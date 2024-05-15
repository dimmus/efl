test_source = files(
  'mixin_inherit.c',
  'mixin_main.c',
  'mixin_mixin.c',
  'mixin_mixin2.c',
  'mixin_mixin3.c',
  'mixin_mixin4.c',
  'mixin_simple.c',
)

tests += [
	{	
		'name'    : 'eo_mixin',
		'sources' : test_source,
		'dep_objs': [ eo ],
		'inc_dirs': inc_eo,
	},
]

tests += [
	{	
		'name'    : 'eo_mixin_dbg',
		'sources' : test_source,
		'dep_objs': [ eo_dbg ],
		'inc_dirs': inc_eo,
	},
]
