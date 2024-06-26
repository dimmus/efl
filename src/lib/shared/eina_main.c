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
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <time.h>

#ifdef EFL_HAVE_THREADS
#  ifdef HAVE_SYS_TYPES_H
#    include <sys/types.h>
#  endif
#  include <unistd.h>
#endif

#ifndef _WIN32
#  ifdef HAVE_SYS_TYPES_H
#    include <sys/types.h>
#  endif
#  include <sys/stat.h>
#  include <fcntl.h>
#endif

#ifdef HAVE_MCHECK_H
#  ifdef HAVE_MTRACE
#    define MT 1
#  endif
#endif

#ifdef MT
#  include <mcheck.h>
#endif

#include "eina_lock.h"
#include "shared_config.h"
#include "eina_private.h"
#include "eina_types.h"
#include "eina_main.h"
#include "eina_error.h"
#include "eina_log.h"
#include "eina_hash.h"
#include "eina_binshare.h"
#include "eina_stringshare.h"
#include "eina_ustringshare.h"
#include "eina_list.h"
#include "eina_matrixsparse.h"
#include "eina_array.h"
#include "eina_benchmark.h"
#include "eina_magic.h"
#include "eina_rectangle.h"
#include "eina_safety_checks.h"
#include "eina_inlist.h"
#include "eina_inarray.h"
#include "eina_value.h"
#include "eina_evlog.h"
#include "eina_freeq.h"
#include "eina_slstr.h"
#include "eina_vpath.h"

/*============================================================================*
*                                  Local                                     *
*============================================================================*/

/**
 * @cond LOCAL
 */

static Eina_Version _version = { EFL_VERSION_MAJOR,
                                 EFL_VERSION_MINOR,
                                 EFL_VERSION_MICRO,
                                 0 };

static int _eina_main_count = 0;
#ifdef EFL_HAVE_THREADS
static int _eina_main_thread_count = 0;
#endif
static int _eina_log_dom = -1;

#ifdef ERR
#  undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_eina_log_dom, __VA_ARGS__)

#ifdef DBG
#  undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_eina_log_dom, __VA_ARGS__)

EINA_API Efl_Bool     _eina_threads_activated    = EFL_FALSE;
EINA_API Eina_Error   EINA_ERROR_NOT_MAIN_LOOP   = 0;
EINA_API Eina_Error   EINA_ERROR_NOT_IMPLEMENTED = 0;
EINA_API unsigned int eina_seed                  = 0;

#ifdef EFL_HAVE_THREADS
EINA_API pthread_t _eina_main_loop;
#endif

#ifdef MT
static int _mt_enabled = 0;
#endif

#ifdef EFL_HAVE_THREADS
EINA_API int             _eina_threads_debug = 0;
EINA_API pthread_mutex_t _eina_tracking_lock;
EINA_API Eina_Inlist    *_eina_tracking = NULL;
extern Eina_Lock         _sysmon_lock;
#endif

#ifdef _WIN32
BCRYPT_ALG_HANDLE _eina_bcrypt_provider;
#endif

/* place module init/shutdown functions here to avoid other modules
 * calling them by mistake.
 */
#define S(x)                               \
    extern Efl_Bool eina_##x##_init(void); \
    extern Efl_Bool eina_##x##_shutdown(void)
S(debug);
S(evlog);
S(log);
S(error);
S(safety_checks);
S(magic_string);
S(iterator);
S(accessor);
S(inarray);
S(array);
S(module);
S(mempool);
S(list);
S(binshare);
S(stringshare);
S(ustringshare);
S(matrixsparse);
S(convert);
S(benchmark);
S(rectangle);
S(strbuf);
S(ustrbuf);
S(quadtree);
S(simple_xml);
S(prefix);
S(value);
S(tmpstr);
S(thread);
S(cow);
S(cpu);
S(thread_queue);
S(rbtree);
S(file);
S(safepointer);
S(slstr);
S(promise);
S(vpath);
S(abstract_content);
#undef S

struct eina_desc_setup
{
  const char *name;
  Efl_Bool (*init)(void);
  Efl_Bool (*shutdown)(void);
};

