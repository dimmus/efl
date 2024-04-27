/*
 * Copyright (C) 2008 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

struct log_ctx
{
    int         level;
    int         line;
    const char *msg;
    const char *fnc;
    const char *dom;
    Efl_Bool    did;
};

/* tests should not output on success, just uncomment this for debugging */
//#define SHOW_LOG 1

static void
_eina_test_log(const Eina_Log_Domain *d,
               Eina_Log_Level         level,
               const char            *file,
               const char            *fnc,
               int                    line,
               const char            *fmt,
               void                  *data,
               va_list args           EFL_UNUSED)
{
    struct log_ctx *ctx = data;
    efl_assert_int_eq(ctx->level, level);
    efl_assert_int_eq(ctx->line, line);
    efl_assert_str_eq(ctx->msg, fmt);
    efl_assert_str_eq(ctx->fnc, fnc);
    efl_assert_str_eq(file, __FILE__);
    ctx->did = EFL_TRUE;
#ifdef SHOW_LOG
    eina_log_print_cb_stderr(d, level, file, fnc, line, fmt, NULL, args);
#else
    (void)d;
#endif
}

static void
_eina_test_log_domain(const Eina_Log_Domain *d,
                      Eina_Log_Level         level,
                      const char            *file,
                      const char            *fnc,
                      int                    line,
                      const char            *fmt,
                      void                  *data,
                      va_list args           EFL_UNUSED)
{
    struct log_ctx *ctx = data;
    efl_assert_int_eq(ctx->level, level);
    efl_assert_int_eq(ctx->line, line);
    efl_assert_str_eq(ctx->msg, fmt);
    efl_assert_str_eq(ctx->fnc, fnc);
    efl_assert_str_eq(file, __FILE__);
    efl_assert_str_eq(ctx->dom, d->name);
    ctx->did = EFL_TRUE;
#ifdef SHOW_LOG
    eina_log_print_cb_stderr(d, level, file, fnc, line, fmt, NULL, args);
#endif
}

static void
_eina_test_log_safety(const Eina_Log_Domain *d,
                      Eina_Log_Level         level,
                      const char            *file,
                      const char            *fnc,
                      int                    line,
                      const char            *fmt,
                      void                  *data,
                      va_list args           EFL_UNUSED)
{
    struct log_ctx *ctx = data;
    va_list         cp_args;
    const char     *str;

    va_copy(cp_args, args);
    str = va_arg(cp_args, const char *);
    va_end(cp_args);

    efl_assert_int_eq(ctx->level, level);
    efl_assert_str_eq(fmt, "%s");
    efl_assert_str_eq(ctx->msg, str);
    efl_assert_str_eq(ctx->fnc, fnc);
    ctx->did = EFL_TRUE;

#ifdef SHOW_LOG
    eina_log_print_cb_stderr(d, level, file, fnc, line, fmt, NULL, args);
#else
    (void)d;
    (void)file;
    (void)line;
#endif
}

TEST(log_macro)
{
    struct log_ctx ctx;
    int            oldlevel;
    int            maxlevel;

    oldlevel = eina_log_level_get();
    eina_log_level_set(EINA_LOG_LEVEL_DBG);
    eina_log_print_cb_set(_eina_test_log, &ctx);

#define TEST_LOG_CTX(lvl, _msg) \
    ctx.level = lvl;            \
    ctx.line  = __LINE__ + 1;   \
    ctx.msg   = _msg;           \
    ctx.fnc   = __func__;       \
    ctx.did   = EFL_FALSE

#ifdef EINA_LOG_LEVEL_MAXIMUM
    maxlevel = EINA_LOG_LEVEL_MAXIMUM;
#else
    maxlevel = EINA_LOG_LEVEL_DBG;
#endif

    if (EINA_LOG_LEVEL_CRITICAL <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_CRITICAL, "Critical message");
        EINA_LOG_CRIT("Critical message");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_ERR <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_ERR, "An error");
        EINA_LOG_ERR("An error");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_WARN <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_WARN, "A warning");
        EINA_LOG_WARN("A warning");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_INFO <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_INFO, "An info");
        EINA_LOG_INFO("An info");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_DBG <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_DBG, "A debug");
        EINA_LOG_DBG("A debug");
        assert(ctx.did);
    }

#undef TEST_LOG_CTX

    eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
    eina_log_level_set(oldlevel);
}

