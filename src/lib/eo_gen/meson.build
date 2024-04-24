dep_eo_gen = [ shared ]

inc_eo_gen = include_directories('.')

src_eo_gen = files(
  'eo_lexer.c',
  'eo_parser.c',
  'eolian.c',
  'eolian_database.c',
  'database_check.c',
  'database_class.c',
  'database_class_api.c',
  'database_function.c',
  'database_function_api.c',
  'database_function_parameter.c',
  'database_function_parameter_api.c',
  'database_type.c',
  'database_type_api.c',
  'database_implement.c',
  'database_implement_api.c',
  'database_constructor.c',
  'database_constructor_api.c',
  'database_event.c',
  'database_event_api.c',
  'database_part.c',
  'database_part_api.c',
  'database_expr.c',
  'database_expr_api.c',
  'database_var.c',
  'database_var_api.c',
  'database_validate.c',
  'eolian_aux.c'
)

lib_eo_gen = library('efl_eo_gen', src_eo_gen,
    include_directories: inc_config + inc_shared,
    dependencies: shared,
    install: true,
    c_args : [ package_c_args, '-DEOLIAN_BUILD' ],
    version : version
)

eo_gen = declare_dependency(
  include_directories:  inc_eo_gen,
  link_with :           lib_eo_gen,
  dependencies:         shared,
)

eo_gen_include_dir = join_paths(dir_data, 'eo_gen/include')

src_eo_gen_header = files(
  'eolian_api.h',
  'Efl_Eo_Gen.h',
  'Eolian_Aux.h'
)

install_headers(src_eo_gen_header,
   install_dir : dir_package_include
)

dep_eo_gen_one = dep_eo_gen