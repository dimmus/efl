#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

#include "Efl_Core.h"
#include "core_private.h"

#define LK(x)  Eina_Lock x
#define LKI(x) eina_lock_new(&(x))
#define LKD(x) eina_lock_free(&(x))
#define LKL(x) eina_lock_take(&(x))
#define LKU(x) eina_lock_release(&(x))

#define SLK(x)  Eina_Spinlock x
#define SLKI(x) eina_spinlock_new(&(x))
#define SLKD(x) eina_spinlock_free(&(x))
#define SLKL(x) eina_spinlock_take(&(x))
#define SLKU(x) eina_spinlock_release(&(x))

#define CD(x)     Eina_Condition x
#define CDI(x, m) eina_condition_new(&(x), &(m))
#define CDD(x)    eina_condition_free(&(x))
#define CDB(x)    eina_condition_broadcast(&(x))
#define CDW(x, t) eina_condition_timedwait(&(x), t)

#define LRWK(x)   Eina_RWLock x
#define LRWKI(x)  eina_rwlock_new(&(x));
#define LRWKD(x)  eina_rwlock_free(&(x));
#define LRWKWL(x) eina_rwlock_take_write(&(x));
#define LRWKRL(x) eina_rwlock_take_read(&(x));
#define LRWKU(x)  eina_rwlock_release(&(x));

#define PH(x)     Eina_Thread x
#define PHE(x, y) eina_thread_equal(x, y)
#define PHS()     eina_thread_self()
#define PHC(x, f, d) \
    eina_thread_create(&(x), EINA_THREAD_BACKGROUND, -1, (void *)f, d)
#define PHC2(x, f, d) \
    eina_thread_create(&(x), EINA_THREAD_URGENT, -1, (void *)f, d)
#define PHJ(x) eina_thread_join(x)

typedef struct _Core_Pthread_Worker Core_Pthread_Worker;
typedef struct _Core_Pthread        Core_Pthread;
typedef struct _Core_Thread_Data    Core_Thread_Data;
typedef struct _Core_Thread_Waiter  Core_Thread_Waiter;

struct _Core_Thread_Waiter
{
  Core_Thread_Cb func_cancel;
  Core_Thread_Cb func_end;
  Efl_Bool       waiting;
};

struct _Core_Thread_Data
{
  void        *data;
  Eina_Free_Cb cb;
};

struct _Core_Pthread_Worker
{
  union {
    struct
    {
      Core_Thread_Cb func_blocking;
    } short_run;

    struct
    {
      Core_Thread_Cb        func_heavy;
      Core_Thread_Notify_Cb func_notify;

      Core_Pthread_Worker *direct_worker;

      int send;
      int received;
    } feedback_run;

    struct
    {
      Core_Thread_Cb        func_main;
      Core_Thread_Notify_Cb func_notify;

      Core_Pipe           *send;
      Core_Pthread_Worker *direct_worker;

      struct
      {
        int send;
        int received;
      } from, to;
    } message_run;
  } u;

  Core_Thread_Waiter *waiter;
  Core_Thread_Cb      func_cancel;
  Core_Thread_Cb      func_end;
  PH(self);
  Eina_Hash *hash;
  CD(cond);
  LK(mutex);

  const void *data;

  int cancel;

  SLK(cancel_mutex);

  Efl_Bool message_run  : 1;
  Efl_Bool feedback_run : 1;
  Efl_Bool kill         : 1;
  Efl_Bool reschedule   : 1;
  Efl_Bool no_queue     : 1;
};

typedef struct _Core_Pthread_Notify Core_Pthread_Notify;

struct _Core_Pthread_Notify
{
  Core_Pthread_Worker *work;
  const void          *user_data;
};

typedef void *(*Core_Thread_Sync_Cb)(void *data, Core_Thread *thread);

typedef struct _Core_Pthread_Message Core_Pthread_Message;

struct _Core_Pthread_Message
{
  union {
    Core_Thread_Cb      async;
    Core_Thread_Sync_Cb sync;
  } u;

  const void *data;

  int code;

  Efl_Bool callback : 1;
  Efl_Bool sync     : 1;
};

static int _core_thread_count_max = 0;

static void _core_thread_handler(void *data);

static int _core_thread_count          = 0;
static int _core_thread_count_no_queue = 0;

static Eina_List *_Core_running_job                  = NULL;
static Eina_List *_Core_pending_job_threads          = NULL;
static Eina_List *_Core_pending_job_threads_feedback = NULL;
static SLK(_Core_pending_job_threads_mutex);
static SLK(_Core_running_job_mutex);

static Eina_Hash *_core_thread_global_hash = NULL;
static LRWK(_core_thread_global_hash_lock);
static LK(_core_thread_global_hash_mutex);
static CD(_core_thread_global_hash_cond);

static Efl_Bool have_main_loop_thread = 0;

static Eina_Trash *_core_thread_worker_trash = NULL;
static int         _core_thread_worker_count = 0;

static void                *_core_thread_worker(void *, Eina_Thread);
static Core_Pthread_Worker *_core_thread_worker_new(void);

static PH(get_main_loop_thread)(void)
{
  static PH(main_loop_thread);
  static pid_t main_loop_pid;
  pid_t        pid = getpid();

  if (pid != main_loop_pid)
  {
    main_loop_pid         = pid;
    main_loop_thread      = PHS();
    have_main_loop_thread = 1;
  }

  return main_loop_thread;
}

