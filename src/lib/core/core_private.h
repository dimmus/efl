#ifndef _CORE_PRIVATE_H
#define _CORE_PRIVATE_H

#include <assert.h>

#include "core_internal.h"

#include "core_exe_eo.h"
#include "core_event_message.eo.h"
#include "core_event_message_handler.eo.h"

#ifndef EFL_NOLEGACY_API_SUPPORT
# include "core_legacy.h"
#endif

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

extern int _core_log_dom;
#ifdef  _CORE_DEFAULT_LOG_DOM
# undef _CORE_DEFAULT_LOG_DOM
#endif
#define _CORE_DEFAULT_LOG_DOM _core_log_dom

#ifdef CORE_DEFAULT_LOG_COLOR
# undef CORE_DEFAULT_LOG_COLOR
#endif
#define CORE_DEFAULT_LOG_COLOR EINA_COLOR_BLUE

#ifdef ERR
# undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_CORE_DEFAULT_LOG_DOM, __VA_ARGS__)

#ifdef DBG
# undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_CORE_DEFAULT_LOG_DOM, __VA_ARGS__)

#ifdef INF
# undef INF
#endif
#define INF(...) EINA_LOG_DOM_INFO(_CORE_DEFAULT_LOG_DOM, __VA_ARGS__)

#ifdef WRN
# undef WRN
#endif
#define WRN(...) EINA_LOG_DOM_WARN(_CORE_DEFAULT_LOG_DOM, __VA_ARGS__)

#ifdef CRI
# undef CRI
#endif
#define CRI(...) EINA_LOG_DOM_CRIT(_CORE_DEFAULT_LOG_DOM, __VA_ARGS__)

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

#ifndef ABS
# define ABS(x)             ((x) < 0 ? -(x) : (x))
#endif

#ifndef CLAMP
# define CLAMP(x, min, max) (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))
#endif

typedef struct _Core_Factorized_Idle Core_Factorized_Idle;

typedef struct _Efl_Loop_Promise_Simple_Data Efl_Loop_Promise_Simple_Data;
typedef struct _Efl_Loop_Timer_Data Efl_Loop_Timer_Data;
typedef struct _Efl_Loop_Data Efl_Loop_Data;
typedef struct _Efl_Task_Data Efl_Task_Data;
typedef struct _Efl_Appthread_Data Efl_Appthread_Data;

typedef struct _Message Message;

struct _Message
{
   EINA_INLIST;
   Eo *handler;
   Eo *message;
   Efl_Bool delete_me;
};

struct _Efl_Loop_Data
{
   double               loop_time;

   Efl_Loop_Message_Handler *future_message_handler;

   Efl_Loop_Timer      *poll_high;
   Efl_Loop_Timer      *poll_medium;
   Efl_Loop_Timer      *poll_low;

   Eina_List           *exes; // only used in main loop (for now?)

   Eina_List           *fd_handlers_obj;

   Core_Fd_Handler    *fd_handlers;
   Eina_List           *fd_handlers_with_prep;
   Eina_List           *file_fd_handlers;
   Eina_List           *always_fd_handlers;
   Eina_List           *fd_handlers_with_buffer;
   Eina_List           *fd_handlers_to_delete;
   Core_Fd_Handler    *fd_handlers_to_call;
   Core_Fd_Handler    *fd_handlers_to_call_current;

# ifdef _WIN32
   Core_Win32_Handler *win32_handlers;
   Core_Win32_Handler *win32_handler_current;
   Eina_List           *win32_handlers_to_delete;
# endif

   Eina_List           *thread_children;

   Eina_Inlist         *message_queue;
   Eina_Inlist         *message_pending_queue;
   unsigned int         message_walking;

   unsigned int         throttle;

   int                  epoll_fd;
   pid_t                epoll_pid;
   int                  timer_fd;

   double               last_check;
   Eina_Inlist         *timers;
   Eina_Inlist         *suspended;
   Efl_Loop_Timer_Data *timer_current;
   int                  timers_added;

   Eina_Value           exit_code;

   int                  idlers;
   int                  in_loop;
   unsigned int         loop_active;

   struct {
      int               high;
      int               medium;
      int               low;
   } pollers;

   struct {
      char      **environ_ptr;
      char      **environ_copy;
   } env;

   Efl_Bool            do_quit : 1;
   Efl_Bool            quit_on_last_thread_child_del : 1;
};

struct _Efl_Task_Data
{
   Eina_Stringshare  *command;
   Eina_Array        *args;
   Efl_Task_Priority  priority;
   int                exit_code;
   Efl_Task_Flags     flags;
   Efl_Bool          command_dirty : 1;
   Efl_Bool          exited : 1;
};