static const struct eina_desc_setup _eina_desc_setup[] = {
#define S(x)                                     \
    {                                            \
        #x, eina_##x##_init, eina_##x##_shutdown \
    }
   /* log is a special case as it needs printf */
  S(module),          S(mempool),      S(list),
  S(stringshare),     S(vpath),        S(debug),
  S(evlog),           S(error),        S(safety_checks),
  S(magic_string),    S(iterator),     S(accessor),
  S(inarray),         S(array),        S(binshare),
  S(ustringshare),    S(matrixsparse), S(convert),
  S(benchmark),       S(rectangle),    S(strbuf),
  S(ustrbuf),         S(quadtree),     S(simple_xml),
  S(prefix),          S(value),        S(tmpstr),
  S(thread),          S(cow),          S(cpu),
  S(thread_queue),    S(rbtree),       S(file),
  S(safepointer),     S(slstr),        S(promise),
  S(abstract_content)
#undef S
};
static const size_t _eina_desc_setup_len =
  sizeof(_eina_desc_setup) / sizeof(_eina_desc_setup[0]);

static void
_eina_shutdown_from_desc(const struct eina_desc_setup *itr)
{
  for (itr--; itr >= _eina_desc_setup; itr--)
  {
    if (!itr->shutdown())
      ERR("Problems shutting down eina module '%s', ignored.", itr->name);
  }

  eina_log_domain_unregister(_eina_log_dom);
  _eina_log_dom = -1;
  eina_log_shutdown();
}

static void
_eina_threads_do_shutdown(void)
{
#ifdef EINA_HAVE_DEBUG_THREADS
  const Eina_Lock *lk;

  pthread_mutex_lock(&_eina_tracking_lock);
  if (_eina_tracking)
  {
    if (((Eina_Lock *)_eina_tracking != (&_sysmon_lock)) ||
        (_eina_tracking->next))
    {
      fprintf(stderr, "*************************\n");
      fprintf(stderr, "* The IMPOSSIBLE HAPPEN *\n");
      fprintf(stderr, "* LOCK STILL TAKEN :    *\n");
      fprintf(stderr, "*************************\n");
      EINA_INLIST_FOREACH(_eina_tracking, lk)
      {
        fprintf(stderr, "=======\n");
        eina_lock_debug(lk);
      }
      fprintf(stderr, "*************************\n");
      abort();
    }
  }
  pthread_mutex_unlock(&_eina_tracking_lock);
#endif

  eina_share_common_threads_shutdown();
  eina_log_threads_shutdown();

  _eina_threads_activated = EFL_FALSE;
}

/**
 * @endcond
 */

/*============================================================================*
*                                 Global                                     *
*============================================================================*/

/*============================================================================*
*                                   API                                      *
*============================================================================*/

/**
 * @var eina_version
 * @brief Eina version (defined at configuration time)
 */
EINA_API Eina_Version *eina_version = &_version;

EINA_API int
eina_init(void)
{
  const struct eina_desc_setup *itr, *itr_end;

  if (EINA_LIKELY(_eina_main_count > 0)) return ++_eina_main_count;

#ifdef _WIN32
  if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&_eina_bcrypt_provider,
                                                  BCRYPT_RNG_ALGORITHM,
                                                  NULL,
                                                  0)))
    return 0;
#else
  int fd = open("/dev/urandom", O_RDONLY);
  if (fd >= 0)
  {
    unsigned int val;

    if (read(fd, &val, sizeof(val)) == sizeof(val)) srand(val);
    else srand(time(NULL));
    close(fd);
  }
  else
#endif
  srand(time(NULL));
  while (eina_seed == 0)
    eina_seed = rand();

#ifdef MT
  if ((getenv("EINA_MTRACE")) && (getenv("MALLOC_TRACE")))
  {
    _mt_enabled = 1;
    mtrace();
  }
#endif

#ifdef EFL_HAVE_THREADS
  _eina_main_loop = pthread_self();
#endif

  eina_freeq_main_set(eina_freeq_new(EINA_FREEQ_DEFAULT));

  if (!eina_log_init())
  {
    fprintf(stderr, "Could not initialize eina logging system.\n");
    return 0;
  }

  _eina_log_dom = eina_log_domain_register("eina", EINA_LOG_COLOR_DEFAULT);
  if (_eina_log_dom < 0)
  {
    EINA_LOG_ERR("Could not register log domain: eina");
    eina_log_shutdown();
    return 0;
  }

