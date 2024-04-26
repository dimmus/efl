add_project_arguments(dev_cflags_cpp,  native: false, language: 'cpp')
add_project_arguments(dev_cflags_cc,   native: false, language: 'c')

add_global_link_arguments(dev_ldflags, native: false, language: langs)

add_project_arguments('-DHAVE_CONFIG_H=1',        language: langs)
add_project_arguments('-D_GNU_SOURCE=1',          language: langs)
add_project_arguments('-DEFL_BETA_API_SUPPORT=1', language: langs)
add_project_arguments('-DNEED_RUN_IN_TREE=1',     language: langs)
# add_project_arguments('-DELM_INTERNAL_API_ARGESFSDFEFC=1', language: langs)
