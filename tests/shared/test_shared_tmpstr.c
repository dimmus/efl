/*
 * Copyright (C) 2013 Vlad Brovko
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <Efl_Shared.h>
#include "eina_tmpstr.h"

#include "efl-test-runner.h"

EFL_START_TEST(tmpstr_simple)
{

   const int cnt_tmp_strings = 10;
   const int max_str_len = 255;
   char buf[max_str_len + 1];
   Eina_Tmpstr *tmp_strings[cnt_tmp_strings];

   // Add several tmp strings
   for (int i = 0; i != cnt_tmp_strings; ++i)
     {
        snprintf(buf, max_str_len, "Tmp string %d", (i + 1));
        tmp_strings[i] = eina_tmpstr_add(buf);

        efl_assert_fail_if(strcmp(buf, tmp_strings[i]));
     }

   // Delete these tmp strings
   for (int i = 0; i != cnt_tmp_strings; ++i)
     {
        snprintf(buf, max_str_len, "Tmp string %d", (cnt_tmp_strings - i - 1 + 1));

        efl_assert_fail_if(strcmp(buf, tmp_strings[cnt_tmp_strings - i - 1]));

        eina_tmpstr_del(tmp_strings[cnt_tmp_strings - i - 1]);
        tmp_strings[cnt_tmp_strings - i - 1] = 0;
     }

   // Delete non tmp string (should do nothing)
   eina_tmpstr_del("Some non tmp string");

}

EFL_START_TEST(tmpstr_simple_len)
{

   const int cnt_tmp_strings = 10;
   const int max_str_len = 255;
   char buf[max_str_len + 1];
   Eina_Tmpstr *tmp_strings[cnt_tmp_strings];

   // Add several tmp strings
   for (int i = 0; i != cnt_tmp_strings; ++i)
     {
        snprintf(buf, max_str_len, "Tmp string %d", (i + 1));
        tmp_strings[i] = eina_tmpstr_add_length(buf, (max_str_len + 1));

        efl_assert_fail_if(strcmp(buf, tmp_strings[i]));
     }

   // Delete these tmp strings
   for (int i = 0; i != cnt_tmp_strings; ++i)
     {
        snprintf(buf, max_str_len, "Tmp string %d", (cnt_tmp_strings - i - 1 + 1));

        efl_assert_fail_if(strcmp(buf, tmp_strings[cnt_tmp_strings - i - 1]));

        eina_tmpstr_del(tmp_strings[cnt_tmp_strings - i - 1]);
        tmp_strings[cnt_tmp_strings - i - 1] = 0;
     }

   // Delete non tmp string (should do nothing)
   eina_tmpstr_del("Some non tmp string");

}

EFL_START_TEST(tmpstr_manage)
{

   char *buf = malloc(7);
   strcpy(buf, "tmpstr");
   Eina_Tmpstr *tstr1 = eina_tmpstr_manage_new(buf);
   efl_assert_fail_if(strcmp(buf, tstr1));
   eina_tmpstr_del(tstr1);

}

EFL_START_TEST(tmpstr_manage_len)
{

   char *buf = malloc(10);
   strcpy(buf, "tmpstr");
   Eina_Tmpstr *tstr1 = eina_tmpstr_manage_new_length(buf, 7);
   efl_assert_fail_if(strcmp(buf, tstr1));
   eina_tmpstr_del(tstr1);

}

EFL_START_TEST(tmpstr_len)
{

   const char *str1 = "12345";
   const char *str2 = "123456789";
   Eina_Tmpstr *tstr1 = eina_tmpstr_add(str1);
   Eina_Tmpstr *tstr2 = eina_tmpstr_add(str2);
   Eina_Tmpstr *tstr_empty = eina_tmpstr_add("");

   ck_assert_int_eq(eina_tmpstr_len(tstr1), strlen(str1));
   ck_assert_int_eq(eina_tmpstr_len(tstr2), strlen(str2));

   ck_assert_int_eq(eina_tmpstr_len(tstr_empty), 0);

   eina_tmpstr_del(tstr1);
   eina_tmpstr_del(tstr2);

}

void
eina_test_tmpstr(TCase *tc)
{
   tcase_add_test(tc, tmpstr_simple);
   tcase_add_test(tc, tmpstr_simple_len);
   tcase_add_test(tc, tmpstr_manage);
   tcase_add_test(tc, tmpstr_manage_len);
   tcase_add_test(tc, tmpstr_len);
}
