#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#ifdef HAVE_LANGINFO_H
#  include <langinfo.h>
#endif

#ifdef _WIN32
#  include <evil_private.h> /* mmap */
#else
#  include <sys/mman.h>
#endif

#ifdef _WIN32
#  include <evil_private.h> /* evil_init/shutdown */
#endif
#include <Efl_Shared.h>
#include <Efl_Eo_Interfaces.h>

#include "Efl_Core.h"
#include "core_private.h"
// #include "../../static_libs/buildsystem/buildsystem.h" // we have no autotools, so no need anymore

#if defined(HAVE_MALLINFO) || defined(HAVE_MALLOC_INFO) || \
  defined(HAVE_MALLINFO2)
#  include <malloc.h>
#endif

#ifndef O_BINARY
#  define O_BINARY 0
#endif

static Core_Version _version     = { EFL_VERSION_MAJOR,
                                     EFL_VERSION_MINOR,
                                     EFL_VERSION_MICRO,
                                     0 };
EAPI Core_Version  *core_version = &_version;

EAPI double _efl_startup_time = 0;

#if defined(HAVE_MALLINFO) || defined(HAVE_MALLOC_INFO) || \
  defined(HAVE_MALLINFO2)
#  define KEEP_MAX(Global, Local) \
      if (Global < (Local)) Global = Local;

static Efl_Bool _core_memory_statistic(void *data);
#  ifdef HAVE_MALLINFO2
static size_t _core_memory_max_total = 0;
static size_t _core_memory_max_free  = 0;
#  else
static int _core_memory_max_total = 0;
static int _core_memory_max_free  = 0;
#  endif
static pid_t _core_memory_pid = 0;
#  ifdef HAVE_MALLOC_INFO
static FILE *_core_memory_statistic_file = NULL;
#  endif
#endif

static Efl_Bool _no_system_modules = 0xff;

static const char *_core_magic_string_get(Core_Magic m);
static int         _core_init_count           = 0;
static int         _core_init_count_threshold = 0;
int                _core_log_dom              = -1;
int                _core_fps_debug            = 0;

extern void _core_thread_join(void *data);

typedef struct _core_Safe_Call core_Safe_Call;

struct _core_Safe_Call
{
  union {
    Core_Cb      async;
    Core_Data_Cb sync;
  } cb;

  void *data;

  Eina_Lock      m;
  Eina_Condition c;

  Efl_Domain_Data *eo_domain_data;
  int              current_id;

  Efl_Bool sync    : 1;
  Efl_Bool suspend : 1;
};

static void _core_main_loop_thread_safe_call(core_Safe_Call *order);
static void _thread_safe_cleanup(void *data);
static void _thread_callback(void *data, void *buffer, unsigned int nbyte);
static Eina_List *_thread_cb   = NULL;
static Core_Pipe *_thread_call = NULL;
static Eina_Lock  _thread_safety;
static const int  wakeup = 42;

static int            _thread_loop = 0;
static Eina_Lock      _thread_mutex;
static Eina_Condition _thread_cond;
static Eina_Lock      _thread_feedback_mutex;
static Eina_Condition _thread_feedback_cond;

static Eina_Lock _thread_id_lock;
static int       _thread_id        = -1;
static int       _thread_id_max    = 0;
static int       _thread_id_update = 0;

static Core_Power_State  _core_power_state  = CORE_POWER_STATE_MAINS;
static Core_Memory_State _core_memory_state = CORE_MEMORY_STATE_NORMAL;

#ifdef HAVE_SYSTEMD
static void _systemd_watchdog_cb(void *data, const Efl_Event *event);

static Efl_Loop_Timer *_systemd_watchdog = NULL;
#endif

Eina_Lock _core_main_loop_lock;
int       _core_main_lock_count;

/* OpenBSD does not define CODESET
 * FIXME ??
 */

#ifndef CODESET
#  define CODESET "INVALID"
#endif

static Eina_Prefix *_core_pfx   = NULL;
static Eina_Array  *module_list = NULL;

