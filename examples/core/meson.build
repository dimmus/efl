examples = [
  'core_event_example_01',
  'core_event_example_02',
  'core_exe_example',
  'core_exe_example_child',
  'core_fd_handler_example',
  'core_idler_example',
  # 'core_job_example',    # legacy
  # 'core_poller_example', # legacy
  'core_thread_example',
  'core_time_functions_example',
  'core_timer_example',
  'core_getopt_example',
]

if efl_config.has('HAVE_FORK')
  examples += 'core_pipe_simple_example'
endif

foreach example : examples
  executable(example, example + '.c', dependencies: [ shared, core ])
endforeach

# examples = [
#   'core_file_download_example',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [shared, ecore, core_file])
# endforeach

# if get_option('gstreamer')
#   examples = [
#     'core_pipe_gstreamer_example',
#   ]
#   gstreamer = dependency('gstreamer-1.0')
#   foreach example : examples
#     executable(example, example + '.c', dependencies: [shared, ecore, gstreamer, core_file])
#   endforeach
# endif

# examples = [
#   'core_ipc_client_example',
#   'core_ipc_server_example',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [eina, ecore, core_ipc])
# endforeach

# examples = [
#   'core_audio_custom',
#   'core_audio_playback',
#   'core_audio_to_ogg',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [eina, ecore, core_audio])
# endforeach


# examples = [
#   'core_con_client_example',
#   'core_con_client_simple_example',
#   'core_con_lookup_example',
#   'core_con_server_example',
#   'core_con_server_http_example',
#   'core_con_server_simple_example',
#   'core_con_url_cookies_example',
#   'core_con_url_download_example',
#   'core_con_url_headers_example',
#   'core_client_bench',
#   'core_server_bench',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [eina, ecore, core_con])
# endforeach

# examples = [
#   'core_animator_example',
#   'core_evas_basics_example',
#   'core_evas_buffer_example_01',
#   'core_evas_buffer_example_02',
#   'core_evas_callbacks',
#   'core_evas_object_example',
#   'core_evas_window_sizes_example',
#   'core_buffer_consumer_example',
#   'core_buffer_example',
#   'core_buffer_provider_example',
#   'core_compose_get_example',
# ]

# foreach example : examples
#   executable(example, example + '.c',
#     dependencies: [eina, core_evas],
#     c_args : [
#       '-DPACKAGE_EXAMPLES_DIR="'+meson.current_source_dir()+'"']
#   )
# endforeach

examples = [
  'efl_exe',
  'efl_thread',
]

foreach example : examples
  # executable(example, example + '.c', dependencies: [eina, ecore, core_con])
  executable(example, example + '.c', dependencies: [shared, core])
endforeach

#  'core_imf_example',
