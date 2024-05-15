#ifndef _EFL_DBUS_MODEL_PRIVATE_H
#define _EFL_DBUS_MODEL_PRIVATE_H

#include "efl_dbus_private.h"

#include <Efl_Core.h>
#include <Efl_Shared.h>
#include <Efl_Eo.h>
#include <Efl_Eo_Interfaces.h>

#include "core_internal.h"

typedef struct _Efl_Dbus_Children_Slice_Promise Efl_Dbus_Children_Slice_Promise;
typedef struct _Efl_Dbus_Model_Data Efl_Dbus_Model_Data;

struct _Efl_Dbus_Children_Slice_Promise
{
   Eina_Promise *p;

   unsigned int start;
   unsigned int count;
};

struct _Efl_Dbus_Model_Data
{
   Efl_Dbus_Connection *connection;
   Efl_Dbus_Connection_Type type;

   Eina_Stringshare *address;
   Eina_Stringshare *unique_name;

   Efl_Bool private : 1;

   Efl_Bool is_listed : 1;
};

#define UNIQUE_NAME_PROPERTY "unique_name"

/* logging support */
extern int efl_dbus_model_log_dom;

#define EFL_DBUS_MODEL_ON_ERROR_EXIT_PROMISE_SET(exp, promise, err, v)    \
  do                                                                    \
    {                                                                   \
      if (EFL_UNLIKELY(!(exp)))                                        \
        {                                                               \
            efl_promise_failed_set(promise, err);                       \
            return v;                                                   \
        }                                                               \
    }                                                                   \
  while(0)

#ifdef CRI
# undef CRI
#endif
#define CRI(...) EINA_LOG_DOM_CRIT(efl_dbus_model_log_dom, __VA_ARGS__)

#ifdef ERR
# undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(efl_dbus_model_log_dom, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...) EINA_LOG_DOM_WARN(efl_dbus_model_log_dom, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...) EINA_LOG_DOM_INFO(efl_dbus_model_log_dom, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(efl_dbus_model_log_dom, __VA_ARGS__)

#endif