static void
_core_thread_worker_free(Core_Pthread_Worker *worker)
{
  SLKD(worker->cancel_mutex);
  CDD(worker->cond);
  LKD(worker->mutex);

  if (_core_thread_worker_count > ((_core_thread_count_max + 1) * 16))
  {
    _core_thread_worker_count--;
    free(worker);
    return;
  }

  eina_trash_push(&_core_thread_worker_trash, worker);
}

static void
_core_thread_data_free(void *data)
{
  Core_Thread_Data *d = data;

  if (d->cb) d->cb(d->data);
  free(d);
}

void
_core_thread_join(void *data)
{
  PH(thread) = (uintptr_t)data;
  DBG("joining thread=%" PRIu64, (uint64_t)thread);
  PHJ(thread);
}

static void
_core_thread_kill(Core_Pthread_Worker *work)
{
  if (work->cancel)
  {
    if (work->func_cancel)
      work->func_cancel((void *)work->data, (Core_Thread *)work);
  }
  else
  {
    if (work->func_end) work->func_end((void *)work->data, (Core_Thread *)work);
  }

  if (work->feedback_run)
  {
    if (work->u.feedback_run.direct_worker)
      _core_thread_worker_free(work->u.feedback_run.direct_worker);
  }
  if (work->hash) eina_hash_free(work->hash);
  _core_thread_worker_free(work);
}

static void
_core_thread_handler(void *data)
{
  Core_Pthread_Worker *work = data;

  if (work->feedback_run)
  {
    if (work->u.feedback_run.send != work->u.feedback_run.received)
    {
      work->kill = EFL_TRUE;
      return;
    }
  }

  _core_thread_kill(work);
}

#if 0
static void
_Core_nothing_handler(void *data EFL_UNUSED, void *buffer EFL_UNUSED, unsigned int nbyte EFL_UNUSED)
{
}

#endif

static void
_Core_notify_handler(void *data)
{
  Core_Pthread_Notify *notify    = data;
  Core_Pthread_Worker *work      = notify->work;
  void                *user_data = (void *)notify->user_data;

  work->u.feedback_run.received++;

  if (work->u.feedback_run.func_notify)
    work->u.feedback_run.func_notify((void *)work->data,
                                     (Core_Thread *)work,
                                     user_data);

   /* Force reading all notify event before killing the thread */
  if (work->kill && work->u.feedback_run.send == work->u.feedback_run.received)
  {
    _core_thread_kill(work);
  }

  free(notify);
}

static void
_Core_message_notify_handler(void *data)
{
  Core_Pthread_Notify  *notify    = data;
  Core_Pthread_Worker  *work      = notify->work;
  Core_Pthread_Message *user_data = (void *)notify->user_data;
  Efl_Bool delete                 = EFL_TRUE;

  work->u.message_run.from.received++;

  if (!user_data->callback)
  {
    if (work->u.message_run.func_notify)
      work->u.message_run.func_notify((void *)work->data,
                                      (Core_Thread *)work,
                                      (void *)user_data->data);
  }
  else
  {
    if (user_data->sync)
    {
      user_data->data =
        user_data->u.sync((void *)user_data->data, (Core_Thread *)work);
      user_data->callback = EFL_FALSE;
      user_data->code     = INT_MAX;
      core_pipe_write(work->u.message_run.send,
                      &user_data,
                      sizeof(Core_Pthread_Message *));

      delete = EFL_FALSE;
    }
    else
    {
      user_data->u.async((void *)user_data->data, (Core_Thread *)work);
    }
  }

  if (delete)
  {
    free(user_data);
  }

   /* Force reading all notify event before killing the thread */
  if (work->kill &&
      work->u.message_run.from.send == work->u.message_run.from.received)
  {
    _core_thread_kill(work);
  }
  free(notify);
}

static void
_Core_short_job_cleanup(void *data)
{
  Core_Pthread_Worker *work = data;

  DBG("cleanup work=%p, thread=%" PRIu64, work, (uint64_t)work->self);

  SLKL(_Core_running_job_mutex);
  _Core_running_job = eina_list_remove(_Core_running_job, work);
  SLKU(_Core_running_job_mutex);

  if (work->reschedule)
  {
    work->reschedule = EFL_FALSE;

    SLKL(_Core_pending_job_threads_mutex);
    _Core_pending_job_threads =
      eina_list_append(_Core_pending_job_threads, work);
    SLKU(_Core_pending_job_threads_mutex);
  }
  else
  {
    core_main_loop_thread_safe_call_async(_core_thread_handler, work);
  }
}

static void
_Core_short_job(PH(thread))
{
  Core_Pthread_Worker *work;
  int                  cancel;

  SLKL(_Core_pending_job_threads_mutex);

  if (!_Core_pending_job_threads)
  {
    SLKU(_Core_pending_job_threads_mutex);
    return;
  }

  work = eina_list_data_get(_Core_pending_job_threads);
  _Core_pending_job_threads =
    eina_list_remove_list(_Core_pending_job_threads, _Core_pending_job_threads);
  SLKU(_Core_pending_job_threads_mutex);

  SLKL(_Core_running_job_mutex);
  _Core_running_job = eina_list_append(_Core_running_job, work);
  SLKU(_Core_running_job_mutex);

  SLKL(work->cancel_mutex);
  cancel = work->cancel;
  SLKU(work->cancel_mutex);
  work->self = thread;

  EINA_THREAD_CLEANUP_PUSH(_Core_short_job_cleanup, work);
  if (!cancel)
    work->u.short_run.func_blocking((void *)work->data, (Core_Thread *)work);
  eina_thread_cancellable_set(EFL_FALSE, NULL);
  EINA_THREAD_CLEANUP_POP(EFL_TRUE);
}

