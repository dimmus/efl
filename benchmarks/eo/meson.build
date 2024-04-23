if get_option('build-benchmarks')
  eo_benchmark_src = [
    'class_simple.c',
    'class_simple.h',
    'eo_bench.c',
    'eo_bench.h',
    'eo_bench_callbacks.c',
    'eo_bench_eo_do.c',
    'eo_bench_eo_add.c'
  ]

  eo_bench = executable('eo_bench',
    eo_benchmark_src,
    dependencies: [eo, shared],
  )

  benchmark('eo', eo_bench,
    args: run_command('date','+%F_%s', check: true).stdout()
  )
endif