#ifdef EINA_HAVE_DEBUG_THREADS
  pthread_mutex_init(&_eina_tracking_lock, NULL);

  if (getenv("EINA_DEBUG_THREADS"))
    _eina_threads_debug = atoi(getenv("EINA_DEBUG_THREADS"));
#endif

  itr     = _eina_desc_setup;
  itr_end = itr + _eina_desc_setup_len;
  for (; itr < itr_end; itr++)
  {
    if (!itr->init())
    {
      ERR("Could not initialize eina module '%s'.", itr->name);
      _eina_shutdown_from_desc(itr);
      return 0;
    }
  }

  eina_cpu_count_internal();

  EINA_ERROR_NOT_MAIN_LOOP =
    eina_error_msg_static_register("Not in main loop.");
  EINA_ERROR_NOT_IMPLEMENTED =
    eina_error_msg_static_register("Functionality not implemented.");

  eina_log_timing(_eina_log_dom, EINA_LOG_STATE_STOP, EINA_LOG_STATE_INIT);

  _eina_main_count = 1;
  eina_evlog("-eina_init", NULL, 0.0, NULL);
  return 1;
}

EINA_API int
eina_shutdown(void)
{
  printf("%d", _eina_main_count);
  if (_eina_main_count <= 0)
  {
    ERR("Init count not greater than 0 in shutdown.");
    return 0;
  }
  _eina_main_count--;
  if (EINA_UNLIKELY(_eina_main_count == 0))
  {
    eina_log_timing(_eina_log_dom,
                    EINA_LOG_STATE_START,
                    EINA_LOG_STATE_SHUTDOWN);

    _eina_shutdown_from_desc(_eina_desc_setup + _eina_desc_setup_len);

    if (_eina_threads_activated && (!_eina_main_thread_count))
      _eina_threads_do_shutdown();
#ifdef EINA_HAVE_DEBUG_THREADS
    pthread_mutex_destroy(&_eina_tracking_lock);
#endif
    eina_freeq_free(eina_freeq_main_get());
#ifdef MT
    if (_mt_enabled)
    {
      muntrace();
      _mt_enabled = 0;
    }
#endif

#ifdef _WIN32
    BCryptCloseAlgorithmProvider(_eina_bcrypt_provider, 0);
#endif
  }

  return _eina_main_count;
}

EINA_API int
eina_threads_init(void)
{
#ifdef EFL_HAVE_THREADS
  int ret;

#  ifdef EINA_HAVE_DEBUG_THREADS
  assert(pthread_equal(_eina_main_loop, pthread_self()));
#  endif

  ++_eina_main_thread_count;
  ret = _eina_main_thread_count;

  if (_eina_main_thread_count > 1) return ret;

  eina_share_common_threads_init();
  eina_log_threads_init();
  _eina_threads_activated = EFL_TRUE;

  return ret;
#else
  return 0;
#endif
}

EINA_API int
eina_threads_shutdown(void)
{
#ifdef EFL_HAVE_THREADS
  int ret;

#  ifdef EINA_HAVE_DEBUG_THREADS
  assert(pthread_equal(_eina_main_loop, pthread_self()));
  assert(_eina_main_thread_count > 0);
#  endif

  ret = --_eina_main_thread_count;
  if (_eina_main_thread_count > 0) return ret;

  if (!_eina_main_count) _eina_threads_do_shutdown();

  return ret;
#else
  return 0;
#endif
}

EINA_API Efl_Bool
eina_main_loop_is(void)
{
#ifdef EFL_HAVE_THREADS
#  ifdef __GNUC__
   /* pthread_self() can't be optimized, it's a single asm "movl" */
  if (__builtin_types_compatible_p(pthread_t, unsigned long int))
    return (pthread_self() == _eina_main_loop);
  else
#  endif
    if (pthread_equal(_eina_main_loop, pthread_self()))
    return EFL_TRUE;
#endif
  return EFL_FALSE;
}

/** The purpose of this API should not be documented, it is used only by the one who know what they are doing. */
EINA_API void
eina_main_loop_define(void)
{
#ifdef EFL_HAVE_THREADS
  _eina_main_loop = pthread_self();
#endif
}

/**
 * @}
 */