static void
_Core_feedback_job_cleanup(void *data)
{
  Core_Pthread_Worker *work = data;

  DBG("cleanup work=%p, thread=%" PRIu64, work, (uint64_t)work->self);

  SLKL(_Core_running_job_mutex);
  _Core_running_job = eina_list_remove(_Core_running_job, work);
  SLKU(_Core_running_job_mutex);

  if (work->reschedule)
  {
    work->reschedule = EFL_FALSE;

    SLKL(_Core_pending_job_threads_mutex);
    _Core_pending_job_threads_feedback =
      eina_list_append(_Core_pending_job_threads_feedback, work);
    SLKU(_Core_pending_job_threads_mutex);
  }
  else
  {
    core_main_loop_thread_safe_call_async(_core_thread_handler, work);
  }
}

static void
_Core_feedback_job(PH(thread))
{
  Core_Pthread_Worker *work;
  int                  cancel;

  SLKL(_Core_pending_job_threads_mutex);

  if (!_Core_pending_job_threads_feedback)
  {
    SLKU(_Core_pending_job_threads_mutex);
    return;
  }

  work = eina_list_data_get(_Core_pending_job_threads_feedback);
  _Core_pending_job_threads_feedback =
    eina_list_remove_list(_Core_pending_job_threads_feedback,
                          _Core_pending_job_threads_feedback);
  SLKU(_Core_pending_job_threads_mutex);
  SLKL(_Core_running_job_mutex);
  _Core_running_job = eina_list_append(_Core_running_job, work);
  SLKU(_Core_running_job_mutex);

  SLKL(work->cancel_mutex);
  cancel = work->cancel;
  SLKU(work->cancel_mutex);
  work->self = thread;

  EINA_THREAD_CLEANUP_PUSH(_Core_feedback_job_cleanup, work);
  if (!cancel)
    work->u.feedback_run.func_heavy((void *)work->data, (Core_Thread *)work);
  eina_thread_cancellable_set(EFL_FALSE, NULL);
  EINA_THREAD_CLEANUP_POP(EFL_TRUE);
}

static void
_Core_direct_worker_cleanup(void *data)
{
  Core_Pthread_Worker *work = data;

  DBG("cleanup work=%p, thread=%" PRIu64 " (should join)",
      work,
      (uint64_t)work->self);

  SLKL(_Core_pending_job_threads_mutex);
  _core_thread_count_no_queue--;
  core_main_loop_thread_safe_call_async(_core_thread_handler, work);

  core_main_loop_thread_safe_call_async((Core_Cb)_core_thread_join,
                                        (void *)(intptr_t)PHS());
  SLKU(_Core_pending_job_threads_mutex);
}

static void *
_Core_direct_worker(void *data, Eina_Thread t EFL_UNUSED)
{
  Core_Pthread_Worker *work = data;
  eina_thread_cancellable_set(EFL_FALSE, NULL);
  eina_thread_name_set(eina_thread_self(), "Ethread-feedback");
  work->self = PHS();

  EINA_THREAD_CLEANUP_PUSH(_Core_direct_worker_cleanup, work);
  if (work->message_run)
    work->u.message_run.func_main((void *)work->data, (Core_Thread *)work);
  else work->u.feedback_run.func_heavy((void *)work->data, (Core_Thread *)work);
  eina_thread_cancellable_set(EFL_FALSE, NULL);
  EINA_THREAD_CLEANUP_POP(EFL_TRUE);

  return NULL;
}

static void
_core_thread_worker_cleanup(void *data EFL_UNUSED)
{
  DBG("cleanup thread=%" PRIuPTR " (should join)", PHS());
  SLKL(_Core_pending_job_threads_mutex);
  _core_thread_count--;
  core_main_loop_thread_safe_call_async((Core_Cb)_core_thread_join,
                                        (void *)(intptr_t)PHS());
  SLKU(_Core_pending_job_threads_mutex);
}

static void *
_core_thread_worker(void *data EFL_UNUSED, Eina_Thread t EFL_UNUSED)
{
  eina_thread_cancellable_set(EFL_FALSE, NULL);
  EINA_THREAD_CLEANUP_PUSH(_core_thread_worker_cleanup, NULL);
restart:

   /* these 2 are cancellation points as user cb may enable */
  _Core_short_job(PHS());
  _Core_feedback_job(PHS());

   /* from here on, cancellations are guaranteed to be disabled */

  /* FIXME: Check if there is feedback running task todo, and switch to feedback run handler. */
  eina_thread_name_set(eina_thread_self(), "Ethread-worker");

  SLKL(_Core_pending_job_threads_mutex);
  if (_Core_pending_job_threads || _Core_pending_job_threads_feedback)
  {
    SLKU(_Core_pending_job_threads_mutex);
    goto restart;
  }
  SLKU(_Core_pending_job_threads_mutex);

  /* Sleep a little to prevent premature death */
#ifdef _WIN32
  Sleep(1); /* around 50ms */
#else
  usleep(50);
#endif

  SLKL(_Core_pending_job_threads_mutex);
  if (_Core_pending_job_threads || _Core_pending_job_threads_feedback)
  {
    SLKU(_Core_pending_job_threads_mutex);
    goto restart;
  }
  SLKU(_Core_pending_job_threads_mutex);

  EINA_THREAD_CLEANUP_POP(EFL_TRUE);

  return NULL;
}

