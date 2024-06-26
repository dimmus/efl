/* EINA - EFL data type library
 * Copyright (C) 2012 Cedric Bail
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

#include <stdlib.h>

#include "shared_config.h"
#include "eina_lock.h" /* it will include pthread.h with proper flags */
#include "eina_thread.h"
#include "eina_cpu.h"
#include "eina_str.h"

/* undefs EINA_ARG_NONULL() so NULL checks are not compiled out! */
#include "eina_safety_checks.h"

#include "eina_debug_private.h"

#include <pthread.h>
#include <errno.h>
#ifndef _WIN32
#  include <signal.h>
#endif
#include <string.h>

#if defined(EINA_HAVE_PTHREAD_AFFINITY) || defined(EINA_HAVE_PTHREAD_SETNAME)
#  ifndef __linux__
#    include <pthread_np.h>
#    define cpu_set_t cpuset_t
#  endif
#endif

#ifdef __linux__
#  include <sched.h>
#  include <sys/time.h>
#  include <sys/resource.h>
#endif

#include "eina_log.h"

#define RTNICENESS 1
#define NICENESS   5

static inline void *
_eina_thread_join(Eina_Thread t)
{
  void *ret = NULL;
  int   err = pthread_join((pthread_t)t, &ret);

  if (err == 0) return ret;
  return NULL;
}

static inline Efl_Bool
_eina_thread_create(Eina_Thread *t,
                    int          affinity,
                    void *(*func)(void *data),
                    void *data)
{
  int            err;
  pthread_attr_t attr;
  sigset_t       oldset, newset;

  if (pthread_attr_init(&attr) != 0)
  {
    return EFL_FALSE;
  }
  if (affinity >= 0)
  {
#ifdef EINA_HAVE_PTHREAD_AFFINITY
    cpu_set_t cpu;

    CPU_ZERO(&cpu);
    CPU_SET(affinity, &cpu);
    pthread_attr_setaffinity_np(&attr, sizeof(cpu), &cpu);
#endif
  }

   /* setup initial locks */
  sigemptyset(&newset);
  sigaddset(&newset, SIGPIPE);
  sigaddset(&newset, SIGALRM);
  sigaddset(&newset, SIGCHLD);
  sigaddset(&newset, SIGUSR1);
  sigaddset(&newset, SIGUSR2);
  sigaddset(&newset, SIGHUP);
  sigaddset(&newset, SIGQUIT);
  sigaddset(&newset, SIGINT);
  sigaddset(&newset, SIGTERM);
#ifdef SIGPWR
  sigaddset(&newset, SIGPWR);
#endif
  pthread_sigmask(SIG_BLOCK, &newset, &oldset);
  err = pthread_create((pthread_t *)t, &attr, func, data);
  pthread_sigmask(SIG_SETMASK, &oldset, NULL);
  pthread_attr_destroy(&attr);

  if (err == 0) return EFL_TRUE;

  return EFL_FALSE;
}

static inline Efl_Bool
_eina_thread_equal(Eina_Thread t1, Eina_Thread t2)
{
  return pthread_equal((pthread_t)t1, (pthread_t)t2);
}

static inline Eina_Thread
_eina_thread_self(void)
{
  return (Eina_Thread)pthread_self();
}

typedef struct _Eina_Thread_Call Eina_Thread_Call;

struct _Eina_Thread_Call
{
  Eina_Thread_Cb func;
  const void    *data;

  Eina_Thread_Priority prio;
  int                  affinity;
};

static void *
_eina_internal_call(void *context)
{
  Eina_Thread_Call *c = context;
  void             *r;
  pthread_t         self;

   // Default this thread to not cancellable as per Eina documentation
  eina_thread_cancellable_set(EFL_FALSE, NULL);

  EINA_THREAD_CLEANUP_PUSH(free, c);

  self = pthread_self();

  if (c->prio == EINA_THREAD_IDLE)
  {
    struct sched_param params;
    int                min;
#ifdef SCHED_IDLE
    int pol = SCHED_IDLE;
#else
    int pol;
    pthread_getschedparam(self, &pol, &params);
#endif
    min                   = sched_get_priority_min(pol);
    params.sched_priority = min;
    pthread_setschedparam(self, pol, &params);
  }
  else if (c->prio == EINA_THREAD_BACKGROUND)
  {
    struct sched_param params;
    int                min, max;
#ifdef SCHED_BATCH
    int pol = SCHED_BATCH;
#else
    int pol;
    pthread_getschedparam(self, &pol, &params);
#endif
    min                   = sched_get_priority_min(pol);
    max                   = sched_get_priority_max(pol);
    params.sched_priority = (max - min) / 2;
    pthread_setschedparam(self, pol, &params);
  }
// do nothing for normal
//   else if (c->prio == EINA_THREAD_NORMAL)
//     {
//     }
  else if (c->prio == EINA_THREAD_URGENT)
  {
    struct sched_param params;
    int                max, pol;

    pthread_getschedparam(self, &pol, &params);
    max                    = sched_get_priority_max(pol);
    params.sched_priority += 5;
    if (params.sched_priority > max) params.sched_priority = max;
    pthread_setschedparam(self, pol, &params);
  }

  _eina_debug_thread_add(&self);
  EINA_THREAD_CLEANUP_PUSH(_eina_debug_thread_del, &self);
  r = c->func((void *)c->data, eina_thread_self());
  EINA_THREAD_CLEANUP_POP(EFL_TRUE);
  EINA_THREAD_CLEANUP_POP(EFL_TRUE);

  return r;
}

