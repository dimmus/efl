/*
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Efl_Shared.h>

#include "efl-test-runner.h"

EFL_START_TEST(trash_simple)
{
   Eina_Trash *trash;
   unsigned int i;
   Eina_Array *array;
   int inp_int = 9;
   int inp_char = inp_int + '0';
   void *data;


   trash = calloc(1, sizeof(Eina_Trash));
   efl_assert_fail_if(trash == NULL);
   eina_trash_init(&trash);

   for (i = 1; i < 51; ++i)
     {
        array = eina_array_new(1);
        efl_assert_fail_if(!array);
        eina_array_push(array, &inp_int);
        eina_trash_push(&trash, array);
        array = eina_array_new(1);
        efl_assert_fail_if(!array);
        eina_array_push(array, &inp_char);
        eina_trash_push(&trash, array);
     }

   data = eina_trash_pop(&trash);
   efl_assert_fail_if(!data);
   efl_assert_fail_if(*((char *)eina_array_data_get(data, 0)) != inp_char);
   data = eina_trash_pop(&trash);
   efl_assert_fail_if(!data);
   efl_assert_fail_if(*((int *)eina_array_data_get(data, 0)) != inp_int);
   free(data);

   i = 0;
   EINA_TRASH_CLEAN(&trash, data)
     {
        efl_assert_fail_if(!data);
        free(data);
        ++i;
     }

   efl_assert_fail_if(i != 98);

}

void
eina_test_trash(TCase *tc)
{
   tcase_add_test(tc, trash_simple);
}