static Core_Pthread_Worker *
_core_thread_worker_new(void)
{
  Core_Pthread_Worker *result;

  result = eina_trash_pop(&_core_thread_worker_trash);

  if (!result)
  {
    result = calloc(1, sizeof(Core_Pthread_Worker));
    _core_thread_worker_count++;
  }
  else
  {
    memset(result, 0, sizeof(Core_Pthread_Worker));
  }

  SLKI(result->cancel_mutex);
  LKI(result->mutex);
  CDI(result->cond, result->mutex);

  return result;
}

void
_core_thread_init(void)
{
  _core_thread_count_max = eina_cpu_count() * 4;
  if (_core_thread_count_max <= 0) _core_thread_count_max = 1;

  SLKI(_Core_pending_job_threads_mutex);
  LRWKI(_core_thread_global_hash_lock);
  LKI(_core_thread_global_hash_mutex);
  SLKI(_Core_running_job_mutex);
  CDI(_core_thread_global_hash_cond, _core_thread_global_hash_mutex);
}

void
_core_thread_shutdown(void)
{
  /* FIXME: If function are still running in the background, should we kill them ? */
  Core_Pthread_Worker *work;
  Eina_List           *l;
  Efl_Bool             test;
  int                  iteration = 0;

  SLKL(_Core_pending_job_threads_mutex);

  EINA_LIST_FREE(_Core_pending_job_threads, work)
  {
    if (work->func_cancel)
      work->func_cancel((void *)work->data, (Core_Thread *)work);
    free(work);
  }

  EINA_LIST_FREE(_Core_pending_job_threads_feedback, work)
  {
    if (work->func_cancel)
      work->func_cancel((void *)work->data, (Core_Thread *)work);
    free(work);
  }

  SLKU(_Core_pending_job_threads_mutex);
  SLKL(_Core_running_job_mutex);

  EINA_LIST_FOREACH(_Core_running_job, l, work)
    core_thread_cancel((Core_Thread *)work);

  SLKU(_Core_running_job_mutex);

  do
  {
    SLKL(_Core_pending_job_threads_mutex);
    if (_core_thread_count + _core_thread_count_no_queue > 0)
    {
      test = EFL_TRUE;
    }
    else
    {
      test = EFL_FALSE;
    }
    SLKU(_Core_pending_job_threads_mutex);
    iteration++;
    if (test)
    {
      _core_main_call_flush();
      usleep(1000);
    }
  }
  while (test == EFL_TRUE && iteration < 50);

  if (iteration == 20 && _core_thread_count > 0)
  {
    ERR("%i of the child thread are still running after 1s. This can lead "
        "to a segv. Sorry.",
        _core_thread_count);
  }

  if (_core_thread_global_hash) eina_hash_free(_core_thread_global_hash);
  have_main_loop_thread = 0;

  while ((work = eina_trash_pop(&_core_thread_worker_trash)))
  {
    free(work);
  }

  SLKD(_Core_pending_job_threads_mutex);
  LRWKD(_core_thread_global_hash_lock);
  LKD(_core_thread_global_hash_mutex);
  SLKD(_Core_running_job_mutex);
  CDD(_core_thread_global_hash_cond);
}

EAPI Core_Thread *
core_thread_run(Core_Thread_Cb func_blocking,
                Core_Thread_Cb func_end,
                Core_Thread_Cb func_cancel,
                const void    *data)
{
  Core_Pthread_Worker *work;
  Efl_Bool             tried = EFL_FALSE;
  PH(thread);

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

  if (!func_blocking) return NULL;

  work = _core_thread_worker_new();
  if (!work)
  {
    if (func_cancel) func_cancel((void *)data, NULL);
    return NULL;
  }

  work->u.short_run.func_blocking = func_blocking;
  work->func_end                  = func_end;
  work->func_cancel               = func_cancel;
  work->cancel                    = EFL_FALSE;
  work->feedback_run              = EFL_FALSE;
  work->message_run               = EFL_FALSE;
  work->kill                      = EFL_FALSE;
  work->reschedule                = EFL_FALSE;
  work->no_queue                  = EFL_FALSE;
  work->data                      = data;

  work->self = 0;
  work->hash = NULL;

  SLKL(_Core_pending_job_threads_mutex);
  _Core_pending_job_threads = eina_list_append(_Core_pending_job_threads, work);

  if (_core_thread_count == _core_thread_count_max)
  {
    SLKU(_Core_pending_job_threads_mutex);
    return (Core_Thread *)work;
  }

  SLKU(_Core_pending_job_threads_mutex);

  /* One more thread could be created. */
  eina_threads_init();

  SLKL(_Core_pending_job_threads_mutex);

retry:
  if (PHC(thread, _core_thread_worker, NULL))
  {
    _core_thread_count++;
    SLKU(_Core_pending_job_threads_mutex);
    return (Core_Thread *)work;
  }
  if (!tried)
  {
    _core_main_call_flush();
    tried = EFL_TRUE;
    goto retry;
  }

  if (_core_thread_count == 0)
  {
    _Core_pending_job_threads =
      eina_list_remove(_Core_pending_job_threads, work);

    if (work->func_cancel)
      work->func_cancel((void *)work->data, (Core_Thread *)work);

    _core_thread_worker_free(work);
    work = NULL;
  }
  SLKU(_Core_pending_job_threads_mutex);

  eina_threads_shutdown();

  return (Core_Thread *)work;
}