TEST(log_domains_macros)
{
    struct log_ctx ctx;
    int            oldlevel;
    int            maxlevel;

   /* make global log level blocker */
    oldlevel = eina_log_level_get();
    eina_log_level_set(EINA_LOG_LEVEL_CRITICAL);
    eina_log_print_cb_set(_eina_test_log_domain, &ctx);

    int d = eina_log_domain_register("MyDomain", EINA_COLOR_GREEN);
    efl_assert_fail_if(d < 0);

   /* make specific domain permissive */
    eina_log_domain_level_set("MyDomain", EINA_LOG_LEVEL_DBG);

#define TEST_LOG_CTX(lvl, _msg) \
    ctx.level = lvl;            \
    ctx.line  = __LINE__ + 1;   \
    ctx.msg   = _msg;           \
    ctx.fnc   = __func__;       \
    ctx.dom   = "MyDomain";     \
    ctx.did   = EFL_FALSE

#ifdef EINA_LOG_LEVEL_MAXIMUM
    maxlevel = EINA_LOG_LEVEL_MAXIMUM;
#else
    maxlevel = EINA_LOG_LEVEL_DBG;
#endif

    if (EINA_LOG_LEVEL_CRITICAL <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_CRITICAL, "A critical message");
        EINA_LOG_DOM_CRIT(d, "A critical message");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_ERR <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_ERR, "An error");
        EINA_LOG_DOM_ERR(d, "An error");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_WARN <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_WARN, "A warning");
        EINA_LOG_DOM_WARN(d, "A warning");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_INFO <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_INFO, "An info");
        EINA_LOG_DOM_INFO(d, "An info");
        assert(ctx.did);
    }

    if (EINA_LOG_LEVEL_DBG <= maxlevel)
    {
        TEST_LOG_CTX(EINA_LOG_LEVEL_DBG, "A debug");
        EINA_LOG_DOM_DBG(d, "A debug");
        assert(ctx.did);
    }

#undef TEST_LOG_CTX

    eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
    eina_log_level_set(oldlevel);
}

TEST(log_domains_registry)
{
    int i;
    int d[50];

    for (i = 0; i < 50; i++)
    {
        d[i] = eina_log_domain_register("Test", EINA_COLOR_GREEN);
        efl_assert_fail_if(d[i] < 0);
    }

    for (i = 0; i < 50; i++)
        eina_log_domain_unregister(d[i]);
}

TEST(log_domains_slot_reuse)
{
    efl_assert_fail_if(!eina_threads_init());

   // Create 9 domains
    int idx[9];
    int i;

    for (i = 0; i < 9; i++)
    {
        idx[i] = eina_log_domain_register("Test1", EINA_COLOR_GREEN);
        efl_assert_fail_if(idx[i] < 0);
    }

    // Slot 0 by default contains the global logger. The above code created
    // domains for slots indexes from 1 to 9.
    //
    // The global logger allocated the first 8 initial slots. The 8th domain
    // registered on the for loop will create 8 more slots.
    //
    // Test will just unregister a domain between 1 and 9 and assure that a new
    // domain register will be placed on the available slot and not at the end.

    int removed = idx[5];
    eina_log_domain_unregister(removed);

    int new = eina_log_domain_register("Test Slot", EINA_COLOR_GREEN);

    // Check for slot reuse
    efl_assert_fail_if(new != removed);

    eina_threads_shutdown();
}

