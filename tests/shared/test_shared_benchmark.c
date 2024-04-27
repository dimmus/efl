#include "eina_benchmark.h"

#include "efl-test-runner.h"

static int global_test = 0;

static void
_eina_benchmark_specimens(int request)
{
    int i;
    int a = 0;

    for (i = 0; i < request; ++i)
        a += i;

    global_test = a;
}

TEST(benchmark_simple)
{
    Eina_Benchmark     *eb;
    Eina_Array_Iterator it;
    Eina_Array         *ea;
    char               *tmp;
    unsigned int        i;

    eb = eina_benchmark_new("benchmark", "test");
    efl_assert_fail_if(!eb);

    eina_benchmark_register(eb,
                            "specimens_check",
                            EINA_BENCHMARK(_eina_benchmark_specimens),
                            1000,
                            1100,
                            100);

    ea = eina_benchmark_run(eb);
    efl_assert_fail_if(!ea);

    EINA_ARRAY_ITER_NEXT(ea, i, tmp, it)
    {
        efl_assert_fail_if(!tmp);
        efl_assert_fail_if(unlink(tmp));
    }

    efl_assert_fail_if(global_test != 499500);

    eina_benchmark_free(eb);
}