EAPI Efl_Bool
core_thread_cancel(Core_Thread *thread)
{
  Core_Pthread_Worker *volatile work = (Core_Pthread_Worker *)thread;
  Eina_List *l;
  int        cancel;

  if (!work) return EFL_TRUE;
  SLKL(work->cancel_mutex);
  cancel = work->cancel;
  SLKU(work->cancel_mutex);
  if (cancel) return EFL_FALSE;

  if (work->feedback_run)
  {
    if (work->kill) return EFL_TRUE;
    if (work->u.feedback_run.send != work->u.feedback_run.received)
      goto on_exit;
  }

  SLKL(_Core_pending_job_threads_mutex);

  if ((have_main_loop_thread) && (PHE(get_main_loop_thread(), PHS())))
  {
    if (!work->feedback_run)
      EINA_LIST_FOREACH(_Core_pending_job_threads, l, work)
      {
        if ((void *)work == (void *)thread)
        {
          _Core_pending_job_threads =
            eina_list_remove_list(_Core_pending_job_threads, l);

          SLKU(_Core_pending_job_threads_mutex);

          if (work->func_cancel)
            work->func_cancel((void *)work->data, (Core_Thread *)work);
          free(work);

          return EFL_TRUE;
        }
      }
    else
      EINA_LIST_FOREACH(_Core_pending_job_threads_feedback, l, work)
      {
        if ((void *)work == (void *)thread)
        {
          _Core_pending_job_threads_feedback =
            eina_list_remove_list(_Core_pending_job_threads_feedback, l);

          SLKU(_Core_pending_job_threads_mutex);

          if (work->func_cancel)
            work->func_cancel((void *)work->data, (Core_Thread *)work);
          free(work);

          return EFL_TRUE;
        }
      }
  }

  SLKU(_Core_pending_job_threads_mutex);

  work = (Core_Pthread_Worker *)thread;

  /* Delay the destruction */
on_exit:
  eina_thread_cancel(
    work
      ->self); /* noop unless eina_thread_cancellable_set() was used by user */
  SLKL(work->cancel_mutex);
  work->cancel = EFL_TRUE;
  SLKU(work->cancel_mutex);

  return EFL_FALSE;
}

static void
_core_thread_wait_reset(Core_Thread_Waiter *waiter, Core_Pthread_Worker *worker)
{
  worker->func_cancel = waiter->func_cancel;
  worker->func_end    = waiter->func_end;
  worker->waiter      = NULL;

  waiter->func_end    = NULL;
  waiter->func_cancel = NULL;
  waiter->waiting     = EFL_FALSE;
}

static void
_core_thread_wait_cancel(void *data EFL_UNUSED, Core_Thread *thread)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;
  Core_Thread_Waiter  *waiter = worker->waiter;

  if (waiter->func_cancel) waiter->func_cancel(data, thread);
  _core_thread_wait_reset(waiter, worker);
}

static void
_core_thread_wait_end(void *data EFL_UNUSED, Core_Thread *thread)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;
  Core_Thread_Waiter  *waiter = worker->waiter;

  if (waiter->func_end) waiter->func_end(data, thread);
  _core_thread_wait_reset(waiter, worker);
}

EAPI Efl_Bool
core_thread_wait(Core_Thread *thread, double wait)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;
  Core_Thread_Waiter   waiter;

  if (!thread) return EFL_TRUE;

  waiter.func_end    = worker->func_end;
  waiter.func_cancel = worker->func_cancel;
  waiter.waiting     = EFL_TRUE;

  // Now trick the thread to call the wrapper function
  worker->waiter      = &waiter;
  worker->func_cancel = _core_thread_wait_cancel;
  worker->func_end    = _core_thread_wait_end;

  while (waiter.waiting == EFL_TRUE)
  {
    double start, end;

    start = core_time_get();
    _core_main_call_flush();
    core_main_loop_thread_safe_call_wait(0.0001);
    end = core_time_get();

    wait -= end - start;

    if (wait <= 0) break;
  }

  if (waiter.waiting == EFL_FALSE)
  {
    return EFL_TRUE;
  }
  else
  {
    _core_thread_wait_reset(&waiter, worker);
    return EFL_FALSE;
  }
}

EAPI Efl_Bool
core_thread_check(Core_Thread *thread)
{
  Core_Pthread_Worker *volatile worker = (Core_Pthread_Worker *)thread;
  int cancel;

  if (!worker) return EFL_TRUE;
  SLKL(worker->cancel_mutex);

  cancel = worker->cancel;
  /* FIXME: there is an insane bug driving me nuts here. I don't know if
      it's a race condition, some cache issue or some alien attack on our software.
      But core_thread_check will only work correctly with a printf, all the volatile,
      lock and even usleep don't help here... */
  /* fprintf(stderr, "wc: %i\n", cancel); */
  SLKU(worker->cancel_mutex);
  return cancel;
}

