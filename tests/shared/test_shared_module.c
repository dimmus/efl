/*
 * Copyright (C) 2008 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

static Eina_Bool list_cb(Eina_Module *m, void *data EINA_UNUSED)
{
   int *sym;
   const char *file;

   /* the reference count */
   efl_assert_fail_if(!eina_module_load(m));
   /* get */
   sym = eina_module_symbol_get(m, "dummy_symbol");
   efl_assert_fail_if(!sym);
   efl_assert_fail_if(*sym != 0xbad);
   file = eina_module_file_get(m);
   efl_assert_fail_if(!file);
   efl_assert_fail_if(eina_module_unload(m));

   return EINA_TRUE;
}


TEST(module_load_unload)
{
   Eina_Array *_modules;
   unsigned int i;
   Eina_Array_Iterator it;
   Eina_Module *module;	
   
   _modules = eina_module_list_get(NULL,
                                   PACKAGE_BUILD_DIR "/src/tests/eina",
                                   EINA_TRUE,
                                   &list_cb,
                                   NULL);
   efl_assert_fail_if(!_modules);
   eina_module_list_load(_modules);
   eina_module_list_unload(_modules);
   eina_module_list_free(_modules);
   EINA_ARRAY_ITER_NEXT(_modules, i, module, it)
     free(module);
   eina_array_free(_modules);
}

TEST(module_find_test)
{
   Eina_Array *_modules;

   _modules = eina_module_list_get(NULL,
                                   PACKAGE_BUILD_DIR "/src/tests/eina",
                                   EINA_TRUE,
                                   &list_cb,
                                   NULL);
   efl_assert_fail_if(!_modules);
   efl_assert_fail_if(eina_module_find(_modules, NULL) != NULL);
}

void
eina_test_module(TCase *tc)
{
   tcase_add_test(tc, eina_module_load_unload);
   tcase_add_test(tc, eina_module_find_test);
}