EINA_API Eina_Thread
eina_thread_self(void)
{
  return _eina_thread_self();
}

EINA_API Efl_Bool
eina_thread_equal(Eina_Thread t1, Eina_Thread t2)
{
  return !!_eina_thread_equal(t1, t2);
}

EINA_API Efl_Bool
eina_thread_create(Eina_Thread         *t,
                   Eina_Thread_Priority prio,
                   int                  affinity,
                   Eina_Thread_Cb       func,
                   const void          *data)
{
  Eina_Thread_Call *c;

  EINA_SAFETY_ON_NULL_RETURN_VAL(t, EFL_FALSE);
  EINA_SAFETY_ON_NULL_RETURN_VAL(func, EFL_FALSE);

  c = malloc(sizeof(Eina_Thread_Call));
  if (!c) return EFL_FALSE;

  c->func     = func;
  c->data     = data;
  c->prio     = prio;
  c->affinity = affinity;

   // valgrind complains c is lost - but it's not - it is handed to the
  // child thread to be freed when c->func returns in _eina_internal_call().
  if (_eina_thread_create(t, affinity, _eina_internal_call, c)) return EFL_TRUE;

  free(c);
  return EFL_FALSE;
}

EINA_API void *
eina_thread_join(Eina_Thread t)
{
  return _eina_thread_join(t);
}

EINA_API Efl_Bool
eina_thread_name_set(Eina_Thread t, const char *name)
{
#ifdef EINA_HAVE_PTHREAD_SETNAME
  char buf[16];
  if (name)
  {
    eina_strlcpy(buf, name, 15);
    buf[15] = 0;
  }
  else buf[0] = 0;
#  ifndef __linux__
  pthread_set_name_np((pthread_t)t, buf);
  return EFL_TRUE;
#  else
  if (pthread_setname_np((pthread_t)t, buf) == 0) return EFL_TRUE;
#  endif
#else
  (void)t;
  (void)name;
#endif
  return EFL_FALSE;
}

EINA_API Efl_Bool
eina_thread_cancel(Eina_Thread t)
{
  if (!t) return EFL_FALSE;
  return pthread_cancel((pthread_t)t) == 0;
}

EINA_API Efl_Bool
eina_thread_cancellable_set(Efl_Bool cancellable, Efl_Bool *was_cancellable)
{
  int state = cancellable ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE;
  int old   = 0;
  int r;

  /* enforce deferred in case users changed to asynchronous themselves */
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);

  r = pthread_setcancelstate(state, &old);
  if (was_cancellable && r == 0)
    *was_cancellable = (old == PTHREAD_CANCEL_ENABLE);

  return r == 0;
}

EINA_API void
eina_thread_cancel_checkpoint(void)
{
  pthread_testcancel();
}

EINA_API const void *EINA_THREAD_JOIN_CANCELED = PTHREAD_CANCELED;

EINA_API void
eina_sched_prio_drop(void)
{
  struct sched_param param;
  int                pol, ret;
  pthread_t          pthread_id;

  pthread_id = pthread_self();
  ret        = pthread_getschedparam(pthread_id, &pol, &param);
  if (ret)
  {
    EINA_LOG_ERR("Unable to query sched parameters");
    return;
  }

  if (EINA_UNLIKELY(pol == SCHED_RR || pol == SCHED_FIFO))
  {
    param.sched_priority -= RTNICENESS;

    /* We don't change the policy */
    if (param.sched_priority < 1)
    {
      EINA_LOG_INFO("RT prio < 1, setting to 1 instead");
      param.sched_priority = 1;
    }

    pthread_setschedparam(pthread_id, pol, &param);
  }
#ifdef __linux__
  else
  {
    int prio;
    errno = 0;
    prio  = getpriority(PRIO_PROCESS, 0);
    if (errno == 0)
    {
      prio += NICENESS;
      if (prio > 19)
      {
        EINA_LOG_INFO("Max niceness reached; keeping max (19)");
        prio = 19;
      }

      setpriority(PRIO_PROCESS, 0, prio);
    }
  }
#endif
}

EINA_API Efl_Bool
eina_thread_init(void)
{
  return EFL_TRUE;
}

EINA_API Efl_Bool
eina_thread_shutdown(void)
{
  return EFL_TRUE;
}