static void
core_system_modules_load(void)
{
  char buf[PATH_MAX] = "";

#ifdef NEED_RUN_IN_TREE
#  if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
  if (getuid() == geteuid())
#  endif
  {
    if (getenv("EFL_RUN_IN_TREE"))
    {
      struct stat st;
      snprintf(buf, sizeof(buf), "%s/src/modules/ecore/system", EFL_BUILD_DIR);
      if (stat(buf, &st) == 0)
      {
        const char *built_modules[] = {
#  ifdef HAVE_SYSTEMD
          "systemd",
#  endif
#  ifdef HAVE_TIZEN_CONFIGURATION_MANAGER
          "tizen",
#  endif
          NULL
        };
        const char **itr;
        for (itr = built_modules; *itr != NULL; itr++)
        {
          eina_module_subsystem_lib_exist(buf,
                                          sizeof(buf),
                                          "core/system",
                                          "system");
          module_list =
            eina_module_list_get(module_list, buf, EFL_FALSE, NULL, NULL);
        }

        if (module_list) eina_module_list_load(module_list);
        return;
      }
    }
  }
#endif

  snprintf(buf, sizeof(buf), "%s/ecore/system", eina_prefix_lib_get(_core_pfx));
  module_list = eina_module_arch_list_get(module_list, buf, EFL_MODULE_ARCH);

  // XXX: MODFIX: do not list ALL modules and load them ALL! this is
  // just polluting memory pages and I/O with modules and code that
  // is then never used. detetc the module we need to use them use
  // that. if anything load each module, have it do a detect and if
  // it fails UNLOAD and try the next one.
  eina_module_list_load(module_list);
}

static void
core_system_modules_unload(void)
{
  if (module_list)
  {
    eina_module_list_free(module_list);
    eina_array_free(module_list);
    module_list = NULL;
  }
}

static void
_efl_first_loop_iterate(void *data, const Efl_Event *event)
{
  double end   = core_time_unix_get();
  char  *first = data;

  switch (*first)
  {
    case 'A':
      abort();
    case 'E':
    case 'D':
      exit(-1);
    case 'T':
      fprintf(stderr,
              "Loop started: '%f' - '%f' = '%f' sec\n",
              end,
              _efl_startup_time,
              end - _efl_startup_time);
      break;
  }
  efl_event_callback_del(event->object,
                         EFL_APP_EVENT_RESUME,
                         _efl_first_loop_iterate,
                         data);
}

EAPI void
core_app_no_system_modules(void)
{
  _no_system_modules = EFL_TRUE;
}

