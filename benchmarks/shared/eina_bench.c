/* EINA - EFL data type library
 * Copyright (C) 2008 Cedric Bail
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "eina_bench.h"
#include "Efl_Shared.h"

typedef struct _Eina_Benchmark_Case Eina_Benchmark_Case;
struct _Eina_Benchmark_Case
{
   const char *bench_case;
   void (*build)(Eina_Benchmark *bench);
   Efl_Bool run_by_default;
};

static const Eina_Benchmark_Case etc[] = {
   { "Hash_Short_Key", eina_bench_crc_hash_short, EFL_TRUE },
   { "Hash_Medium_Key", eina_bench_crc_hash_medium, EFL_TRUE },
   { "Hash_Large_key", eina_bench_crc_hash_large, EFL_TRUE },
   { "Array vs List vs Inlist", eina_bench_array, EFL_TRUE },
   { "Stringshare", eina_bench_stringshare, EFL_TRUE },
   { "Convert", eina_bench_convert, EFL_TRUE },
   { "Sort", eina_bench_sort, EFL_TRUE },
   { "Mempool", eina_bench_mempool, EFL_TRUE },
   { "Rectangle_Pool", eina_bench_rectangle_pool, EFL_TRUE },
   { "Render Loop", eina_bench_quadtree, EFL_FALSE },
   { NULL, NULL, EFL_FALSE }
};

/* FIXME this is a copy from eina_test_mempool
 * we should remove the duplication
 */
static Eina_Array *_modules;
static void _mempool_init(void)
{
   eina_init();
   /* force modules to be loaded in case they are not installed */
   _modules = eina_module_list_get(NULL,
                                   EFL_BUILD_DIR "/src/modules/efl_utils",
                                   EFL_TRUE,
                                   NULL,
                                   NULL);
   eina_module_list_load(_modules);
}

static void _mempool_shutdown(void)
{
   eina_module_list_free(_modules);
   if (_modules)
     {
        Eina_Array_Iterator it;
        char* module;
        unsigned int i;

        EINA_ARRAY_ITER_NEXT(_modules, i, module, it)
          free(module);
        eina_array_free(_modules);
     }
   eina_shutdown();
}

int
main(int argc, char **argv)
{
   Eina_Benchmark *test;
   unsigned int i;

   _mempool_init();

   eina_init();

   switch (argc)
     {
      case 1:
         for (i = 0; etc[i].bench_case; ++i)
           if (etc[i].run_by_default)
             {
                test = eina_benchmark_new(etc[i].bench_case, "default");
                if (!test) continue;

                etc[i].build(test);

                eina_benchmark_run(test);

                eina_benchmark_free(test);
             }
         break;
      case 2:
         for (i = 0; etc[i].bench_case; ++i)
           {
              if (strcasecmp(etc[i].bench_case, argv[1]))
                continue;

              test = eina_benchmark_new(etc[i].bench_case, argv[1]);
              if (!test)
                continue;

              etc[i].build(test);

              eina_benchmark_run(test);

              eina_benchmark_free(test);

              break;
           }
         break;
     }

   eina_shutdown();

   _mempool_shutdown();
   return 0;
}
