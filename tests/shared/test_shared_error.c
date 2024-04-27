/*
 * Copyright (C) 2008 Cedric Bail
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#  include <evil_private.h> /* setenv */
#endif

#include <Efl_Shared.h>

#include "efl-test-runner.h"

#define TEST_TEXT "The big test\n"

struct log_ctx
{
    const char *msg;
    const char *fnc;
    Efl_Bool    did;
};

/* tests should not output on success, just uncomment this for debugging */
//#define SHOW_LOG 1

static void
_eina_test_safety_print_cb(const Eina_Log_Domain *d,
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

    efl_assert_int_eq(level, EINA_LOG_LEVEL_ERR);
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

TEST(error_errno)
{
    int test;

    setenv("EINA_ERROR_LEVEL", "1", 0);

    test = eina_error_msg_register(TEST_TEXT);
    efl_assert_fail_if(!eina_error_msg_get(test));
    efl_assert_fail_if(strcmp(eina_error_msg_get(test), TEST_TEXT) != 0);

    eina_error_set(test);
    efl_assert_fail_if(eina_error_get() != test);

    eina_error_set(EBADF);
    efl_assert_int_eq(eina_error_get(), EBADF);
    efl_assert_str_eq(eina_error_msg_get(EBADF), strerror(EBADF));
}

TEST(error_test_find)
{
    int         test, r;
    const char *str;

   /* Make sure the value isn't already there. */
    r = eina_error_find(TEST_TEXT TEST_TEXT);
    efl_assert_int_eq(r, 0);

    test = eina_error_msg_register(TEST_TEXT TEST_TEXT);
    efl_assert_int_ne(test, 0);

    str = eina_error_msg_get(test);
    assert(str != NULL);
    efl_assert_str_eq(str, TEST_TEXT TEST_TEXT);

    eina_error_set(test);
    efl_assert_fail_if(eina_error_get() != test);

    r = eina_error_find(TEST_TEXT TEST_TEXT);
    efl_assert_int_eq(r, test);
}

TEST(error_test_modify)
{
    int         test, r;
    const char *str, smsg[] = "Do not copy this string";

    test = eina_error_msg_register("Some Test Error");
    efl_assert_int_ne(test, 0);

    str = eina_error_msg_get(test);
    assert(str != NULL);
    efl_assert_str_eq(str, "Some Test Error");

    eina_error_set(test);
    efl_assert_fail_if(eina_error_get() != test);

    assert(eina_error_msg_modify(test, "ABCDE"));

    r = eina_error_find("ABCDE");
    efl_assert_int_eq(r, test);

    test = eina_error_msg_static_register(smsg);
    efl_assert_int_ne(test, 0);

    str = eina_error_msg_get(test);
    assert(str != NULL);
    assert(str == smsg);

    assert(eina_error_msg_modify(test, "Change that!"));
    r = eina_error_find("Change that!");
    efl_assert_int_eq(r, test);
}

TEST(error_test_lots)
{
    char         buf[64];
    int          codes[512];
    unsigned int i;

    for (i = 0; i < sizeof(codes) / sizeof(codes[0]); i++)
    {
        snprintf(buf, sizeof(buf), "myerr-%u", i);
        codes[i] = eina_error_msg_register(buf);
        efl_assert_int_ne(codes[i], 0);
    }

    for (i = 0; i < sizeof(codes) / sizeof(codes[0]); i++)
    {
        int found;

        snprintf(buf, sizeof(buf), "myerr-%u", i);

        found = eina_error_find(buf);
        efl_assert_int_eq(codes[i], found);
    }
}

#ifdef EINA_SAFETY_CHECKS
TEST(error_test_failures)
{
    struct log_ctx ctx;
    Eina_Error     local_error;

    eina_log_print_cb_set(_eina_test_safety_print_cb, &ctx);

#  define TEST_MAGIC_SAFETY(fn, _msg) \
      ctx.msg = _msg;                 \
      ctx.fnc = fn;                   \
      ctx.did = EFL_FALSE

    TEST_MAGIC_SAFETY("eina_error_msg_register",
                      "safety check failed: msg == NULL");
    efl_assert_int_eq(eina_error_msg_register(NULL), 0);
    assert(ctx.did);

    TEST_MAGIC_SAFETY("eina_error_msg_static_register",
                      "safety check failed: msg == NULL");
    efl_assert_int_eq(eina_error_msg_static_register(NULL), 0);
    assert(ctx.did);

    TEST_MAGIC_SAFETY(
        "eina_error_msg_modify",
        "safety check failed: EINA_ERROR_REGISTERED_CHECK(error) is false");
    efl_assert_int_eq(eina_error_msg_modify(0, "X"), EFL_FALSE);

    TEST_MAGIC_SAFETY(
        "eina_error_msg_modify",
        "safety check failed: EINA_ERROR_REGISTERED_CHECK(error) is false");
    efl_assert_int_eq(eina_error_msg_modify(4096, "X"), EFL_FALSE);

    TEST_MAGIC_SAFETY("eina_error_msg_modify",
                      "safety check failed: msg == NULL");
    efl_assert_int_eq(eina_error_msg_modify(ENOMEM, NULL), EFL_FALSE);
    assert(ctx.did);

    local_error = eina_error_msg_static_register("Local error for test");
    efl_assert_int_ne(local_error, 0);

    TEST_MAGIC_SAFETY("eina_error_msg_modify",
                      "safety check failed: msg == NULL");
    efl_assert_int_eq(eina_error_msg_modify(local_error, NULL), EFL_FALSE);
    assert(ctx.did);

    efl_assert_str_eq(eina_error_msg_get(ENOMEM),
#  if defined(_WIN32) || defined(__sun)
                      "Not enough space");
#  else
                      "Cannot allocate memory");
#  endif

    TEST_MAGIC_SAFETY("eina_error_find", "safety check failed: msg == NULL");
    efl_assert_int_eq(eina_error_find(NULL), 0);
    assert(ctx.did);

    efl_assert_int_eq(eina_error_find("Non-existent Error..."), 0);

    efl_assert_fail_if(eina_error_msg_get(0));
    efl_assert_fail_if(eina_error_msg_get(4096));

    eina_log_print_cb_set(eina_log_print_cb_stderr, NULL);
}
#endif
