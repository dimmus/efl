#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <Core_IMF.h>

#include "efl_core_suite.h"

EFL_START_TEST(core_test_core_imf_init)
{
   core_imf_init();
   core_imf_shutdown();
}
EFL_END_TEST

static const char *built_modules[] = {
#ifdef BUILD_CORE_IMF_XIM
  "xim",
#endif
#ifdef BUILD_CORE_IMF_IBUS
  "ibus",
#endif
#ifdef BUILD_CORE_IMF_SCIM
/* The scim module needs some configuration on the host or it might just block
 * the whole test suite when it tries to load the module. Disabling it as we
 * do not have control over the host.
  "scim", */
#endif
  NULL
};

static Efl_Bool
_find_list(const Eina_List *lst, const char *item)
{
   const Eina_List *n;
   const char *s;

   /* these modules (currently) require x11 to run */
   if (eina_streq(item, "xim") || eina_streq(item, "ibus"))
     {
        if (!getenv("DISPLAY")) return EFL_TRUE;
     }
   EINA_LIST_FOREACH(lst, n, s)
     {
        if (strcmp(s, item) == 0)
          return EFL_TRUE;
     }
   return EFL_FALSE;
}

EFL_START_TEST(core_test_core_imf_modules)
{
   Eina_List *modules;
   const char **itr;
   char *failure = NULL;

   putenv("CORE_IMF_MODULE=");
   core_imf_init();
   modules = core_imf_context_available_ids_get();

   for (itr = built_modules; *itr != NULL; itr++)
     {
        Efl_Bool found = _find_list(modules, *itr);
        if (!found) failure = eina_strdup(*itr);
        if (failure) break;
     }

   eina_list_free(modules);
   core_imf_shutdown();
   ck_assert_msg(!failure, "compiled imf module not found: %s", failure);
}
EFL_END_TEST

EFL_START_TEST(core_test_core_imf_modules_load)
{
   const char **itr;
   char *failure = NULL;

   putenv("CORE_IMF_MODULE=");
   core_imf_init();
   for (itr = built_modules; *itr != NULL; itr++)
     {
        Core_IMF_Context *ctx;

        /* these modules (currently) require x11 to run */
        if (eina_streq(*itr, "xim") || eina_streq(*itr, "ibus"))
          {
             if (!getenv("DISPLAY")) continue;
          }

        ctx = core_imf_context_add(*itr);
        if (!ctx)
          {
             failure = eina_strdup(*itr);
             break;
          }
        core_imf_context_del(ctx);
     }

   core_imf_shutdown();
   ck_assert_msg(!failure, "could not add imf context: %s", failure);
}
EFL_END_TEST

void core_test_core_imf(TCase *tc)
{
   tcase_add_test(tc, core_test_core_imf_init);
   tcase_add_test(tc, core_test_core_imf_modules);
   tcase_add_test(tc, core_test_core_imf_modules_load);
}
