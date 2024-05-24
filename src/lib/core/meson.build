dir_current_source = meson.current_source_dir()
dir_current_build  = meson.current_build_dir()

dep_core_pub = [ shared, eo, eo_interfaces ]
dep_core_ext = [ intl, dep_libm ]
# dep_core_ext = [intl, m, buildsystem]

inc_core = include_directories('.')

legacy_eo_files = [
  'core_event_message.eo',
  'core_event_message_handler.eo',
]

eo_file_target = []
foreach eo_file : legacy_eo_files
  eo_file_target += custom_target(eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : dir_package_include,
    command :     efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

eo_files = [
  'efl_app.eo',
  'efl_loop.eo',
  'efl_loop_consumer.eo',
  'efl_loop_fd.eo',
  'efl_loop_handler.eo',
  'efl_loop_message.eo',
  'efl_loop_message_handler.eo',
  'efl_exe.eo',
  'efl_thread.eo',
  'efl_threadio.eo',
  'efl_appthread.eo',
  'efl_task.eo',
  'efl_io_closer_fd.eo',
  'efl_io_positioner_fd.eo',
  'efl_io_reader_fd.eo',
  'efl_io_sizer_fd.eo',
  'efl_io_writer_fd.eo',
  'efl_io_stdin.eo',
  'efl_io_stdout.eo',
  'efl_io_stderr.eo',
  'efl_io_file.eo',
  'efl_io_copier.eo',
  'efl_io_buffered_stream.eo',
  'efl_linear_interpolator.eo',
  'efl_accelerate_interpolator.eo',
  'efl_decelerate_interpolator.eo',
  'efl_sinusoidal_interpolator.eo',
  'efl_divisor_interpolator.eo',
  'efl_bounce_interpolator.eo',
  'efl_spring_interpolator.eo',
  'efl_cubic_bezier_interpolator.eo',
  'efl_loop_message_future_handler.eo',
  'efl_loop_message_future.eo',
  'efl_loop_model.eo',
  'efl_generic_model.eo',
  'efl_container_model.eo',
  'efl_boolean_model.eo',
  'efl_composite_model.eo',
  'efl_core_env.eo',
  'efl_core_proc_env.eo',
  'efl_core_command_line.eo',
  'efl_loop_timer.eo',
  'efl_filter_model.eo',
]

foreach eo_file : eo_files
  eo_file_target += custom_target(eo_file,
    input :       eo_file,
    output :      eo_file + '.h',
    depfile :     eo_file + '.d',
    install :     true,
    install_dir : dir_package_include,
    command :     efl_eo_gen + [ '-I', dir_current_source, eo_gen_include_directories,
                           '-o', 'h:' + join_paths(dir_current_build, eo_file + '.h'),
                           '-o', 'c:' + join_paths(dir_current_build, eo_file + '.c'),
                           '-o', 'd:' + join_paths(dir_current_build, eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach

eo_types_files = []

eo_gen_include_directories += ['-I', dir_current_source]

src_core_header = [
  'Efl_Core.h',
  'Efl_Core_Common.h',
  'Efl_Core_Eo.h',
  'Efl_Core_Getopt.h',
  'core.h',
  'core_legacy.h',
  'core_exe_eo.h',
  'core_exe_eo.legacy.h',
  'efl_general.h',
  'efl_loop_timer_eo.legacy.h',
]

src_core = files(
  'core.c',
  'core_alloc.c',
  'core_anim.c',
  'core_app.c',
  'core_events.c',
  'core_getopt.c',
  'core_glib.c',
  'core_idle_enterer.c',
  'core_idle_exiter.c',
  'core_idler.c',
  'core_job.c',
  'core_main.c',
  'core_event_message.c',
  'core_event_message_handler.c',
  'core_main_timechanges.c',
  'core_pipe.c',
  'core_poller.c',
  'core_time.c',
  'core_timer.c',
  'core_thread.c',
  'core_throttle.c',
  'core_exe.c',
  'efl_app.c',
  'efl_loop.c',
  'efl_loop_consumer.c',
  'efl_loop_fd.c',
  'efl_loop_handler.c',
  'efl_loop_message.c',
  'efl_loop_message_future.c',
  'efl_loop_message_handler.c',
  'efl_loop_message_future_handler.c',
  'efl_task.c',
  'efl_io_closer_fd.c',
  'efl_io_positioner_fd.c',
  'efl_io_reader_fd.c',
  'efl_io_sizer_fd.c',
  'efl_io_writer_fd.c',
  'efl_io_stdin.c',
  'efl_io_stdout.c',
  'efl_io_stderr.c',
  'efl_io_file.c',
  'efl_io_copier.c',
  'efl_io_buffered_stream.c',
  'efl_loop_model.c',
  'efl_generic_model.c',
  'efl_container_model.c',
  'efl_composite_model.c',
  'efl_boolean_model.c',
  'efl_filter_model.c',
  'efl_linear_interpolator.c',
  'efl_accelerate_interpolator.c',
  'efl_decelerate_interpolator.c',
  'efl_sinusoidal_interpolator.c',
  'efl_divisor_interpolator.c',
  'efl_bounce_interpolator.c',
  'efl_spring_interpolator.c',
  'efl_cubic_bezier_interpolator.c',
  'efl_exe.c',
  'efl_thread.c',
  'efl_appthread.c',
  'efl_threadio.c',
  'efl_appthread.c',
  'efl_core_env.c',
  'efl_core_proc_env.c',
  'efl_core_command_line.c',
)

if sys_windows
  #nothing for now ... needs testing
  src_core += files('core_exe_win32.c')
else
  src_core += files('core_signal.c', 'core_exe_posix.c')
endif

# if get_option('glib') and not sys_windows
#   core_ext_deps += dependency('glib-2.0')
#   core_ext_deps += dependency('gthread-2.0')
#   config_h.set('GLIB_INTEGRATION_ALWAYS', '1')
#   config_h.set('HAVE_GLIB', '1')
# endif

# if get_option('g-mainloop')
#   if not get_option('glib')
#     error('g-mainloop enabled by glib not. Please enable with -Dglib=true')
#   else
#     config_h.set('USE_G_MAIN_LOOP', '1')
#   endif
# endif

lib_core = library('efl_core',
  src_core, eo_file_target,
  dependencies:         [ dep_core_pub, dep_core_ext ],
  include_directories : [ inc_config, include_directories('../..')],
  install:              true,
  c_args :              package_c_args,
  version :             version
)

core = declare_dependency(
  include_directories:  inc_core,
  link_with:            lib_core,
  sources :             eo_file_target,
  dependencies:         dep_core_pub,
)

if get_option('install-eo-files')
  install_data(eo_files + eo_types_files,
    install_dir:        join_paths(eo_gen_include_dir, package_version_name)
  )
endif

install_headers(src_core_header,
  install_dir :         dir_package_include,
)

dep_core_one = dep_core_pub + dep_core_ext