EAPI int
core_init(void)
{
  if (++_core_init_count != 1) return _core_init_count;

  /* make sure libecore is linked to libefl - workaround gcc bug */
  __efl_internal_init();

  setlocale(LC_CTYPE, "");
  /*
      if (strcmp(nl_langinfo(CODESET), "UTF-8"))
      {
        WRN("Not a utf8 locale!");
      }
    */
#ifdef _WIN32
  if (!evil_init()) return --_core_init_count;
  if (!eina_init()) goto shutdown_evil;
#endif
  eina_evlog(">RUN", NULL, 0.0, NULL);
  _core_log_dom = eina_log_domain_register("ecore", CORE_DEFAULT_LOG_COLOR);
  if (_core_log_dom < 0)
  {
    EINA_LOG_ERR("Ecore was unable to create a log domain.");
    goto shutdown_log_dom;
  }
  _core_animator_init();

  _core_pfx = eina_prefix_new(NULL,
                              core_init,
                              "ECORE",
                              "ecore",
                              "checkme",
                              EFL_BIN_DIR,
                              EFL_LIB_DIR,
                              PACKAGE_DATA_DIR,
                              PACKAGE_DATA_DIR);
  if (!_core_pfx)
  {
    ERR("Could not get ecore installation prefix");
    goto shutdown_log_dom;
  }

  efl_object_init();

  if (getenv("core_FPS_DEBUG")) _core_fps_debug = 1;
  if (_core_fps_debug) _core_fps_debug_init();
  if (!core_mempool_init()) goto shutdown_mempool;
  _core_main_loop_init();
  if (!_core_event_init()) goto shutdown_event;

  _core_signal_init();
  _core_exe_init();
  _core_thread_init();
  _core_job_init();
  _core_time_init();

  eina_lock_new(&_thread_mutex);
  eina_condition_new(&_thread_cond, &_thread_mutex);
  eina_lock_new(&_thread_feedback_mutex);
  eina_condition_new(&_thread_feedback_cond, &_thread_feedback_mutex);
  _thread_call = _core_pipe_add(_thread_callback, NULL);
  eina_lock_new(&_thread_safety);

  eina_lock_new(&_thread_id_lock);

  eina_lock_new(&_core_main_loop_lock);

#if defined(GLIB_INTEGRATION_ALWAYS)
  if (_core_glib_always_integrate) core_main_loop_glib_integrate();
#endif

#if defined(HAVE_MALLINFO) || defined(HAVE_MALLOC_INFO) || \
  defined(HAVE_MALLINFO2)
  if (getenv("CORE_MEM_STAT"))
  {
#  ifdef HAVE_MALLOC_INFO
    char tmp[1024];

    snprintf(tmp, sizeof(tmp), "core_mem_stat.%i", getpid());
    _core_memory_statistic_file = fopen(tmp, "wb");
#  endif
    _core_memory_pid = getpid();
    core_poller_add(CORE_POLLER_CORE, 1, _core_memory_statistic, NULL);
    _core_memory_statistic(NULL);
  }
#endif

#ifdef HAVE_SYSTEMD
  if (getenv("WATCHDOG_USEC"))
  {
    double sec = ((double)atoi(getenv("WATCHDOG_USEC"))) / 1000 / 1000;

    _systemd_watchdog =
      efl_add(EFL_LOOP_TIMER_CLASS,
              efl_main_loop_get(),
              efl_loop_timer_interval_set(efl_added, sec / 2),
              efl_event_callback_add(efl_added,
                                     EFL_LOOP_TIMER_EVENT_TIMER_TICK,
                                     _systemd_watchdog_cb,
                                     NULL));
    unsetenv("WATCHDOG_USEC");

    INF("Setup systemd watchdog to : %f", sec);
    _systemd_watchdog_cb(NULL, NULL);
  }
#endif

  if (_no_system_modules == 0xff)
  {
    const char *s = getenv("core_NO_SYSTEM_MODULES");
    if (s) _no_system_modules = atoi(s);
    else _no_system_modules = EFL_FALSE;
  }

  if (!_no_system_modules) core_system_modules_load();
  if (getenv("EFL_FIRST_LOOP"))
    efl_event_callback_add(efl_main_loop_get(),
                           EFL_APP_EVENT_RESUME,
                           _efl_first_loop_iterate,
                           getenv("EFL_FIRST_LOOP"));
  _core_init_count_threshold = _core_init_count;

  eina_log_timing(_core_log_dom, EINA_LOG_STATE_STOP, EINA_LOG_STATE_INIT);

  return _core_init_count;

shutdown_event:
  _core_event_shutdown();
  _core_main_shutdown();
shutdown_mempool:
  core_mempool_shutdown();
  efl_object_shutdown();
shutdown_log_dom:
  eina_shutdown();
#ifdef _WIN32
shutdown_evil:
  evil_shutdown();
#endif

  return --_core_init_count;
}