struct _Efl_Appthread_Data
{
   int read_listeners;
   struct {
      int in, out;
      Eo *in_handler, *out_handler;
      Efl_Bool can_read : 1;
      Efl_Bool eos_read : 1;
      Efl_Bool can_write : 1;
   } fd, ctrl;
   void *thdat;
};

struct _Core_Animator
{
   EINA_INLIST;

   Core_Task_Cb     func;
   void             *data;

   double            start, run;
   Core_Timeline_Cb run_func;
   void             *run_data;

   void             *ee;

   Efl_Bool         delete_me : 1;
   Efl_Bool         suspended : 1;
   Efl_Bool         just_added : 1;
};

typedef struct _Core_Evas_Object_Animator_Interface
{
   Core_Animator *(*timeline_add)(void *obj, double runtime, Core_Timeline_Cb func, const void *data);
   Core_Animator *(*add)(void *obj, Core_Task_Cb func, const void *data);
   void (*freeze)(Core_Animator *animator);
   void (*thaw)(Core_Animator *animator);
   void *(*del)(Core_Animator *animator);
} Core_Evas_Object_Animator_Interface;

EAPI void core_evas_object_animator_init(Core_Evas_Object_Animator_Interface *iface);

#define EVAS_FRAME_QUEUING        1 /* for test */

#define READBUFSIZ                65536

#define CORE_MAGIC_NONE          0x1234fedc
#define CORE_MAGIC_EXE           0xf7e812f5
#define CORE_MAGIC_TIMER         0xf7d713f4
#define CORE_MAGIC_IDLER         0xf7c614f3
#define CORE_MAGIC_IDLE_ENTERER  0xf7b515f2
#define CORE_MAGIC_IDLE_EXITER   0xf7601afd
#define CORE_MAGIC_FD_HANDLER    0xf7a416f1
#define CORE_MAGIC_EVENT_HANDLER 0xf79317f0
#define CORE_MAGIC_EVENT_FILTER  0xf78218ff
#define CORE_MAGIC_EVENT         0xf77119fe
#define CORE_MAGIC_ANIMATOR      0xf7643ea5
#define CORE_MAGIC_POLLER        0xf7568127
#define CORE_MAGIC_PIPE          0xf7458226
#define CORE_MAGIC_WIN32_HANDLER 0xf7e8f1a3
#define CORE_MAGIC_JOB           0x76543210

typedef unsigned int Core_Magic;
#define CORE_MAGIC               Core_Magic __magic

#define CORE_MAGIC_SET(d, m)      (d)->__magic = (m)
#define CORE_MAGIC_CHECK(d, m)    ((d) && ((d)->__magic == (m)))
#define CORE_MAGIC_FAIL(d, m, fn) _core_magic_fail((d), (d) ? (d)->__magic : 0, (m), (fn));

/* undef the following, we want our version */
#undef FREE
#define FREE(ptr)                  free(ptr); ptr = NULL;

#undef IF_FREE
#define IF_FREE(ptr)               if (ptr) free(ptr); ptr = NULL;

#undef IF_FN_DEL
#define IF_FN_DEL(_fn, ptr)        if (ptr) { _fn(ptr); ptr = NULL; }

EAPI void
core_print_warning(const char *function,
                    const char *sparam);

/* convenience macros for checking pointer parameters for non-NULL */
#undef CHECK_PARAM_POINTER_RETURN
#define CHECK_PARAM_POINTER_RETURN(sparam, param, ret) \
  if (!(param))                                        \
    {                                                  \
       core_print_warning(__func__, sparam);      \
       return ret;                                     \
    }

#undef CHECK_PARAM_POINTER
#define CHECK_PARAM_POINTER(sparam, param)        \
  if (!(param))                                   \
    {                                             \
       core_print_warning(__func__, sparam); \
       return;                                    \
    }

EAPI void _core_magic_fail(const void *d,
                            Core_Magic m,
                            Core_Magic req_m,
                            const char *fname);

void         _core_time_init(void);

void        *_efl_loop_timer_del(Core_Timer *timer);
void         _efl_loop_timer_enable_new(Eo *obj, Efl_Loop_Data *pd);
double       _efl_loop_timer_next_get(Eo *obj, Efl_Loop_Data *pd);
void         _efl_loop_timer_expired_timers_call(Eo *obj, Efl_Loop_Data *pd, double when);
int          _efl_loop_timers_exists(Eo *obj, Efl_Loop_Data *pd);
int          _efl_loop_timer_expired_call(Eo *obj, Efl_Loop_Data *pd, double when);

Core_Factorized_Idle *_core_factorized_idle_add(const Efl_Callback_Array_Item*desc,
                                                  Core_Task_Cb func,
                                                  const void   *data);
void        *_core_factorized_idle_del(Core_Idler *idler);
void         _core_factorized_idle_process(void *data, const Efl_Event *event);
void         _core_factorized_idle_event_del(void *data, const Efl_Event *event);

