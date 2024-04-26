examples = [
  'ecore_event_example_01',
  'ecore_event_example_02',
  'ecore_exe_example',
  'ecore_exe_example_child',
  'ecore_fd_handler_example',
  'ecore_idler_example',
  'ecore_job_example',
  'ecore_poller_example',
  'ecore_thread_example',
  'ecore_time_functions_example',
  'ecore_timer_example',
  'ecore_getopt_example',
]

if efl_config.has('HAVE_FORK')
  examples += 'ecore_pipe_simple_example'
endif

foreach example : examples
  executable(example, example + '.c', dependencies: [ shared, core ])
endforeach

# examples = [
#   'ecore_file_download_example',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [shared, ecore, ecore_file])
# endforeach

# if get_option('gstreamer')
#   examples = [
#     'ecore_pipe_gstreamer_example',
#   ]
#   gstreamer = dependency('gstreamer-1.0')
#   foreach example : examples
#     executable(example, example + '.c', dependencies: [shared, ecore, gstreamer, ecore_file])
#   endforeach
# endif

# examples = [
#   'ecore_ipc_client_example',
#   'ecore_ipc_server_example',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [eina, ecore, ecore_ipc])
# endforeach

# examples = [
#   'ecore_audio_custom',
#   'ecore_audio_playback',
#   'ecore_audio_to_ogg',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [eina, ecore, ecore_audio])
# endforeach


# examples = [
#   'ecore_con_client_example',
#   'ecore_con_client_simple_example',
#   'ecore_con_lookup_example',
#   'ecore_con_server_example',
#   'ecore_con_server_http_example',
#   'ecore_con_server_simple_example',
#   'ecore_con_url_cookies_example',
#   'ecore_con_url_download_example',
#   'ecore_con_url_headers_example',
#   'ecore_client_bench',
#   'ecore_server_bench',
# ]

# foreach example : examples
#   executable(example, example + '.c', dependencies: [eina, ecore, ecore_con])
# endforeach

# examples = [
#   'ecore_animator_example',
#   'ecore_evas_basics_example',
#   'ecore_evas_buffer_example_01',
#   'ecore_evas_buffer_example_02',
#   'ecore_evas_callbacks',
#   'ecore_evas_object_example',
#   'ecore_evas_window_sizes_example',
#   'ecore_buffer_consumer_example',
#   'ecore_buffer_example',
#   'ecore_buffer_provider_example',
#   'ecore_compose_get_example',
# ]

# foreach example : examples
#   executable(example, example + '.c',
#     dependencies: [eina, ecore_evas],
#     c_args : [
#       '-DPACKAGE_EXAMPLES_DIR="'+meson.current_source_dir()+'"']
#   )
# endforeach

examples = [
  'efl_exe',
  'efl_thread',
]

foreach example : examples
  # executable(example, example + '.c', dependencies: [eina, ecore, ecore_con])
  executable(example, example + '.c', dependencies: [shared, core])
endforeach

#  'ecore_imf_example',