EAPI Core_Thread *
core_thread_feedback_run(Core_Thread_Cb        func_heavy,
                         Core_Thread_Notify_Cb func_notify,
                         Core_Thread_Cb        func_end,
                         Core_Thread_Cb        func_cancel,
                         const void           *data,
                         Efl_Bool              try_no_queue)
{
  Core_Pthread_Worker *worker;
  Efl_Bool             tried = EFL_FALSE;
  PH(thread);

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

  if (!func_heavy) return NULL;

  worker = _core_thread_worker_new();
  if (!worker) goto on_error;

  worker->u.feedback_run.func_heavy  = func_heavy;
  worker->u.feedback_run.func_notify = func_notify;
  worker->hash                       = NULL;
  worker->func_cancel                = func_cancel;
  worker->func_end                   = func_end;
  worker->data                       = data;
  worker->cancel                     = EFL_FALSE;
  worker->message_run                = EFL_FALSE;
  worker->feedback_run               = EFL_TRUE;
  worker->kill                       = EFL_FALSE;
  worker->reschedule                 = EFL_FALSE;
  worker->self                       = 0;

  worker->u.feedback_run.send     = 0;
  worker->u.feedback_run.received = 0;

  worker->u.feedback_run.direct_worker = NULL;

  if (try_no_queue)
  {
    PH(t);

    worker->u.feedback_run.direct_worker = _core_thread_worker_new();
    worker->no_queue                     = EFL_TRUE;

    eina_threads_init();

retry_direct:
    if (PHC2(t, _Core_direct_worker, worker))
    {
      SLKL(_Core_pending_job_threads_mutex);
      _core_thread_count_no_queue++;
      SLKU(_Core_pending_job_threads_mutex);
      return (Core_Thread *)worker;
    }
    if (!tried)
    {
      _core_main_call_flush();
      tried = EFL_TRUE;
      goto retry_direct;
    }

    if (worker->u.feedback_run.direct_worker)
    {
      _core_thread_worker_free(worker->u.feedback_run.direct_worker);
      worker->u.feedback_run.direct_worker = NULL;
    }

    eina_threads_shutdown();
  }

  worker->no_queue = EFL_FALSE;

  SLKL(_Core_pending_job_threads_mutex);
  _Core_pending_job_threads_feedback =
    eina_list_append(_Core_pending_job_threads_feedback, worker);

  if (_core_thread_count == _core_thread_count_max)
  {
    SLKU(_Core_pending_job_threads_mutex);
    return (Core_Thread *)worker;
  }

  SLKU(_Core_pending_job_threads_mutex);

  /* One more thread could be created. */
  eina_threads_init();

  SLKL(_Core_pending_job_threads_mutex);
retry:
  if (PHC(thread, _core_thread_worker, NULL))
  {
    _core_thread_count++;
    SLKU(_Core_pending_job_threads_mutex);
    return (Core_Thread *)worker;
  }
  if (!tried)
  {
    _core_main_call_flush();
    tried = EFL_TRUE;
    goto retry;
  }
  SLKU(_Core_pending_job_threads_mutex);

  eina_threads_shutdown();

on_error:
  SLKL(_Core_pending_job_threads_mutex);
  if (_core_thread_count == 0)
  {
    _Core_pending_job_threads_feedback =
      eina_list_remove(_Core_pending_job_threads_feedback, worker);

    if (func_cancel) func_cancel((void *)data, NULL);

    if (worker)
    {
      CDD(worker->cond);
      LKD(worker->mutex);
      free(worker);
      worker = NULL;
    }
  }
  SLKU(_Core_pending_job_threads_mutex);

  return (Core_Thread *)worker;
}

EAPI Efl_Bool
core_thread_feedback(Core_Thread *thread, const void *data)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;

  if (!worker) return EFL_FALSE;

  if (!PHE(worker->self, PHS())) return EFL_FALSE;

  if (worker->feedback_run)
  {
    Core_Pthread_Notify *notify;

    notify = malloc(sizeof(Core_Pthread_Notify));
    if (!notify) return EFL_FALSE;

    notify->user_data = data;
    notify->work      = worker;
    worker->u.feedback_run.send++;

    core_main_loop_thread_safe_call_async(_Core_notify_handler, notify);
  }
  else if (worker->message_run)
  {
    Core_Pthread_Message *msg;
    Core_Pthread_Notify  *notify;

    msg = malloc(sizeof(Core_Pthread_Message));
    if (!msg) return EFL_FALSE;
    msg->data     = data;
    msg->callback = EFL_FALSE;
    msg->sync     = EFL_FALSE;

    notify = malloc(sizeof(Core_Pthread_Notify));
    if (!notify)
    {
      free(msg);
      return EFL_FALSE;
    }
    notify->work      = worker;
    notify->user_data = msg;

    worker->u.message_run.from.send++;
    core_main_loop_thread_safe_call_async(_Core_message_notify_handler, notify);
  }
  else return EFL_FALSE;

  return EFL_TRUE;
}