EAPI int
core_shutdown(void)
{
  Core_Pipe *p;
  /*
    * take a lock here because _core_event_shutdown() does callbacks
    */
  if (_core_init_count <= 0)
  {
    ERR("Init count not greater than 0 in shutdown.");
    return 0;
  }
  if (_core_init_count-- != _core_init_count_threshold) goto end;
  efl_event_callback_call(efl_main_loop_get(), EFL_APP_EVENT_TERMINATE, NULL);

  core_system_modules_unload();

  eina_log_timing(_core_log_dom, EINA_LOG_STATE_START, EINA_LOG_STATE_SHUTDOWN);

#ifdef HAVE_SYSTEMD
  if (_systemd_watchdog)
  {
    efl_del(_systemd_watchdog);
    _systemd_watchdog = NULL;
  }
#endif

  if (_core_fps_debug) _core_fps_debug_shutdown();
  _core_poller_shutdown();
  _core_animator_shutdown();
  _core_glib_shutdown();
  _core_job_shutdown();
  _core_thread_shutdown();

  /* this looks horrible - a hack for now, but something to note. as
    * we delete the _thread_call pipe a thread COULD be doing
    * core_pipe_write() or what not to it at the same time - we
    * must ensure all possible users of this _thread_call are finished
    * and exited before we delete it here */
  /*
    * ok - this causes other valgrind complaints regarding glib aquiring
    * locks internally. so fix bug a or bug b. let's leave the original
    * bug in then and leave this as a note for now
    */
  /*
    * It should be fine now as we do wait for thread to shutdown before
    * we try to destroy the pipe.
    */
  _core_pipe_wait(_thread_call, 1, 0);
  p            = _thread_call;
  _thread_call = NULL;
  _core_pipe_wait(p, 1, 0);
  _core_pipe_del(p);
  eina_lock_free(&_thread_safety);
  eina_condition_free(&_thread_cond);
  eina_lock_free(&_thread_mutex);
  eina_condition_free(&_thread_feedback_cond);
  eina_lock_free(&_thread_feedback_mutex);
  eina_lock_free(&_thread_id_lock);

  _core_exe_shutdown();
  _core_event_shutdown();
  _core_main_shutdown();
  _core_signal_shutdown();

  _core_main_loop_shutdown();

#if defined(HAVE_MALLINFO) || defined(HAVE_MALLOC_INFO) || \
  defined(HAVE_MALLINFO2)
  if (getenv("core_MEM_STAT"))
  {
    _core_memory_statistic(NULL);

#  ifdef HAVE_MALLINFO2
    ERR("[%i] Memory MAX total: %lu, free: %lu",
        _core_memory_pid,
        (unsigned long)_core_memory_max_total,
        (unsigned long)_core_memory_max_free);
#  else
    ERR("[%i] Memory MAX total: %lu, free: %lu",
        _core_memory_pid,
        (unsigned long)_core_memory_max_total,
        (unsigned long)_core_memory_max_free);
#  endif

#  ifdef HAVE_MALLOC_INFO
    fclose(_core_memory_statistic_file);
    _core_memory_statistic_file = NULL;
#  endif
  }
#endif
  core_mempool_shutdown();
  eina_log_domain_unregister(_core_log_dom);
  _core_log_dom = -1;

  eina_prefix_free(_core_pfx);
  _core_pfx = NULL;

  efl_object_shutdown();

  eina_evlog("<RUN", NULL, 0.0, NULL);
  eina_shutdown();
#ifdef _WIN32
  evil_shutdown();
#endif

end:
  return _core_init_count;
}

static unsigned int _core_init_ex = 0;

EAPI unsigned int
core_init_ex(int argc, char **argv)
{
  if (_core_init_ex++ != 0) return _core_init_ex;

  core_init();
  core_loop_arguments_send(argc, (argc > 0) ? ((const char **)argv) : NULL);
  core_app_args_set(argc, (const char **)argv);

  return _core_init_ex;
}

EAPI unsigned int
core_shutdown_ex(void)
{
  if (--_core_init_ex != 0) return _core_init_ex;

  core_shutdown();

  return _core_init_ex;
}

struct _core_Fork_Cb
{
  Core_Cb  func;
  void    *data;
  Efl_Bool delete_me : 1;
};

typedef struct _core_Fork_Cb core_Fork_Cb;

static int        fork_cbs_walking = 0;
static Eina_List *fork_cbs         = NULL;

EAPI Efl_Bool
core_fork_reset_callback_add(Core_Cb func, const void *data)
{
  core_Fork_Cb *fcb;

  if (!func) return EFL_FALSE;
  fcb = calloc(1, sizeof(core_Fork_Cb));
  if (!fcb) return EFL_FALSE;
  fcb->func = func;
  fcb->data = (void *)data;
  fork_cbs  = eina_list_append(fork_cbs, fcb);
  return EFL_TRUE;
}

EAPI Efl_Bool
core_fork_reset_callback_del(Core_Cb func, const void *data)
{
  Eina_List    *l;
  core_Fork_Cb *fcb;

  EINA_LIST_FOREACH(fork_cbs, l, fcb)
  {
    if ((fcb->func == func) && (fcb->data == data))
    {
      if (!fork_cbs_walking)
      {
        fork_cbs = eina_list_remove_list(fork_cbs, l);
        free(fcb);
      }
      else fcb->delete_me = EFL_TRUE;
      return EFL_TRUE;
    }
  }
  return EFL_FALSE;
}

