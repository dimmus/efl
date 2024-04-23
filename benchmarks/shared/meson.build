if get_option('build-benchmarks')
  bench_src_shared = files(
    'eina_bench.c',
    'eina_bench_sort.c',
    'eina_bench_crc_hash.c',
    'eina_bench_stringshare.c',
    'eina_bench_convert.c',
    'eina_bench_mempool.c',
    'eina_bench_array.c',
    'eina_bench_rectangle_pool.c',
    'ecore_list.c',
    'ecore_sheap.c',
    'evas_hash.c',
    'evas_list.c',
    'evas_mempool.c',
    'evas_object_list.c',
    'evas_stringshare.c',
    'eina_bench_quad.c',
    'eina_bench.h',
    'Ecore_Data.h',
    'Evas_Data.h',
    'evas_mempool.h'
  )

  city = shared_library('city',
    sources : ['city.cc','city.h'],
  )

  bench_shared = executable('eina_bench',
    sources : bench_src_shared,
    dependencies : shared,
    c_args : ['-fPIC','-DCITYHASH_BENCH', '-DEINA_ENABLE_BENCH_E17', '-DEFL_BUILD'],
    include_directories : inc_config,
    link_with : city,
  )

  benchmark('bench_shared', bench_shared,
    args : 'libcity.la',
  )
endif