Efl_Bool     _core_event_init(void);
void         _core_event_shutdown(void);
int          _core_event_exist(void);
Core_Event *_core_event_add(int type,
                              void *ev,
                              Core_End_Cb func_free,
                              void *data);
void         _core_event_call(void);

Efl_Loop_Timer *_core_exe_doomsday_clock_get(Core_Exe *exe);
void            _core_exe_doomsday_clock_set(Core_Exe *exe, Efl_Loop_Timer *dc);

void       *_core_event_signal_user_new(void);
void       *_core_event_signal_hup_new(void);
void       *_core_event_signal_exit_new(void);
void       *_core_event_signal_power_new(void);
void       *_core_event_signal_realtime_new(void);

Core_Pipe *_core_pipe_add(Core_Pipe_Cb handler,
                            const void   *data);
int         _core_pipe_wait(Core_Pipe *p,
                             int message_count,
                             double wait);
void       *_core_pipe_del(Core_Pipe *p);

Core_Fd_Handler *_core_main_fd_handler_add(Eo *obj,
                                             Efl_Loop_Data *pd,
                                             Eo *handler,
                                             int fd,
                                             Core_Fd_Handler_Flags flags,
                                             Core_Fd_Cb func,
                                             const void *data,
                                             Core_Fd_Cb buf_func,
                                             const void *buf_data,
                                             Efl_Bool is_file);
void      *_core_main_fd_handler_del(Eo *obj,
                                      Efl_Loop_Data *pd,
                                      Core_Fd_Handler *fd_handler);
Core_Win32_Handler *
_core_main_win32_handler_add(Eo                    *obj,
                              Efl_Loop_Data         *pd,
                              Eo                    *handler,
                              void                  *h,
                              Core_Win32_Handle_Cb  func,
                              const void            *data);
void *
_core_main_win32_handler_del(Eo *obj,
                              Efl_Loop_Data *pd,
                              Core_Win32_Handler *win32_handler);

void       _core_main_content_clear(Eo *obj, Efl_Loop_Data *pd);
void       _core_main_shutdown(void);

#if defined (_WIN32) || defined (__lv2ppu__)
static inline void _core_signal_shutdown(void) { }
static inline void _core_signal_init(void) { }
static inline void _core_signal_received_process(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd EFL_UNUSED) { }
static inline int  _core_signal_count_get(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd EFL_UNUSED) { return 0; }
static inline void _core_signal_call(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd EFL_UNUSED) { }
#else
#define CORE_SIGNALS 1
typedef struct _Core_Signal_Pid_Info Core_Signal_Pid_Info;

struct _Core_Signal_Pid_Info
{
   pid_t pid;
   int exit_code;
   int exit_signal;
   siginfo_t info;
};

void _core_signal_shutdown(void);
void _core_signal_init(void);
void _core_signal_received_process(Eo *obj, Efl_Loop_Data *pd);
int  _core_signal_count_get(Eo *obj, Efl_Loop_Data *pd);
void _core_signal_call(Eo *obj, Efl_Loop_Data *pd);
void _core_signal_pid_lock(void);
void _core_signal_pid_unlock(void);
void _core_signal_pid_register(pid_t pid, int fd);
void _core_signal_pid_unregister(pid_t pid, int fd);

#endif

void       _core_exe_init(void);
void       _core_exe_shutdown(void);
#ifndef _WIN32
Core_Exe *_core_exe_find(pid_t pid);
void      *_core_exe_event_del_new(void);
void       _core_exe_event_del_free(void *data,
                                     void *ev);
#endif

void     _core_animator_init(void);
void     _core_animator_shutdown(void);
void     _core_animator_run_reset(void);
Efl_Bool _core_animator_run_get(void);
Efl_Bool _core_animator_flush(void);

void _core_poller_shutdown(void);

void _core_fps_debug_init(void);
void _core_fps_debug_shutdown(void);
void _core_fps_debug_runtime_add(double t);

void _core_thread_init(void);
void _core_thread_shutdown(void);

void _core_glib_init(void);
void _core_glib_shutdown(void);

void _core_job_init(void);
void _core_job_shutdown(void);

void _core_main_loop_init(void);
void _core_main_loop_shutdown(void);
void _core_main_loop_iterate(Eo *obj, Efl_Loop_Data *pd);
int  _core_main_loop_iterate_may_block(Eo *obj, Efl_Loop_Data *pd, int may_block);
void _core_main_loop_begin(Eo *obj, Efl_Loop_Data *pd);
void _core_main_loop_quit(Eo *obj, Efl_Loop_Data *pd);

void _core_coroutine_init(void);
void _core_coroutine_shutdown(void);

void _core_throttle(void);

void _core_main_call_flush(void);