#if 0
EAPI Core_Thread *
core_thread_message_run(Core_Thread_Cb func_main,
                         Core_Thread_Notify_Cb func_notify,
                         Core_Thread_Cb func_end,
                         Core_Thread_Cb func_cancel,
                         const void *data)
{
   Core_Pthread_Worker *worker;
   PH(t);

   if (!func_main) return NULL;

   worker = _core_thread_worker_new();
   if (!worker) return NULL;

   worker->u.message_run.func_main = func_main;
   worker->u.message_run.func_notify = func_notify;
   worker->u.message_run.direct_worker = _core_thread_worker_new();
   worker->u.message_run.send = core_pipe_add(_Core_nothing_handler, worker);
   worker->u.message_run.from.send = 0;
   worker->u.message_run.from.received = 0;
   worker->u.message_run.to.send = 0;
   worker->u.message_run.to.received = 0;

   core_pipe_freeze(worker->u.message_run.send);

   worker->func_cancel = func_cancel;
   worker->func_end = func_end;
   worker->hash = NULL;
   worker->data = data;

   worker->cancel = EFL_FALSE;
   worker->message_run = EFL_TRUE;
   worker->feedback_run = EFL_FALSE;
   worker->kill = EFL_FALSE;
   worker->reschedule = EFL_FALSE;
   worker->no_queue = EFL_FALSE;
   worker->self = 0;

   eina_threads_init();

   if (PHC(t, _Core_direct_worker, worker))
     return (Core_Thread *)worker;

   eina_threads_shutdown();

   if (worker->u.message_run.direct_worker) _core_thread_worker_free(worker->u.message_run.direct_worker);
   if (worker->u.message_run.send) core_pipe_del(worker->u.message_run.send);

   CDD(worker->cond);
   LKD(worker->mutex);

   func_cancel((void *)data, NULL);

   return NULL;
}

#endif

EAPI Efl_Bool
core_thread_reschedule(Core_Thread *thread)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;

  if (!worker) return EFL_FALSE;

  if (!PHE(worker->self, PHS())) return EFL_FALSE;

  worker->reschedule = EFL_TRUE;
  return EFL_TRUE;
}

EAPI int
core_thread_active_get(void)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  return _core_thread_count;
}

EAPI int
core_thread_pending_get(void)
{
  int ret;

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  SLKL(_Core_pending_job_threads_mutex);
  ret = eina_list_count(_Core_pending_job_threads);
  SLKU(_Core_pending_job_threads_mutex);
  return ret;
}

EAPI int
core_thread_pending_feedback_get(void)
{
  int ret;

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  SLKL(_Core_pending_job_threads_mutex);
  ret = eina_list_count(_Core_pending_job_threads_feedback);
  SLKU(_Core_pending_job_threads_mutex);
  return ret;
}

EAPI int
core_thread_pending_total_get(void)
{
  int ret;

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  SLKL(_Core_pending_job_threads_mutex);
  ret = eina_list_count(_Core_pending_job_threads) +
        eina_list_count(_Core_pending_job_threads_feedback);
  SLKU(_Core_pending_job_threads_mutex);
  return ret;
}

EAPI int
core_thread_max_get(void)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  return _core_thread_count_max;
}

EAPI void
core_thread_max_set(int num)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  if (num < 1) return;
  /* avoid doing something hilarious by blocking dumb users */
  if (num > (32 * eina_cpu_count())) num = 32 * eina_cpu_count();

  _core_thread_count_max = num;
}

EAPI void
core_thread_max_reset(void)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  _core_thread_count_max = eina_cpu_count() * 4;
}

EAPI int
core_thread_available_get(void)
{
  int ret;

  SLKL(_Core_pending_job_threads_mutex);
  ret = _core_thread_count_max - _core_thread_count;
  SLKU(_Core_pending_job_threads_mutex);
  return ret;
}

EAPI Efl_Bool
core_thread_name_set(Core_Thread *thread, const char *name)
{
  Core_Pthread_Worker *work = (Core_Pthread_Worker *)thread;

  if ((!work) || (!work->self) || (!name)) return EFL_FALSE;

  if (eina_thread_self() != work->self) return EFL_FALSE;

  return eina_thread_name_set(work->self, name);
}

EAPI Efl_Bool
core_thread_local_data_add(Core_Thread *thread,
                           const char  *key,
                           void        *value,
                           Eina_Free_Cb cb,
                           Efl_Bool     direct)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;
  Core_Thread_Data    *d;
  Efl_Bool             ret;

  if ((!thread) || (!key) || (!value)) return EFL_FALSE;

  LKL(worker->mutex);
  if (!worker->hash)
    worker->hash = eina_hash_string_small_new(_core_thread_data_free);
  LKU(worker->mutex);

  if (!worker->hash) return EFL_FALSE;

  if (!(d = malloc(sizeof(Core_Thread_Data)))) return EFL_FALSE;

  d->data = value;
  d->cb   = cb;

  LKL(worker->mutex);
  if (direct) ret = eina_hash_direct_add(worker->hash, key, d);
  else ret = eina_hash_add(worker->hash, key, d);
  LKU(worker->mutex);
  CDB(worker->cond);
  return ret;
}

