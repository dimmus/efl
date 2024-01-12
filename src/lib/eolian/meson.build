dep_eolian = [etool]

inc_eolian = include_directories('.')

src_eolian = [
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
]

lib_eolian = library('eolian', src_eolian,
    include_directories: inc_config + inc_etool,
    dependencies: etool,
    install: true,
    c_args : [package_c_args, '-DEOLIAN_BUILD'],
    version : version
)

eolian = declare_dependency(
  include_directories: include_directories('.'),
  link_with : lib_eolian,
  dependencies: etool,
)

eolian_include_dir = join_paths(dir_data, 'eolian', 'include')

eolian_header_src = [
  'eolian_api.h',
  'Eolian.h',
  'Eolian_Aux.h'
]

install_headers(eolian_header_src,
   install_dir : dir_package_include
)

dep_eolian_one = dep_eolian