#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#define EINA_SLSTR_INTERNAL
#include <Efl_Shared.h>

#include "efl-test-runner.h"

static Eina_Slstr *
_slstr_copy(void)
{
    const char local[] = "Hello world 1";

    return eina_slstr_copy_new(local);
}

TEST(slstr_copy)
{
    Eina_Slstr *str;

    str = _slstr_copy();
    efl_assert_str_eq(str, "Hello world 1");
}

static Eina_Slstr *
_slstr_steal(void)
{
    char *local = strdup("Hello world 2");

    return eina_slstr_steal_new(local);
}

TEST(slstr_steal)
{
    Eina_Slstr *str;

    str = _slstr_steal();
    efl_assert_str_eq(str, "Hello world 2");
}

static Eina_Slstr *
_slstr_stringshare(void)
{
    Eina_Stringshare *str = eina_stringshare_add("Hello world 3");

    return eina_slstr_stringshare_new(str);
}

TEST(slstr_stringshare)
{
    Eina_Stringshare *ss;
    Eina_Slstr       *str;

    str = _slstr_stringshare();
    ss  = eina_stringshare_add("Hello world 3");
    efl_assert_fail_if(ss != str);
}

static Eina_Slstr *
_slstr_tmpstr(void)
{
    Eina_Tmpstr *str = eina_tmpstr_add("Hello world 4");

    return eina_slstr_tmpstr_new(str);
}

TEST(slstr_tmpstr)
{
    Eina_Slstr *str;

    str = _slstr_tmpstr();
    efl_assert_str_eq(str, "Hello world 4");
}

static Eina_Slstr *
_slstr_strbuf(void)
{
    Efl_Strbuf *str = eina_strbuf_new();

    eina_strbuf_append(str, "Hello ");
    eina_strbuf_append(str, "world ");
    eina_strbuf_append_printf(str, "%d", 5);

    return eina_slstr_strbuf_new(str);
}

TEST(slstr_strbuf)
{
    Eina_Slstr *str;

    str = _slstr_strbuf();
    efl_assert_str_eq(str, "Hello world 5");
}

static Eina_Slstr *
_slstr_printf(int val)
{
    return eina_slstr_printf("Hello %s %d", "world", val);
}

TEST(slstr_slstr_printf)
{
    Eina_Slstr *str;

    str = _slstr_printf(6);
    efl_assert_str_eq(str, "Hello world 6");
}

static void
_many_do(Efl_Bool threaded)
{
    const int   many = threaded ? 256 : 2048;
    Eina_Slstr *str;
    int         k;

    for (k = 0; k < many; k++)
    {
        char local[64];

        str = _slstr_printf(k);
        sprintf(local, "Hello world %d", k);
        efl_assert_str_eq(str, local);
    }
}

TEST(slstr_many)
{
    _many_do(0);

    eina_slstr_local_clear();
}

static void *
_thread_cb(void *data EFL_UNUSED, Eina_Thread th EFL_UNUSED)
{
    _many_do(1);

    return NULL;
}

TEST(slstr_thread)
{
    const int   threads = 8;
    Eina_Thread th[threads];
    int         k;

    for (k = 0; k < threads; k++)
        efl_assert_fail_if(!eina_thread_create(&th[k],
                                               EINA_THREAD_NORMAL,
                                               -1,
                                               _thread_cb,
                                               NULL));

    for (k = 0; k < threads; k++)
        eina_thread_join(th[k]);

    eina_slstr_local_clear();
}

#define STRL "Hello world 7"

static Eina_Slstr *
_slstr_length(unsigned int length)
{
    Eina_Tmpstr *str = eina_tmpstr_add(STRL);

    return eina_slstr_copy_new_length(str, length);
}

TEST(slstr_length)
{
    unsigned int i;

    for (i = 0; i < strlen(STRL); i++)
    {
        Eina_Slstr *slstr;
        char       *str;

        slstr = _slstr_length(i);
        str   = eina_strndup(STRL, i);
        efl_assert_str_eq(str, slstr);

        free(str);
    }

    eina_slstr_local_clear();
}