void _core_main_timechanges_start(Eo *obj);
void _core_main_timechanges_stop(Eo *obj);

Efl_Bool _core_event_do_filter(void *handler_pd, Eo *msg_handler, Eo *msg);
void     _core_event_filters_call(Eo *obj, Efl_Loop_Data *pd);

void _efl_loop_messages_filter(Eo *obj, Efl_Loop_Data *pd, void *handler_pd);
void _efl_loop_messages_call(Eo *obj, Efl_Loop_Data *pd, void *func, void *data);
void _efl_loop_message_send_info_set(Eo *obj, Eina_Inlist *node, Eo *loop, Efl_Loop_Data *loop_data);
void _efl_loop_message_unsend(Eo *obj);

void _efl_thread_child_remove(Eo *loop, Efl_Loop_Data *pd, Eo *child);

static inline Efl_Bool
_core_call_task_cb(Core_Task_Cb func,
                    void *data)
{
   return func(data);
}

static inline void *
_core_call_data_cb(Core_Data_Cb func,
                    void *data)
{
   return func(data);
}

static inline void
_core_call_end_cb(Core_End_Cb func,
                   void *user_data,
                   void *func_data)
{
   func(user_data, func_data);
}

static inline Efl_Bool
_core_call_filter_cb(Core_Filter_Cb func,
                      void *data,
                      void *loop_data,
                      int type,
                      void *event)
{
   return func(data, loop_data, type, event);
}

static inline Efl_Bool
_core_call_handler_cb(Core_Event_Handler_Cb func,
                       void *data,
                       int type,
                       void *event)
{
   return func(data, type, event);
}

static inline void
_core_call_prep_cb(Core_Fd_Prep_Cb func,
                    void *data,
                    Core_Fd_Handler *fd_handler)
{
   func(data, fd_handler);
}

static inline Efl_Bool
_core_call_fd_cb(Core_Fd_Cb func,
                  void *data,
                  Core_Fd_Handler *fd_handler)
{
   return func(data, fd_handler);
}

extern int _core_fps_debug;
extern Efl_Bool _core_glib_always_integrate;
extern Core_Select_Function main_loop_select;
extern int in_main_loop;

#ifdef HAVE_SYSTEMD
void _core_sd_init(void);

extern int (*_core_sd_notify) (int unset_environment, const char *state);
#endif

Efl_Bool core_mempool_init(void);
void core_mempool_shutdown(void);
#define GENERIC_ALLOC_FREE_HEADER(TYPE, Type) \
  TYPE *Type##_calloc(unsigned int);          \
  void Type##_mp_free(TYPE *e);
#define GENERIC_ALLOC_SIZE_DECLARE(TYPE)      \
  size_t _core_sizeof_##TYPE = sizeof (TYPE);

//GENERIC_ALLOC_FREE_HEADER(Core_Animator, core_animator);
//GENERIC_ALLOC_FREE_HEADER(Core_Event_Handler, core_event_handler);
//GENERIC_ALLOC_FREE_HEADER(Core_Event_Filter, core_event_filter);
//GENERIC_ALLOC_FREE_HEADER(Core_Event, core_event);
//GENERIC_ALLOC_FREE_HEADER(Core_Idle_Exiter, core_idler_exiter);
//GENERIC_ALLOC_FREE_HEADER(Core_Idle_Enterer, core_idler_enterer);
//GENERIC_ALLOC_FREE_HEADER(Core_Idler, core_idler);
//GENERIC_ALLOC_FREE_HEADER(Core_Job, core_job);
//GENERIC_ALLOC_FREE_HEADER(Core_Timer, core_timer);
//GENERIC_ALLOC_FREE_HEADER(Core_Poller, core_poller);
GENERIC_ALLOC_FREE_HEADER(Core_Pipe, core_pipe);
GENERIC_ALLOC_FREE_HEADER(Core_Fd_Handler, core_fd_handler);
GENERIC_ALLOC_FREE_HEADER(Efl_Loop_Promise_Simple_Data, efl_loop_promise_simple_data);
#ifdef _WIN32
GENERIC_ALLOC_FREE_HEADER(Core_Win32_Handler, core_win32_handler);
#endif

#undef GENERIC_ALLOC_FREE_HEADER

extern Eo *_mainloop_singleton;
extern Efl_Loop_Data *_mainloop_singleton_data;
#define ML_OBJ _mainloop_singleton
#define ML_DAT _mainloop_singleton_data
//#define ML_DAT efl_data_scope_get(ML_OBJ, EFL_LOOP_CLASS)

extern Efl_Version _app_efl_version;

// access to direct input cb
#define CORE_EVAS_INTERNAL

#define EFL_LOOP_DATA efl_data_scope_get(efl_main_loop_get(), EFL_LOOP_CLASS)

#undef EAPI
#define EAPI

#endif