TEST(log_level_indexes)
{
    struct log_ctx ctx;
    int            maxlevel;

    efl_assert_fail_if(!eina_threads_init());

    int d = eina_log_domain_register("Levels", EINA_COLOR_GREEN);
    efl_assert_fail_if(d < 0);

    eina_log_print_cb_set(_eina_test_log_domain, &ctx);

#define TEST_LOG_CTX(lvl, _msg) \
    ctx.level = lvl;            \
    ctx.line  = __LINE__ + 1;   \
    ctx.msg   = _msg;           \
    ctx.fnc   = __func__;       \
    ctx.dom   = "Levels";       \
    ctx.did   = EFL_FALSE;

#ifdef EINA_LOG_LEVEL_MAXIMUM
    maxlevel = EINA_LOG_LEVEL_MAXIMUM;
#else
    maxlevel = EINA_LOG_LEVEL_DBG;
#endif

    if (-1 <= maxlevel)
    {
        // Displayed unless user sets level lower than -1
        eina_log_domain_level_set("Levels", -1);
        TEST_LOG_CTX(-1, "Negative index message");
        EINA_LOG(d, -1, "Negative index message");
        assert(ctx.did);
    }

    if (-2 <= maxlevel)
    {
        eina_log_domain_level_set("Levels", -2);
        TEST_LOG_CTX(-1, "Negative index message");
        EINA_LOG(d, -1, "Negative index message");
        efl_assert_fail_if(ctx.did);
    }
#ifdef EINA_LOG_LEVEL_MAXIMUM
    if (6 <= maxlevel)
    {
        // Displayed only if user sets level 6 or higher
        eina_log_domain_level_set("Levels", 6);
        TEST_LOG_CTX(6, "Higher level debug");
        EINA_LOG(d, 6, "Higher level debug");
        assert(ctx.did);
    }
#endif
    if (5 <= maxlevel)
    {
        eina_log_domain_level_set("Levels", 5);
        TEST_LOG_CTX(6, "Higher level debug");
        EINA_LOG(d, 6, "Higher level debug");
        efl_assert_fail_if(ctx.did);
    }

#undef TEST_LOG_CTX

    eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);

    eina_threads_shutdown();
}

TEST(log_customize)
{
    struct log_ctx ctx;
    int            d;

    /* please don't define EINA_LOG_LEVELS for it */
#define TEST_DOM "_Test_Log_Dom"

#define test_set_get(func, val) \
    eina_log_##func##_set(val); \
    efl_assert_fail_if(eina_log_##func##_get() != val)

    test_set_get(level, -1234);
    test_set_get(level, 4567);

#define test_set_get_bool(func)    \
    test_set_get(func, EFL_FALSE); \
    test_set_get(func, EFL_TRUE)

    test_set_get_bool(color_disable);
    test_set_get_bool(file_disable);
    test_set_get_bool(function_disable);
    test_set_get_bool(abort_on_critical);

    test_set_get(abort_on_critical_level, -1234);
    test_set_get(abort_on_critical_level, 4567);

    efl_assert_fail_if(eina_log_domain_level_get(TEST_DOM) != eina_log_level_get());

    eina_log_domain_level_set(TEST_DOM, -123);
    efl_assert_fail_if(eina_log_domain_level_get(TEST_DOM) != -123);

    eina_log_domain_level_set(TEST_DOM, 890);
    efl_assert_fail_if(eina_log_domain_level_get(TEST_DOM) != 890);

    d = eina_log_domain_register(TEST_DOM, EINA_COLOR_GREEN);
    efl_assert_fail_if(d < 0);

    efl_assert_fail_if(eina_log_domain_level_get(TEST_DOM) != 890);
    efl_assert_fail_if(eina_log_domain_registered_level_get(d) != 890);

    eina_log_domain_unregister(d);

#ifdef EINA_SAFETY_CHECKS
#  ifdef SHOW_LOG
    fputs("NOTE: You should see a failed safety check or "
          "a crash if compiled without safety checks support.\n",
          stderr);
#  endif
    eina_log_abort_on_critical_set(EFL_FALSE);
    eina_log_function_disable_set(EFL_FALSE);

    eina_log_print_cb_set(_eina_test_log_safety, &ctx);
    ctx.level = EINA_LOG_LEVEL_ERR;
    ctx.msg   = "safety check failed: _log_domains[domain].deleted is true";
    ctx.fnc   = "eina_log_domain_registered_level_get";
    ctx.did   = EFL_FALSE;
    efl_assert_fail_if(eina_log_domain_registered_level_get(d) != EINA_LOG_LEVEL_UNKNOWN);
    assert(ctx.did);

    eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
#else
#  warning "Compiled without safety checks"
#endif

#undef test_set_get_bool
#undef test_set_get
}

TEST(log_level_name)
{
    char name[4];

#define tst(level, str)                   \
    eina_log_level_name_get(level, name); \
    efl_assert_fail_if(strcmp(name, str) != 0)

    tst(0, "CRI");
    tst(1, "ERR");
    tst(2, "WRN");
    tst(3, "INF");
    tst(4, "DBG");
    tst(5, "005");
    tst(12, "012");
    tst(369, "369");
    tst(-1, "-01");
    tst(-48, "-48");

#undef tst
}
