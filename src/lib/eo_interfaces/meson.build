dep_eo_interfaces     = [ m ]
dep_eo_interfaces_pub = [ eo ]
dep_eo_interfaces_ext = []
src_eo_interfaces     = []
inc_eo_interfaces     = include_directories('.')

eo_gen_include_directories += ['-I', dir_current_source]
eo_gen_include_directories += ['-I', join_paths(dir_source, 'src/lib')]

dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

pub_legacy_eo_files = [
  'efl_gfx_fill.eo',
  'efl_gfx_entity.eo',
  'efl_gfx_color.eo',
  'efl_gfx_image.eo',
  'efl_gfx_frame_controller.eo',
  'efl_ui_draggable.eo',
  'efl_ui_scrollable.eo',
  'efl_ui_scrollbar.eo',
  'efl_ui_container_selectable.eo',
  'efl_ui_zoom.eo',
]

pub_eo_file_target = []
foreach eo_file : pub_legacy_eo_files
  pub_eo_file_target += custom_target('eo_gen_' + eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : join_paths(dir_package_include, 'interfaces'),
    command :     efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

pub_eo_files = [
  'efl_playable.eo',
  'efl_config.eo',
  'efl_control.eo',
  'efl_duplicate.eo',
  'efl_file.eo',
  'efl_file_save.eo',
  'efl_gfx_image_load_controller.eo',
  'efl_part.eo',
  'efl_player.eo',
  'efl_audio_control.eo',
  'efl_text.eo',
  'efl_text_font_properties.eo',
  'efl_text_style.eo',
  'efl_text_format.eo',
  'efl_text_markup.eo',
  'efl_text_markup_util.eo',
  'efl_gfx_stack.eo',
  'efl_gfx_view.eo',
  'efl_gfx_buffer.eo',
  'efl_gfx_path.eo',
  'efl_gfx_shape.eo',
  'efl_gfx_gradient.eo',
  'efl_gfx_gradient_linear.eo',
  'efl_gfx_gradient_radial.eo',
  'efl_gfx_filter.eo',
  'efl_gfx_blur.eo',
  'efl_gfx_hint.eo',
  'efl_model.eo',
  'efl_model_provider.eo',
  'efl_interpolator.eo',
  'efl_gfx_image_orientable.eo',
  'efl_container.eo',
  'efl_content.eo',
  'efl_pack.eo',
  'efl_pack_layout.eo',
  'efl_pack_linear.eo',
  'efl_pack_table.eo',
  'efl_gfx_arrangement.eo',
  'efl_ui_i18n.eo',
  'efl_ui_layout_orientable.eo',
  'efl_ui_layout_orientable_readonly.eo',
  'efl_ui_drag.eo',
  'efl_ui_range_display.eo',
  'efl_ui_range_interactive.eo',
  'efl_ui_view.eo',
  'efl_ui_property_bind.eo',
  'efl_ui_factory.eo',
  'efl_ui_view_factory.eo',
  'efl_ui_factory_bind.eo',
  'efl_screen.eo',
  'efl_io_closer.eo',
  'efl_io_positioner.eo',
  'efl_io_reader.eo',
  'efl_io_sizer.eo',
  'efl_io_writer.eo',
  'efl_io_buffer.eo',
  'efl_io_queue.eo',
  'efl_observer.eo',
  'efl_observable.eo',
  'efl_ui_autorepeat.eo',
  'efl_gfx_color_class.eo',
  'efl_gfx_text_class.eo',
  'efl_gfx_size_class.eo',
  'efl_cached_item.eo',
  'efl_input_text_entity.eo',
]

foreach eo_file : pub_eo_files
  pub_eo_file_target += custom_target('eo_gen_' + eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : join_paths(dir_package_include, 'interfaces'),
    command :     efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

pub_eo_files += pub_legacy_eo_files

pub_eo_types_files = [
  'efl_gfx_types.eot',
  'efl_ui_drag_types.eot',
  'efl_types.eot',
  'efl_text_types.eot',
]

foreach eo_file : pub_eo_types_files
  pub_eo_file_target += custom_target('eo_gen_' + eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : join_paths(dir_package_include, 'interfaces'),
    command :     efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-ghd', '@INPUT@'])
endforeach

if get_option('install-eo-files')
  install_data(pub_eo_files + pub_legacy_eo_files + pub_eo_types_files,
    install_dir: join_paths(eo_gen_include_dir, package_version_name)
  )
endif

# efl_interfaces_eo_files  = pub_eo_files
# efl_interfaces_eot_files = pub_eo_types_files

pub_eo_files       = []
pub_eo_types_files = []

src_eo_interfaces += files([
  'efl_interfaces_main.c',
  'efl_mvvm_common.c',
  'efl_gfx_path.c',
  'efl_gfx_shape.c',
  'efl_gfx_color.c',
  'efl_io_closer.c',
  'efl_io_positioner.c',
  'efl_io_reader.c',
  'efl_io_sizer.c',
  'efl_io_writer.c',
  'efl_io_buffer.c',
  'efl_io_queue.c',
  'efl_observer.c',
  'efl_file.c',
  'efl_ui_layout_orientable_readonly.c',
  'efl_text_markup_util.c',
  'efl_model_provider.c',
])

install_headers('efl_file.h',
  install_dir : join_paths(dir_package_include, 'interfaces'),
)

lib_eo_interfaces = library('efl_eo_interfaces',
  src_eo_interfaces, pub_eo_file_target,
  c_args :            package_c_args,
  dependencies:       [ dep_eo_interfaces, dep_eo_interfaces_pub, dep_eo_interfaces_ext ],
  install:            true,
  version :           version
)

eo_interfaces = declare_dependency(
  include_directories:  [ inc_eo_interfaces, inc_config ],
  link_with:            lib_eo_interfaces,
  dependencies:         dep_eo_interfaces + dep_eo_interfaces_pub,
  sources :             pub_eo_file_target
)

package_eo_subdirs += ['eo_interfaces']

dep_eo_interfaces_one = dep_eo_interfaces + dep_eo_interfaces_pub + dep_eo_interfaces_ext

# headers

eo_interfaces_header_src = [
  'Efl_Eo_Interfaces.h',
  'Efl_Model.h'
]

install_headers(eo_interfaces_header_src,
  install_dir : dir_package_include,
)