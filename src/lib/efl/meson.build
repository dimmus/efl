dep_efl = [m]
dep_efl_pub = [eo]
dep_efl_ext = []

inc_efl = [include_directories('.')]

efl_header_src = [
  'Efl.h',
  'Efl_MVVM_Common.h'
]

src_efl = []

subdir('interfaces')
package_header_subdirs += 'interfaces'

lib_efl = library('efl',
    src_efl, pub_eo_file_target,
    c_args : package_c_args,
    dependencies: [dep_efl, dep_efl_pub, dep_efl_ext],
    install: true,
    version : version
)

efl = declare_dependency(
  include_directories: [include_directories('.'), inc_config],
  link_with: lib_efl,
  dependencies: dep_efl + dep_efl_pub,
  sources : pub_eo_file_target
)

install_headers(efl_header_src,
  install_dir : dir_package_include,
)

package_eo_subdirs += ['interfaces']