EAPI void
core_fork_reset(void)
{
  Eina_List    *l, *ln;
  core_Fork_Cb *fcb;

  eina_debug_fork_reset();

  eina_main_loop_define();
  eina_lock_take(&_thread_safety);

  core_pipe_del(_thread_call);
  _thread_call = core_pipe_add(_thread_callback, NULL);
  /* If there was something in the pipe, trigger a wakeup again */
  if (_thread_cb)
  {
    core_Safe_Call *call;

    EINA_LIST_FOREACH_SAFE(_thread_cb, l, ln, call)
    {
      //if something is supsend, then the mainloop will be blocked until until thread is calling core_thread_main_loop_end()
      //if something tries to join a thread as callback, ensure that we remove this
      if (call->suspend || (call->cb.async == (Core_Cb)&_core_thread_join))
      {
        _thread_cb = eina_list_remove_list(_thread_cb, l);
        free(call);
      }
    }
    if (_thread_cb) core_pipe_write(_thread_call, &wakeup, sizeof(int));
  }

  eina_lock_release(&_thread_safety);

  // should this be done withing the eina lock stuff?
  fork_cbs_walking++;
  EINA_LIST_FOREACH(fork_cbs, l, fcb)
  {
    fcb->func(fcb->data);
  }
  fork_cbs_walking--;

  EINA_LIST_FOREACH_SAFE(fork_cbs, l, ln, fcb)
  {
    if (fcb->delete_me)
    {
      fork_cbs = eina_list_remove_list(fork_cbs, l);
      free(fcb);
    }
  }

#ifdef HAVE_SYSTEMD
  unsetenv("NOTIFY_SOCKET");
#endif
}

EAPI void
core_main_loop_thread_safe_call_async(Core_Cb callback, void *data)
{
  core_Safe_Call *order;

  if (!callback) return;

  if (eina_main_loop_is())
  {
    callback(data);
    return;
  }

  order = malloc(sizeof(core_Safe_Call));
  if (!order) return;

  order->cb.async = callback;
  order->data     = data;
  order->sync     = EFL_FALSE;
  order->suspend  = EFL_FALSE;

  _core_main_loop_thread_safe_call(order);
}

EAPI void *
core_main_loop_thread_safe_call_sync(Core_Data_Cb callback, void *data)
{
  core_Safe_Call *order;
  void           *ret;

  if (!callback) return NULL;

  if (eina_main_loop_is())
  {
    return callback(data);
  }

  order = malloc(sizeof(core_Safe_Call));
  if (!order) return NULL;

  order->cb.sync = callback;
  order->data    = data;
  eina_lock_new(&order->m);
  eina_condition_new(&order->c, &order->m);
  order->sync    = EFL_TRUE;
  order->suspend = EFL_FALSE;

  eina_lock_take(&order->m);
  _core_main_loop_thread_safe_call(order);
  eina_condition_wait(&order->c);
  eina_lock_release(&order->m);

  ret = order->data;

  order->sync     = EFL_FALSE;
  order->cb.async = _thread_safe_cleanup;
  order->data     = order;

  _core_main_loop_thread_safe_call(order);

  return ret;
}

EAPI void
core_main_loop_thread_safe_call_wait(double wait)
{
  core_pipe_wait(_thread_call, 1, wait);
}

static Efl_Id_Domain _core_main_domain = EFL_ID_DOMAIN_INVALID;

EAPI int
core_thread_main_loop_begin(void)
{
  core_Safe_Call *order;

  if (eina_main_loop_is())
  {
    return ++_thread_loop;
  }

  order = calloc(1, sizeof(core_Safe_Call));
  if (!order) return -1;

  eina_lock_take(&_thread_id_lock);
  order->current_id = ++_thread_id_max;
  if (order->current_id < 0)
  {
    _thread_id_max    = 0;
    order->current_id = ++_thread_id_max;
  }
  eina_lock_release(&_thread_id_lock);

  eina_lock_new(&order->m);
  eina_condition_new(&order->c, &order->m);
  order->suspend        = EFL_TRUE;
  order->eo_domain_data = NULL;

  _core_main_loop_thread_safe_call(order);

  eina_lock_take(&order->m);
  while (order->current_id != _thread_id)
    eina_condition_wait(&order->c);

  if (order->eo_domain_data)
  {
    _core_main_domain = efl_domain_data_adopt(order->eo_domain_data);
    if (_core_main_domain == EFL_ID_DOMAIN_INVALID)
      ERR("Cannot adopt mainloop eo domain");
  }

  eina_lock_release(&order->m);

  eina_main_loop_define();

  _thread_loop = 1;

  return _thread_loop;
}