EAPI void *
core_thread_local_data_set(Core_Thread *thread,
                           const char  *key,
                           void        *value,
                           Eina_Free_Cb cb)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;
  Core_Thread_Data    *d, *r;
  void                *ret;

  if ((!thread) || (!key) || (!value)) return NULL;

  LKL(worker->mutex);
  if (!worker->hash)
    worker->hash = eina_hash_string_small_new(_core_thread_data_free);
  LKU(worker->mutex);

  if (!worker->hash) return NULL;

  if (!(d = malloc(sizeof(Core_Thread_Data)))) return NULL;

  d->data = value;
  d->cb   = cb;

  LKL(worker->mutex);
  r = eina_hash_set(worker->hash, key, d);
  LKU(worker->mutex);
  CDB(worker->cond);

  if (r)
  {
    ret = r->data;
    free(r);
    return ret;
  }
  return NULL;
}

EAPI void *
core_thread_local_data_find(Core_Thread *thread, const char *key)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;
  Core_Thread_Data    *d;

  if ((!thread) || (!key)) return NULL;

  if (!worker->hash) return NULL;

  LKL(worker->mutex);
  d = eina_hash_find(worker->hash, key);
  LKU(worker->mutex);
  if (d) return d->data;
  return NULL;
}

EAPI Efl_Bool
core_thread_local_data_del(Core_Thread *thread, const char *key)
{
  Core_Pthread_Worker *worker = (Core_Pthread_Worker *)thread;
  Efl_Bool             r;

  if ((!thread) || (!key)) return EFL_FALSE;

  if (!worker->hash) return EFL_FALSE;

  LKL(worker->mutex);
  r = eina_hash_del_by_key(worker->hash, key);
  LKU(worker->mutex);
  return r;
}

EAPI Efl_Bool
core_thread_global_data_add(const char  *key,
                            void        *value,
                            Eina_Free_Cb cb,
                            Efl_Bool     direct)
{
  Core_Thread_Data *d;
  Efl_Bool          ret;

  if ((!key) || (!value)) return EFL_FALSE;

  LRWKWL(_core_thread_global_hash_lock);
  if (!_core_thread_global_hash)
    _core_thread_global_hash =
      eina_hash_string_small_new(_core_thread_data_free);
  LRWKU(_core_thread_global_hash_lock);

  if (!(d = malloc(sizeof(Core_Thread_Data)))) return EFL_FALSE;

  d->data = value;
  d->cb   = cb;

  if (!_core_thread_global_hash)
  {
    free(d);
    return EFL_FALSE;
  }

  LRWKWL(_core_thread_global_hash_lock);
  if (direct) ret = eina_hash_direct_add(_core_thread_global_hash, key, d);
  else ret = eina_hash_add(_core_thread_global_hash, key, d);
  LRWKU(_core_thread_global_hash_lock);
  CDB(_core_thread_global_hash_cond);
  return ret;
}

EAPI void *
core_thread_global_data_set(const char *key, void *value, Eina_Free_Cb cb)
{
  Core_Thread_Data *d, *r;
  void             *ret;

  if ((!key) || (!value)) return NULL;

  LRWKWL(_core_thread_global_hash_lock);
  if (!_core_thread_global_hash)
    _core_thread_global_hash =
      eina_hash_string_small_new(_core_thread_data_free);
  LRWKU(_core_thread_global_hash_lock);

  if (!_core_thread_global_hash) return NULL;

  if (!(d = malloc(sizeof(Core_Thread_Data)))) return NULL;

  d->data = value;
  d->cb   = cb;

  LRWKWL(_core_thread_global_hash_lock);
  r = eina_hash_set(_core_thread_global_hash, key, d);
  LRWKU(_core_thread_global_hash_lock);
  CDB(_core_thread_global_hash_cond);

  if (r)
  {
    ret = r->data;
    free(r);
    return ret;
  }
  return NULL;
}

EAPI void *
core_thread_global_data_find(const char *key)
{
  Core_Thread_Data *ret;

  if (!key) return NULL;

  if (!_core_thread_global_hash) return NULL;

  LRWKRL(_core_thread_global_hash_lock);
  ret = eina_hash_find(_core_thread_global_hash, key);
  LRWKU(_core_thread_global_hash_lock);
  if (ret) return ret->data;
  return NULL;
}

EAPI Efl_Bool
core_thread_global_data_del(const char *key)
{
  Efl_Bool ret;

  if (!key) return EFL_FALSE;

  if (!_core_thread_global_hash) return EFL_FALSE;

  LRWKWL(_core_thread_global_hash_lock);
  ret = eina_hash_del_by_key(_core_thread_global_hash, key);
  LRWKU(_core_thread_global_hash_lock);
  return ret;
}

EAPI void *
core_thread_global_data_wait(const char *key, double seconds)
{
  double            tm  = 0;
  Core_Thread_Data *ret = NULL;

  if (!key) return NULL;

  if (seconds > 0) tm = core_time_get() + seconds;

  while (1)
  {
    LRWKRL(_core_thread_global_hash_lock);
    if (_core_thread_global_hash)
      ret = eina_hash_find(_core_thread_global_hash, key);
    LRWKU(_core_thread_global_hash_lock);
    if ((ret) || (!EINA_DBL_EQ(seconds, 0.0)) ||
        ((seconds > 0) && (tm <= core_time_get())))
      break;
    LKL(_core_thread_global_hash_mutex);
    CDW(_core_thread_global_hash_cond, tm - core_time_get());
    LKU(_core_thread_global_hash_mutex);
  }
  if (ret) return ret->data;
  return NULL;
}
