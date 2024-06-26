#ifndef _EO_ERROR_MSGS_H
#define _EO_ERROR_MSGS_H

#include "Efl_Eo.h"
#include "eo_suite.h"

/* The Max level to consider when working with the print cb. */
#define _EINA_LOG_MAX 2

/* #define SHOW_LOG 1 */

struct log_ctx
{
  const char *msg;
  const char *fnc;
  Efl_Bool    did;
  int         expected_level;
};

void eo_test_print_cb(const Eina_Log_Domain *d,
                      Eina_Log_Level         level,
                      const char            *file,
                      const char            *fnc,
                      int                    line,
                      const char            *fmt,
                      void                  *data,
                      va_list args           EFL_UNUSED);

void eo_test_safety_print_cb(const Eina_Log_Domain *d,
                             Eina_Log_Level         level,
                             const char            *file,
                             const char            *fnc,
                             int                    line,
                             const char            *fmt,
                             void                  *data,
                             va_list args           EFL_UNUSED);

#define TEST_EO_ERROR(fn, _msg)                  \
  ctx.msg = _msg;                                \
  ctx.fnc = fn;                                  \
  ctx.did = EFL_FALSE;                          \
  ctx.expected_level = EINA_LOG_LEVEL_ERR

#define TEST_EO_CRITICAL(fn, _msg)               \
  ctx.msg = _msg;                                \
  ctx.fnc = fn;                                  \
  ctx.did = EFL_FALSE;                          \
  ctx.expected_level = EINA_LOG_LEVEL_CRITICAL

#endif /* _EO_ERROR_MSGS_H */