EAPI int
core_thread_main_loop_end(void)
{
  int current_id;

  if (_thread_loop == 0)
  {
    ERR("the main loop is not locked ! No matching call to "
        "core_thread_main_loop_begin().");
    return -1;
  }

  /* until we unlock the main loop, this thread has the main loop id */
  if (!eina_main_loop_is())
  {
    ERR("Not in a locked thread !");
    return -1;
  }

  _thread_loop--;
  if (_thread_loop > 0) return _thread_loop;

  if (_core_main_domain != EFL_ID_DOMAIN_INVALID)
  {
    efl_domain_data_return(_core_main_domain);
    _core_main_domain = EFL_ID_DOMAIN_INVALID;
  }

  current_id = _thread_id;

  eina_lock_take(&_thread_mutex);
  _thread_id_update = _thread_id;
  eina_condition_broadcast(&_thread_cond);
  eina_lock_release(&_thread_mutex);

  eina_lock_take(&_thread_feedback_mutex);
  while (current_id == _thread_id && _thread_id != -1)
    eina_condition_wait(&_thread_feedback_cond);
  eina_lock_release(&_thread_feedback_mutex);

  return 0;
}

EAPI void
core_print_warning(const char *function EFL_UNUSED,
                   const char *sparam   EFL_UNUSED)
{
  WRN("***** Developer Warning ***** :\n"
      "\tThis program is calling:\n\n"
      "\t%s();\n\n"
      "\tWith the parameter:\n\n"
      "\t%s\n\n"
      "\tbeing NULL. Please fix your program.",
      function,
      sparam);
  if (getenv("core_ERROR_ABORT")) abort();
}

EAPI void
_core_magic_fail(const void       *d,
                 Core_Magic        m,
                 Core_Magic        req_m,
                 const char *fname EFL_UNUSED)
{
  ERR("*** ECORE ERROR: Ecore Magic Check Failed!!! in: %s()", fname);
  if (!d) ERR("    Input handle pointer is NULL!");
  else if (m == CORE_MAGIC_NONE)
    ERR("    Input handle has already been freed!");
  else if (m != req_m)
    ERR("    Input handle is wrong type\n"
        "      Expected: %08x - %s\n"
        "      Supplied: %08x - %s",
        (unsigned int)req_m,
        _core_magic_string_get(req_m),
        (unsigned int)m,
        _core_magic_string_get(m));

  if (getenv("core_ERROR_ABORT")) abort();
}

static const char *
_core_magic_string_get(Core_Magic m)
{
  switch (m)
  {
    case CORE_MAGIC_NONE:
      return "None (Freed Object)";
      break;

    case CORE_MAGIC_EXE:
      return "core_Exe (Executable)";
      break;

    case CORE_MAGIC_TIMER:
      return "core_Timer (Timer)";
      break;

    case CORE_MAGIC_IDLER:
      return "core_Idler (Idler)";
      break;

    case CORE_MAGIC_IDLE_ENTERER:
      return "core_Idle_Enterer (Idler Enterer)";
      break;

    case CORE_MAGIC_IDLE_EXITER:
      return "core_Idle_Exiter (Idler Exiter)";
      break;

    case CORE_MAGIC_FD_HANDLER:
      return "core_Fd_Handler (Fd Handler)";
      break;

    case CORE_MAGIC_WIN32_HANDLER:
      return "core_Win32_Handler (Win32 Handler)";
      break;

    case CORE_MAGIC_EVENT_HANDLER:
      return "core_Event_Handler (Event Handler)";
      break;

    case CORE_MAGIC_EVENT:
      return "core_Event (Event)";
      break;

    default:
      return "<UNKNOWN>";
  }
}

/* fps debug calls - for debugging how much time your app actually spends */
/* "running" (and the inverse being time spent running)... this does not */
/* account for other apps and multitasking... */

static int    _core_fps_debug_init_count = 0;
static int    _core_fps_debug_fd         = -1;
unsigned int *_core_fps_runtime_mmap     = NULL;

void
_core_fps_debug_init(void)
{
  char        buf[PATH_MAX];
  const char *tmp;
  int         pid;

  _core_fps_debug_init_count++;
  if (_core_fps_debug_init_count > 1) return;

  tmp = eina_environment_tmp_get();
  pid = (int)getpid();
  snprintf(buf, sizeof(buf), "%s/.core_fps_debug-%i", tmp, pid);
  _core_fps_debug_fd = open(buf, O_CREAT | O_BINARY | O_TRUNC | O_RDWR, 0644);
  if (_core_fps_debug_fd < 0)
  {
    unlink(buf);
    _core_fps_debug_fd = open(buf, O_CREAT | O_BINARY | O_TRUNC | O_RDWR, 0644);
  }
  if (_core_fps_debug_fd >= 0)
  {
    unsigned int zero = 0;
    char        *buf2 = (char *)&zero;
    ssize_t      todo = sizeof(unsigned int);

    while (todo > 0)
    {
      ssize_t r = write(_core_fps_debug_fd, buf2, todo);
      if (r > 0)
      {
        todo -= r;
        buf2 += r;
      }
      else if ((r < 0) && (errno == EINTR)) continue;
      else
      {
        ERR("could not write to file '%s' fd %d: %s",
            tmp,
            _core_fps_debug_fd,
            strerror(errno));
        close(_core_fps_debug_fd);
        _core_fps_debug_fd = -1;
        return;
      }
    }
    _core_fps_runtime_mmap = mmap(NULL,
                                  sizeof(unsigned int),
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED,
                                  _core_fps_debug_fd,
                                  0);
    if (_core_fps_runtime_mmap == MAP_FAILED) _core_fps_runtime_mmap = NULL;
  }
}

void
_core_fps_debug_shutdown(void)
{
  _core_fps_debug_init_count--;
  if (_core_fps_debug_init_count > 0) return;
  if (_core_fps_debug_fd >= 0)
  {
    char buf[4096];
    int  pid;

    pid = (int)getpid();
    snprintf(buf,
             sizeof(buf),
             "%s/.core_fps_debug-%i",
             eina_environment_tmp_get(),
             pid);
    unlink(buf);
    if (_core_fps_runtime_mmap)
    {
      munmap(_core_fps_runtime_mmap, sizeof(unsigned int));
      _core_fps_runtime_mmap = NULL;
    }
    close(_core_fps_debug_fd);
    _core_fps_debug_fd = -1;
  }
}

void
_core_fps_debug_runtime_add(double t)
{
  if ((_core_fps_debug_fd >= 0) && (_core_fps_runtime_mmap))
  {
    unsigned int tm;

    tm = (unsigned int)(t * 1000000.0);
    /* i know its not 100% theoretically guaranteed, but i'd say a write */
    /* of an int could be considered atomic for all practical purposes */
    /* oh and since this is cumulative, 1 second = 1,000,000 ticks, so */
    /* this can run for about 4294 seconds becore looping. if you are */
    /* doing performance testing in one run for over an hour... well */
    /* time to restart or handle a loop condition :) */
    *(_core_fps_runtime_mmap) += tm;
  }
}

#ifdef HAVE_SYSTEMD
static void
_systemd_watchdog_cb(void *data EFL_UNUSED, const Efl_Event *event EFL_UNUSED)
{
  if (getenv("NOTIFY_SOCKET"))
  {
    _core_sd_init();
    if (_core_sd_notify) _core_sd_notify(0, "WATCHDOG=1");
  }
}
#endif

#if defined(HAVE_MALLINFO) || defined(HAVE_MALLOC_INFO) || \
  defined(HAVE_MALLINFO2)
static Efl_Bool
_core_memory_statistic(EFL_UNUSED void *data)
{
#  ifdef HAVE_MALLOC_INFO
  static int frame = 0;
#  endif

#  if defined(HAVE_MALLINFO) || defined(HAVE_MALLINFO2)
  Efl_Bool changed = EFL_FALSE;

#    if defined(HAVE_MALLINFO2)
  struct mallinfo2 mi;
  static size_t    uordblks = 0;
  static size_t    fordblks = 0;

  mi = mallinfo2();
#    else
  struct mallinfo mi;
  static int      uordblks = 0;
  static int      fordblks = 0;

  mi = mallinfo();
#    endif

#    define HAS_CHANGED(Global, Local) \
        if (Global != Local)           \
        {                              \
            Global  = Local;           \
            changed = EFL_TRUE;        \
        }

  HAS_CHANGED(uordblks, mi.uordblks);
  HAS_CHANGED(fordblks, mi.fordblks);

  if (changed)
  {
#    ifdef HAVE_MALLINFO2
    ERR("[%i] Memory total: %lu, free: %lu",
        _core_memory_pid,
        (unsigned long)mi.uordblks,
        (unsigned long)mi.fordblks);
#    else
    ERR("[%i] Memory total: %lu, free: %lu",
        _core_memory_pid,
        (unsigned long)mi.uordblks,
        (unsigned long)mi.fordblks);
#    endif
  }

  KEEP_MAX(_core_memory_max_total, mi.uordblks);
  KEEP_MAX(_core_memory_max_free, mi.fordblks);
#  endif

#  ifdef HAVE_MALLOC_INFO
  if (frame) fputs("\n", _core_memory_statistic_file);
  malloc_info(0, _core_memory_statistic_file);
#  endif

  return CORE_CALLBACK_RENEW;
}

#endif

static void
_core_main_loop_thread_safe_call(core_Safe_Call *order)
{
  Efl_Bool count;

  eina_lock_take(&_thread_safety);

  count      = _thread_cb ? 0 : 1;
  _thread_cb = eina_list_append(_thread_cb, order);
  if (count) core_pipe_write(_thread_call, &wakeup, sizeof(int));

  eina_lock_release(&_thread_safety);
}

static void
_thread_safe_cleanup(void *data)
{
  core_Safe_Call *call = data;

  eina_condition_free(&call->c);
  eina_lock_free(&call->m);
}

void
_core_main_call_flush(void)
{
  core_Safe_Call *call;
  Eina_List      *callback;

  eina_lock_take(&_thread_safety);
  callback   = _thread_cb;
  _thread_cb = NULL;
  eina_lock_release(&_thread_safety);

  EINA_LIST_FREE(callback, call)
  {
    if (call->suspend)
    {
      eina_lock_take(&_thread_mutex);

      eina_lock_take(&call->m);
      _thread_id           = call->current_id;
      call->eo_domain_data = efl_domain_data_get();
      eina_condition_broadcast(&call->c);
      eina_lock_release(&call->m);

      while (_thread_id_update != _thread_id)
        eina_condition_wait(&_thread_cond);
      eina_lock_release(&_thread_mutex);

      eina_main_loop_define();

      eina_lock_take(&_thread_feedback_mutex);

      _thread_id = -1;

      eina_condition_broadcast(&_thread_feedback_cond);
      eina_lock_release(&_thread_feedback_mutex);

      _thread_safe_cleanup(call);
      free(call);
    }
    else if (call->sync)
    {
      call->data = call->cb.sync(call->data);
      eina_lock_take(&call->m);
      eina_condition_broadcast(&call->c);
      eina_lock_release(&call->m);
    }
    else
    {
      call->cb.async(call->data);
      free(call);
    }
  }
}

static void
_thread_callback(void *data         EFL_UNUSED,
                 void *buffer       EFL_UNUSED,
                 unsigned int nbyte EFL_UNUSED)
{
  _core_main_call_flush();
}

EAPI Core_Power_State
core_power_state_get(void)
{
  return _core_power_state;
}

EAPI void
core_power_state_set(Core_Power_State state)
{
  if (_core_power_state == state) return;
  _core_power_state = state;
  core_event_add(CORE_EVENT_POWER_STATE, NULL, NULL, NULL);
}

EAPI Core_Memory_State
core_memory_state_get(void)
{
  return _core_memory_state;
}

EAPI void
core_memory_state_set(Core_Memory_State state)
{
  if (_core_memory_state == state) return;
  _core_memory_state = state;
  core_event_add(CORE_EVENT_MEMORY_STATE, NULL, NULL, NULL);
}

#ifdef HAVE_SYSTEMD
static Eina_Module *_libsystemd        = NULL;
static Efl_Bool     _libsystemd_broken = EFL_FALSE;

int (*_core_sd_notify)(int unset_environment, const char *state) = NULL;

void
_core_sd_init(void)
{
  if (_libsystemd_broken) return;
  _libsystemd = eina_module_new("libsystemd.so.0");
  if (_libsystemd)
  {
    if (!eina_module_load(_libsystemd))
    {
      eina_module_free(_libsystemd);
      _libsystemd = NULL;
    }
  }
  if (!_libsystemd)
  {
    _libsystemd_broken = EFL_TRUE;
    return;
  }
  _core_sd_notify = eina_module_symbol_get(_libsystemd, "sd_notify");
  if (!_core_sd_notify)
  {
    _core_sd_notify = NULL;
    eina_module_free(_libsystemd);
    _libsystemd        = NULL;
    _libsystemd_broken = EFL_TRUE;
  }
}
#endif
