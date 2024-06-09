#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#define EINA_SLSTR_INTERNAL

// as per:
//
// https://forums.freebsd.org/threads/solved-enlightenment-application-menu-icons-missing-due-to-efreetd-crashing.80743/
//
// from Euan Thoms (ethomas) <euan@potensol.com>
//
// normal fd set size (maximum fd num allowed) is limited to 1024. raise this
// to something more useful than the default 1024 ... this doesn't affect
// linux as it will use epoll. beware that this also will be a performance
// hit - the higher this number the more it costs to zero and re-fill the fd
// set every time we go into select.
#define FD_SETSIZE EFL_MAX_FD_SIZE

#ifdef _WIN32
#  ifndef USER_TIMER_MINIMUM
#    define USER_TIMER_MINIMUM 0x0a
#  endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>

#ifdef HAVE_IEEEFP_H
#  include <ieeefp.h>  // for Solaris
#endif

#ifdef HAVE_ISFINITE
#  define Core_FINITE(t) isfinite(t)
#else
#  define Core_FINITE(t) finite(t)
#endif

//#define FIX_HZ 1

#ifdef FIX_HZ
#  include <sys/param.h>
#  ifndef HZ
#    define HZ 100
#  endif
#endif

#ifdef _WIN32
#  include <evil_private.h> /* evil_last_error_get */
#endif
#include "Efl_Core.h"
#include "core_private.h"

#include "core_main_common.h"

#include "eina_internal.h"

#ifdef USE_G_MAIN_LOOP
#  include <glib.h>
#endif

#ifdef HAVE_LIBUV
#  ifdef HAVE_NODE_UV_H
#    include <node/uv.h>
#  elif defined(HAVE_NODEJS_DEPS_UV_UV_H)
#    include <nodejs/deps/uv/uv.h>
#  elif defined(HAVE_NODEJS_DEPS_UV_INCLUDE_UV_H)
#    include <nodejs/deps/uv/include/uv.h>
#  elif defined(HAVE_NODEJS_SRC_UV_H)
#    include <nodejs/src/uv.h>
#  elif defined(HAVE_UV_H)
#    include <uv.h>
#  else
#    error No uv.h header found?
#  endif

// XXX: FIXME: use eina_module
#  if defined(HAVE_DLOPEN) && !defined(_WIN32)
#    include <dlfcn.h>
#  endif

static uv_prepare_t _core_main_uv_prepare;
static uv_check_t   _core_main_uv_check;
static uv_timer_t   _core_main_uv_handle_timers;
static Efl_Bool     _core_main_uv_idling;

static int (*_dl_uv_loop_alive)(uv_loop_t *)                               = 0;
static int (*_dl_uv_run)(uv_loop_t *, uv_run_mode mode)                    = 0;
static int (*_dl_uv_stop)(uv_loop_t *)                                     = 0;
static uv_loop_t *(*_dl_uv_default_loop)()                                 = 0;
static int (*_dl_uv_poll_init_socket)(uv_loop_t   *loop,
                                      uv_poll_t   *handle,
                                      uv_os_sock_t fd)                     = 0;
static int (*_dl_uv_poll_init)(uv_loop_t *loop, uv_poll_t *handle, int fd) = 0;
static int (*_dl_uv_poll_start)(uv_poll_t *handle,
                                int        events,
                                uv_poll_cb cb)                             = 0;
static int (*_dl_uv_poll_stop)(uv_poll_t *handle)                          = 0;
static int (*_dl_uv_timer_init)(uv_loop_t *, uv_timer_t *handle);
static int (*_dl_uv_timer_start)(uv_timer_t *handle,
                                 uv_timer_cb cb,
                                 uint64_t    timeout,
                                 uint64_t    repeat);
static int (*_dl_uv_timer_stop)(uv_timer_t *handle);
static int (*_dl_uv_prepare_init)(uv_loop_t *, uv_prepare_t *prepare);
static int (*_dl_uv_prepare_start)(uv_prepare_t *prepare, uv_prepare_cb cb);
static int (*_dl_uv_prepare_stop)(uv_prepare_t *prepare);
static int (*_dl_uv_check_init)(uv_loop_t *, uv_check_t *prepare);
static int (*_dl_uv_check_start)(uv_check_t *prepare, uv_check_cb cb);
static int (*_dl_uv_check_stop)(uv_check_t *prepare);
static int (*_dl_uv_close)(uv_handle_t *handle, uv_close_cb close_cb);
#endif

#define NS_PER_SEC (1000000000.0)

struct _Core_Fd_Handler
{
  EINA_INLIST;
  CORE_MAGIC;
  Core_Fd_Handler      *next_ready;
  int                   fd;
  Core_Fd_Handler_Flags flags;
  Eo                   *handler;
  Eo                   *loop;
  Efl_Loop_Data        *loop_data;
  Core_Fd_Cb            func;
  void                 *data;
  Core_Fd_Cb            buf_func;
  void                 *buf_data;
  Core_Fd_Prep_Cb       prep_func;
  void                 *prep_data;
  int                   references;
#if defined(USE_G_MAIN_LOOP)
  GPollFD gfd;
#endif
#ifdef HAVE_LIBUV
  uv_poll_t uv_handle;
#endif
  Efl_Bool read_active  : 1;
  Efl_Bool write_active : 1;
  Efl_Bool error_active : 1;
  Efl_Bool delete_me    : 1;
  Efl_Bool file         : 1;
  Efl_Bool legacy       : 1;
};

GENERIC_ALLOC_SIZE_DECLARE(Core_Fd_Handler);

#ifdef _WIN32
struct _Core_Win32_Handler
{
  EINA_INLIST;
  CORE_MAGIC;
  HANDLE               h;
  Eo                  *handler;
  Eo                  *loop;
  Efl_Loop_Data       *loop_data;
  Core_Win32_Handle_Cb func;
  void                *data;
  int                  references;
  Efl_Bool             delete_me : 1;
};

GENERIC_ALLOC_SIZE_DECLARE(Core_Win32_Handler);
#endif

#if !defined(USE_G_MAIN_LOOP) && !defined(HAVE_LIBUV)
static int _core_main_select(Eo *obj, Efl_Loop_Data *pd, double timeout);
#endif
static void _core_main_prepare_handlers(Eo *obj, Efl_Loop_Data *pd);
static void _core_main_fd_handlers_cleanup(Eo *obj, Efl_Loop_Data *pd);
#ifndef _WIN32
#  ifndef USE_G_MAIN_LOOP
static void _core_main_fd_handlers_bads_rem(Eo *obj, Efl_Loop_Data *pd);
#  endif
#endif
static void _core_main_fd_handlers_call(Eo *obj, Efl_Loop_Data *pd);
static int  _core_main_fd_handlers_buf_call(Eo *obj, Efl_Loop_Data *pd);
#ifndef USE_G_MAIN_LOOP
static void
_core_main_loop_iterate_internal(Eo *obj, Efl_Loop_Data *pd, int once_only);
#endif

#ifdef _WIN32
static int  _core_main_win32_select(int             nfds,
                                    fd_set         *readfds,
                                    fd_set         *writefds,
                                    fd_set         *exceptfds,
                                    struct timeval *timeout);
static void _core_main_win32_handlers_cleanup(Eo *obj, Efl_Loop_Data *pd);
#endif

static void _core_main_loop_setup(Eo *obj, Efl_Loop_Data *pd);
static void _core_main_loop_clear(Eo *obj, Efl_Loop_Data *pd);

// for legacy mainloop only and not other loops
int in_main_loop = 0;
#ifndef USE_G_MAIN_LOOP
static double t1 = 0.0;
static double t2 = 0.0;
#endif

#ifdef _WIN32
Core_Select_Function        main_loop_select    = _core_main_win32_select;
static Core_Select_Function general_loop_select = _core_main_win32_select;
#else
#  include <sys/select.h>
Core_Select_Function        main_loop_select    = select;
static Core_Select_Function general_loop_select = select;
#endif

#ifdef USE_G_MAIN_LOOP
static GPollFD    Core_epoll_fd;
static GPollFD    core_timer_fd;
static GSource   *Core_glib_source;
static guint      Core_glib_source_id;
static GMainLoop *core_main_loop;
static gboolean   Core_idling;
static gboolean   _Core_glib_idle_enterer_called;
static gboolean   Core_fds_ready;
#endif

static double _core_main_loop_wakeup_time = 0.0;

EAPI double
_core_main_loop_wakeup_time_get(void)
{
  return _core_main_loop_wakeup_time;
}

static inline void
_update_loop_time(Efl_Loop_Data *pd)
{
  double loop_time = core_time_get();
  if (loop_time > pd->loop_time) pd->loop_time = loop_time;
  _core_main_loop_wakeup_time = loop_time;
}

#ifdef EFL_EXTRA_SANITY_CHECKS
static inline void
_core_fd_valid(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd EFL_UNUSED)
{
#  ifdef HAVE_SYS_EPOLL_H
  if ((pd->epoll_fd >= 0) && (fcntl(pd->epoll_fd, F_GETFD) < 0))
  {
    ERR("arghhh you caught me! report a backtrace to edevel!");
#    ifdef HAVE_PAUSE
    pause();
#    else
    sleep(60);
#    endif
  }
#  endif
}
#endif

static inline void
_Core_try_add_to_call_list(Eo *obj          EFL_UNUSED,
                           Efl_Loop_Data   *pd,
                           Core_Fd_Handler *fdh)
{
  // check if this fdh is already in the list
  if (fdh->next_ready)
  {
    DBG("next_ready");
    return;
  }
  if (fdh->read_active || fdh->write_active || fdh->error_active ||
      (fdh->flags & CORE_FD_ALWAYS))
  {
    DBG("added");
    // make sure next_ready is non-null by pointing to ourselves
    // use that to indicate this fdh is in the ready list
    // insert at the head of the list to avoid trouble
    fdh->next_ready = pd->fd_handlers_to_call ? pd->fd_handlers_to_call : fdh;
    pd->fd_handlers_to_call = fdh;
  }
}

static inline void
_throttle_do(Efl_Loop_Data *pd)
{
  if (pd->throttle == 0) return;
  eina_evlog("+throttle", NULL, 0.0, NULL);
  usleep(pd->throttle);
  eina_evlog("-throttle", NULL, 0.0, NULL);
}

#ifdef HAVE_SYS_EPOLL_H
static inline int
_Core_get_epoll_fd(Eo *obj, Efl_Loop_Data *pd)
{
  if (pd->epoll_pid && (pd->epoll_pid != getpid()))  // forked!
    _core_main_loop_clear(obj, pd);
  if ((pd->epoll_pid == 0) && (pd->epoll_fd < 0))
    _core_main_loop_setup(obj, pd);
  return pd->epoll_fd;
}

static inline int
_Core_epoll_add(int efd, int fd, int events, void *ptr)
{
  struct epoll_event ev;

  memset(&ev, 0, sizeof(ev));
  ev.events   = events;
  ev.data.ptr = ptr;
  DBG("adding poll on %d %08x", fd, events);
  return epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
}

static inline int
_Core_poll_events_from_fdh(Core_Fd_Handler *fdh)
{
  int events = 0;
  if (fdh->flags & CORE_FD_READ) events |= EPOLLIN | EPOLLHUP;
  if (fdh->flags & CORE_FD_WRITE) events |= EPOLLOUT | EPOLLHUP;
  if (fdh->flags & CORE_FD_ERROR) events |= EPOLLERR | EPOLLPRI | EPOLLHUP;
  return events;
}
#endif

#ifdef USE_G_MAIN_LOOP
static inline int
_gfd_events_from_fdh(Core_Fd_Handler *fdh)
{
  int events = 0;
  if (fdh->flags & CORE_FD_READ) events |= G_IO_IN | G_IO_HUP;
  if (fdh->flags & CORE_FD_WRITE) events |= G_IO_OUT | G_IO_HUP;
  if (fdh->flags & CORE_FD_ERROR) events |= G_IO_ERR | G_IO_HUP;
  return events;
}
#endif

static void
_core_main_pre_idle_exit(void)
{
  // even if we never go idle, keep up flushing some of our freeq away
  // on every idle exit which may happen if we even never called idlers
  // for now an idea but dont enforce
  eina_freeq_reduce(eina_freeq_main_get(), 128);
}

#ifdef HAVE_LIBUV
static void
_core_main_uv_poll_cb(uv_poll_t *handle, int status, int events)
{
  Eo            *obj = ML_OBJ;
  Efl_Loop_Data *pd  = ML_DAT;

  DBG("_core_main_uv_poll_cb %p status %d events %d",
      (void *)handle->data,
      status,
      events);
  Core_Fd_Handler *fdh = handle->data;

  if (_core_main_uv_idling)
  {
    DBG("not IDLE anymore");
    _core_main_uv_idling = EFL_FALSE;
    eina_file_statgen_next();
    _core_main_pre_idle_exit();
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_EXIT, NULL);
    _core_animator_run_reset();
  }

  if (status) fdh->error_active = EFL_TRUE;
  if (events & UV_READABLE) fdh->read_active = EFL_TRUE;
  if (events & UV_WRITABLE) fdh->write_active = EFL_TRUE;

  if (events & UV_DISCONNECT)
  {
    fdh->read_active  = EFL_TRUE;
    fdh->write_active = EFL_TRUE;
    fdh->error_active = EFL_TRUE;
  }
  _Core_try_add_to_call_list(obj, pd, fdh);

  _core_main_fd_handlers_call(obj, pd);
  if (pd->fd_handlers_with_buffer) _core_main_fd_handlers_buf_call(obj, pd);
  _core_signal_received_process(obj, pd);
  efl_loop_message_process(obj);
  _core_main_fd_handlers_cleanup(obj, pd);
  _efl_loop_timer_expired_timers_call(obj, pd, pd->loop_time);
}

static int
_core_main_uv_events_from_fdh(Core_Fd_Handler *fdh)
{
  int events = 0;
  if (fdh->flags & CORE_FD_READ) events |= UV_READABLE;
  if (fdh->flags & CORE_FD_WRITE) events |= UV_WRITABLE;
  DBG("events is %d", (int)events);
  return events;
}
#endif

static inline int
_core_main_fdh_poll_add(Efl_Loop_Data *pd EFL_UNUSED, Core_Fd_Handler *fdh)
{
  DBG("_core_main_fdh_poll_add");
  int r = 0;

#ifdef HAVE_SYS_EPOLL_H
#  ifdef HAVE_LIBUV
  if (!_dl_uv_run)
#  endif
  {
    if ((!fdh->file) && (pd->epoll_fd >= 0))
      r = _Core_epoll_add(_Core_get_epoll_fd(fdh->loop, pd),
                          fdh->fd,
                          _Core_poll_events_from_fdh(fdh),
                          fdh);
  }
#  ifdef HAVE_LIBUV
  else
#  endif
#endif
  {
#ifdef HAVE_LIBUV
    if (!fdh->file)
    {
      DBG("_core_main_fdh_poll_add libuv socket %p", fdh);
      fdh->uv_handle.data = fdh;
      DBG("_core_main_fdh_poll_add2 %p", fdh);
      _dl_uv_poll_init_socket(_dl_uv_default_loop(), &fdh->uv_handle, fdh->fd);
      DBG("_core_main_fdh_poll_add3 %p", fdh->uv_handle.data);
      _dl_uv_poll_start(&fdh->uv_handle,
                        _core_main_uv_events_from_fdh(fdh),
                        _core_main_uv_poll_cb);
      DBG("_core_main_fdh_poll_add libuv DONE");
    }
    else
    {
      DBG("_core_main_fdh_poll_add libuv file");
      fdh->uv_handle.data = fdh;
      DBG("_core_main_fdh_poll_add2 %p", fdh);
      _dl_uv_poll_init(_dl_uv_default_loop(), &fdh->uv_handle, fdh->fd);
      DBG("_core_main_fdh_poll_add3 %p", fdh->uv_handle.data);
      _dl_uv_poll_start(&fdh->uv_handle,
                        _core_main_uv_events_from_fdh(fdh),
                        _core_main_uv_poll_cb);
      DBG("_core_main_fdh_poll_add libuv DONE");
    }
#elif defined(USE_G_MAIN_LOOP)
    fdh->gfd.fd      = fdh->fd;
    fdh->gfd.events  = _gfd_events_from_fdh(fdh);
    fdh->gfd.revents = 0;
    DBG("adding gpoll on %d %08x", fdh->fd, fdh->gfd.events);
    if (Core_glib_source) g_source_add_poll(Core_glib_source, &fdh->gfd);
    else
      ERR("Failed to add gpoll on %d %08x. GSource is NULL",
          fdh->fd,
          fdh->gfd.events);
#endif
  }
  return r;
}

static inline void
_core_main_fdh_poll_del(Efl_Loop_Data *pd, Core_Fd_Handler *fdh)
{
#ifdef HAVE_SYS_EPOLL_H
#  ifdef HAVE_LIBUV
  if (!_dl_uv_run)
#  endif
  {
    if (!pd)
    {
      WRN("Efl_Loop_Data is NULL!");
      return;
    }

    if ((!fdh->file) && (pd->epoll_fd >= 0))
    {
      struct epoll_event ev;
      int                efd = _Core_get_epoll_fd(fdh->loop, pd);

      memset(&ev, 0, sizeof(ev));
      DBG("removing poll on %d", fdh->fd);
      // could get an EBADF if somebody closed the FD before removing
      if ((epoll_ctl(efd, EPOLL_CTL_DEL, fdh->fd, &ev) < 0))
      {
        if (errno == EBADF)
        {
          WRN("fd %d closed, can't remove from epoll - reinit!", fdh->fd);
          _core_main_loop_clear(fdh->loop, pd);
          _core_main_loop_setup(fdh->loop, pd);
        }
        else ERR("Failed to delete epoll fd %d! (errno=%d)", fdh->fd, errno);
      }
    }
  }
#  ifdef HAVE_LIBUV
  else
#  endif
#endif
  {
#ifdef HAVE_LIBUV
    DBG("_core_main_fdh_poll_del libuv %p", fdh);
    uv_handle_t *h = (uv_handle_t *)&fdh->uv_handle;
    _dl_uv_close(h, 0);
    DBG("_core_main_fdh_poll_del libuv DONE");
#endif
#ifdef USE_G_MAIN_LOOP
    fdh->gfd.fd      = fdh->fd;
    fdh->gfd.events  = _gfd_events_from_fdh(fdh);
    fdh->gfd.revents = 0;
    DBG("removing gpoll on %d %08x", fdh->fd, fdh->gfd.events);
    g_source_remove_poll(Core_glib_source, &fdh->gfd);
#endif
  }
}

static inline int
_core_main_fdh_poll_modify(Efl_Loop_Data *pd EFL_UNUSED, Core_Fd_Handler *fdh)
{
  DBG("_core_main_fdh_poll_modify %p", fdh);
  int r = 0;
#ifdef HAVE_SYS_EPOLL_H
#  ifdef HAVE_LIBUV
  if (!_dl_uv_run)
#  endif
  {
    if ((!fdh->file) && (pd->epoll_fd >= 0))
    {
      struct epoll_event ev;
      int                efd = _Core_get_epoll_fd(fdh->loop, pd);

      memset(&ev, 0, sizeof(ev));
      ev.events   = _Core_poll_events_from_fdh(fdh);
      ev.data.ptr = fdh;
      DBG("modifing epoll on %d to %08x", fdh->fd, ev.events);
      r = epoll_ctl(efd, EPOLL_CTL_MOD, fdh->fd, &ev);
    }
  }
#  ifdef HAVE_LIBUV
  else
#  endif
#endif
  {
#ifdef HAVE_LIBUV
    _dl_uv_poll_start(&fdh->uv_handle,
                      _core_main_uv_events_from_fdh(fdh),
                      _core_main_uv_poll_cb);
#elif defined(USE_G_MAIN_LOOP)
    fdh->gfd.fd      = fdh->fd;
    fdh->gfd.events  = _gfd_events_from_fdh(fdh);
    fdh->gfd.revents = 0;
    DBG("modifing gpoll on %d to %08x", fdh->fd, fdh->gfd.events);
#endif
  }
  return r;
}

static Efl_Bool
_core_main_idlers_exist(Efl_Loop_Data *pd)
{
  return pd->idlers || eina_freeq_ptr_pending(eina_freeq_main_get());
}

static void
_core_main_idler_all_call(Eo *loop, Efl_Loop_Data *pd)
{
  if (pd->idlers) efl_event_callback_call(loop, EFL_LOOP_EVENT_IDLE, NULL);
  eina_freeq_reduce(eina_freeq_main_get(), 256);
}

#ifdef HAVE_SYS_EPOLL_H
static inline int
_core_main_fdh_epoll_mark_active(Eo *obj, Efl_Loop_Data *pd)
{
  DBG("_core_main_fdh_epoll_mark_active");
  struct epoll_event ev[32];
  int                i, ret;
  int                efd = _Core_get_epoll_fd(obj, pd);

  memset(&ev, 0, sizeof(ev));
  ret = epoll_wait(efd, ev, sizeof(ev) / sizeof(struct epoll_event), 0);
  if (ret < 0)
  {
    if (errno == EINTR) return -1;
    ERR("epoll_wait failed on fd: %d %s", efd, strerror(errno));
    return -1;
  }

  for (i = 0; i < ret; i++)
  {
    Core_Fd_Handler *fdh;

    fdh = ev[i].data.ptr;
    if (!CORE_MAGIC_CHECK(fdh, CORE_MAGIC_FD_HANDLER))
    {
      CORE_MAGIC_FAIL(fdh,
                      CORE_MAGIC_FD_HANDLER,
                      "_core_main_fdh_epoll_mark_active");
      continue;
    }
    if (fdh->delete_me)
    {
      ERR("deleted fd in epoll");
      continue;
    }

    if (ev[i].events & EPOLLIN) fdh->read_active = EFL_TRUE;
    if (ev[i].events & EPOLLOUT) fdh->write_active = EFL_TRUE;
    if (ev[i].events & EPOLLERR) fdh->error_active = EFL_TRUE;

    if (ev[i].events & EPOLLHUP)
    {
      fdh->read_active  = EFL_TRUE;
      fdh->write_active = EFL_TRUE;
      fdh->error_active = EFL_TRUE;
    }
    /* We'll add this one anyway outside this function,
           don't want it twice */
    if (fdh->flags & CORE_FD_ALWAYS) continue;

    _Core_try_add_to_call_list(obj, pd, fdh);
  }
  return ret;
}
#endif

#ifdef USE_G_MAIN_LOOP
static inline int
_core_main_fdh_glib_mark_active(Eo *obj, Efl_Loop_Data *pd)
{
  Core_Fd_Handler *fdh;
  int              ret = 0;

  // call the prepare callback for all handlers
  EINA_INLIST_FOREACH(pd->fd_handlers, fdh)
  {
    if (fdh->delete_me) continue;

    if (fdh->gfd.revents & G_IO_IN) fdh->read_active = EFL_TRUE;
    if (fdh->gfd.revents & G_IO_OUT) fdh->write_active = EFL_TRUE;
    if (fdh->gfd.revents & G_IO_ERR) fdh->error_active = EFL_TRUE;

    if (fdh->gfd.revents & G_IO_HUP)
    {
      fdh->read_active  = EFL_TRUE;
      fdh->write_active = EFL_TRUE;
      fdh->error_active = EFL_TRUE;
    }

    _Core_try_add_to_call_list(obj, pd, fdh);

    if (fdh->gfd.revents & (G_IO_IN | G_IO_OUT | G_IO_ERR)) ret++;
  }

  return ret;
}

// like we are about to enter main_loop_select in  _core_main_select
static gboolean
_core_main_gsource_prepare(GSource *source EFL_UNUSED, gint *next_time)
{
  Eo            *obj   = ML_OBJ;
  Efl_Loop_Data *pd    = ML_DAT;
  gboolean       ready = FALSE;

  in_main_loop++;
  pd->in_loop = in_main_loop;

  if ((!Core_idling) && (!_Core_glib_idle_enterer_called))
  {
    _update_loop_time(pd);
    _efl_loop_timer_expired_timers_call(obj, pd, pd->loop_time);

    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_ENTER, NULL);
    _core_throttle();
    _throttle_do(pd);
    _Core_glib_idle_enterer_called = FALSE;

    if (pd->fd_handlers_with_buffer) _core_main_fd_handlers_buf_call(obj, pd);
  }

  _core_signal_received_process(obj, pd);

  // don't check fds if somebody quit
  if (g_main_loop_is_running(core_main_loop))
  {
    // only set idling state in dispatch
    if (Core_idling && (!_core_main_idlers_exist(pd)) && (!pd->message_queue))
    {
      if (_efl_loop_timers_exists(obj, pd))
      {
        int    r = -1;
        double t = _efl_loop_timer_next_get(obj, pd);

        if ((pd->timer_fd >= 0) && (t > 0.0))
        {
          struct itimerspec ts;

          ts.it_interval.tv_sec  = 0;
          ts.it_interval.tv_nsec = 0;
          ts.it_value.tv_sec     = t;
          ts.it_value.tv_nsec    = fmod(t * NS_PER_SEC, NS_PER_SEC);

          // timerfd cannot sleep for 0 time
          if (ts.it_value.tv_sec || ts.it_value.tv_nsec)
          {
            r = timerfd_settime(pd->timer_fd, 0, &ts, NULL);
            if (r < 0)
            {
              ERR("timer set returned %d (errno=%d)", r, errno);
              close(pd->timer_fd);
              pd->timer_fd = -1;
            }
            else
              INF("sleeping for %ld s %06ldus",
                  ts.it_value.tv_sec,
                  ts.it_value.tv_nsec / 1000);
          }
        }
        if (r == -1)
        {
          *next_time = ceil(t * 1000.0);
          if (t == 0.0) ready = TRUE;
        }
      }
      else *next_time = -1;
    }
    else
    {
      *next_time = 0;
      if (pd->message_queue) ready = TRUE;
    }

    if (pd->fd_handlers_with_prep) _core_main_prepare_handlers(obj, pd);
  }
  else ready = TRUE;

  in_main_loop--;
  pd->in_loop = in_main_loop;
  DBG("leave, timeout = %d", *next_time);

  // ready if we're not running (about to quit)
  return ready;
}

static gboolean
_core_main_gsource_check(GSource *source EFL_UNUSED)
{
  Eo            *obj = ML_OBJ;
  Efl_Loop_Data *pd  = ML_DAT;
  gboolean       ret = FALSE;

  in_main_loop++;
  pd->in_loop = in_main_loop;
  // check if old timers expired
  if (Core_idling && (!_core_main_idlers_exist(pd)) && (!pd->message_queue))
  {
    if (pd->timer_fd >= 0)
    {
      uint64_t count = 0;
      int      r     = read(pd->timer_fd, &count, sizeof count);
      if ((r == -1) && (errno == EAGAIN))
      {
      }
      else if (r == sizeof count) ret = TRUE;
      else
      {
        // unexpected things happened... fail back to old way
        ERR("timer read returned %d (errno=%d)", r, errno);
        close(pd->timer_fd);
        pd->timer_fd = -1;
      }
    }
  }
  else ret = TRUE;

    // check if fds are ready
#  ifdef HAVE_SYS_EPOLL_H
  if (pd->epoll_fd >= 0)
    Core_fds_ready = (_core_main_fdh_epoll_mark_active(obj, pd) > 0);
  else
#  endif
    Core_fds_ready = (_core_main_fdh_glib_mark_active(obj, pd) > 0);
  _core_main_fd_handlers_cleanup(obj, pd);
  if (Core_fds_ready) ret = TRUE;

  // check timers after updating loop time
  if (!ret && _efl_loop_timers_exists(obj, pd))
    ret = (0.0 == _efl_loop_timer_next_get(obj, pd));

  in_main_loop--;
  pd->in_loop = in_main_loop;
  return ret;
}

// like we just came out of main_loop_select in  _core_main_select
static gboolean
_core_main_gsource_dispatch(GSource *source      EFL_UNUSED,
                            GSourceFunc callback EFL_UNUSED,
                            gpointer user_data   EFL_UNUSED)
{
  Eo            *obj = ML_OBJ;
  Efl_Loop_Data *pd  = ML_DAT;
  gboolean       events_ready, timers_ready, idlers_ready;
  double         next_time;

  _update_loop_time(pd);
  _efl_loop_timer_enable_new(obj, pd);
  next_time = _efl_loop_timer_next_get(obj, pd);

  events_ready = pd->message_queue ? 1 : 0;
  timers_ready = _efl_loop_timers_exists(obj, pd) && (0.0 == next_time);
  idlers_ready = _core_main_idlers_exist(pd);

  in_main_loop++;
  pd->in_loop = in_main_loop;
  DBG("enter idling=%d fds=%d events=%d timers=%d (next=%.2f) idlers=%d",
      Core_idling,
      Core_fds_ready,
      events_ready,
      timers_ready,
      next_time,
      idlers_ready);

  if (Core_idling && events_ready)
  {
    _core_animator_run_reset();
    eina_file_statgen_next();
    _core_main_pre_idle_exit();
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_EXIT, NULL);
    Core_idling = 0;
  }
  else if (!Core_idling && !events_ready) Core_idling = 1;

  if (Core_idling)
  {
    _core_main_idler_all_call(obj, pd);

    events_ready = pd->message_queue ? 1 : 0;

    if (Core_fds_ready || events_ready || timers_ready)
    {
      _core_animator_run_reset();
      eina_file_statgen_next();
      _core_main_pre_idle_exit();
      efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_EXIT, NULL);
      Core_idling = 0;
    }
  }

  // process events
  if (!Core_idling)
  {
    _core_main_fd_handlers_call(obj, pd);
    if (pd->fd_handlers_with_buffer) _core_main_fd_handlers_buf_call(obj, pd);
    _core_signal_received_process(obj, pd);
    efl_loop_message_process(obj);
    _core_main_fd_handlers_cleanup(obj, pd);

    _efl_loop_timer_expired_timers_call(obj, pd, pd->loop_time);

    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_ENTER, NULL);
    _core_throttle();
    _throttle_do(pd);
    _Core_glib_idle_enterer_called = TRUE;

    if (pd->fd_handlers_with_buffer) _core_main_fd_handlers_buf_call(obj, pd);
  }

  in_main_loop--;
  pd->in_loop = in_main_loop;

  return TRUE;  // what should be returned here?
}

static void
_core_main_gsource_finalize(GSource *source EFL_UNUSED)
{}

static GSourceFuncs Core_gsource_funcs = {
  .prepare  = _core_main_gsource_prepare,
  .check    = _core_main_gsource_check,
  .dispatch = _core_main_gsource_dispatch,
  .finalize = _core_main_gsource_finalize,
};
#endif

#ifdef HAVE_LIBUV
static inline void _core_main_loop_uv_check(uv_check_t *handle);
static void        _core_main_loop_uv_prepare(uv_prepare_t *handle);

static void
_core_main_loop_timer_run(uv_timer_t *timer EFL_UNUSED)
{
  Eo            *obj = ML_OBJ;
  Efl_Loop_Data *pd  = ML_DAT;

  if (_core_main_uv_idling)
  {
    _core_main_uv_idling = EFL_FALSE;
    eina_file_statgen_next();
    _core_main_pre_idle_exit();
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_EXIT, NULL);
    _core_animator_run_reset();
  }
  _update_loop_time(pd);
  _core_main_loop_uv_check(NULL);
  _core_main_loop_uv_prepare(NULL);
}

static inline void
_core_main_loop_uv_check(uv_check_t *handle EFL_UNUSED)
{
  Eo            *obj = ML_OBJ;
  Efl_Loop_Data *pd  = ML_DAT;

  DBG("_core_main_loop_uv_check idling? %d", (int)_core_main_uv_idling);
  in_main_loop++;
  pd->in_loop = in_main_loop;

  if (pd->do_quit) goto quit;

  do
  {
    _core_main_fd_handlers_call(obj, pd);
    if (pd->fd_handlers_with_buffer) _core_main_fd_handlers_buf_call(obj, pd);
    _core_signal_received_process(obj, pd);
    efl_loop_message_process(obj);
    _core_main_fd_handlers_cleanup(obj, pd);
    _efl_loop_timer_expired_timers_call(obj, pd, pd->loop_time);
  }
  while (pd->fd_handlers_to_call);
quit:
  in_main_loop--;
  pd->in_loop = in_main_loop;
}
#endif

static void
_core_main_loop_setup(Eo *obj, Efl_Loop_Data *pd)
{
  if (obj == ML_OBJ)
  {
#ifdef HAVE_LIBUV
    // XXX: FIXME: the below uv init should not assert but gracefully
    // fail with errors
    DBG("loading lib uv");
#  ifdef HAVE_NODEJS
    void *lib = dlopen(NULL, RTLD_LAZY);
#  else
    void *lib = dlopen("libuv.so.1", RTLD_GLOBAL | RTLD_LAZY);
#  endif

    if (lib && dlsym(lib, "uv_run"))
    {
      DBG("loaded lib uv");
      _dl_uv_run = dlsym(lib, "uv_run");
      assert(!!_dl_uv_run);
      _dl_uv_stop = dlsym(lib, "uv_stop");
      assert(!!_dl_uv_stop);
      _dl_uv_default_loop = dlsym(lib, "uv_default_loop");
      assert(!!_dl_uv_default_loop);
      _dl_uv_poll_init_socket = dlsym(lib, "uv_poll_init_socket");
      assert(!!_dl_uv_poll_init_socket);
      _dl_uv_poll_init = dlsym(lib, "uv_poll_init");
      assert(!!_dl_uv_poll_init);
      _dl_uv_poll_start = dlsym(lib, "uv_poll_start");
      assert(!!_dl_uv_poll_start);
      _dl_uv_poll_stop = dlsym(lib, "uv_poll_stop");
      assert(!!_dl_uv_poll_stop);
      _dl_uv_timer_init = dlsym(lib, "uv_timer_init");
      assert(!!_dl_uv_timer_init);
      _dl_uv_timer_start = dlsym(lib, "uv_timer_start");
      assert(!!_dl_uv_timer_start);
      _dl_uv_timer_stop = dlsym(lib, "uv_timer_stop");
      assert(!!_dl_uv_timer_stop);
      _dl_uv_prepare_init = dlsym(lib, "uv_prepare_init");
      assert(!!_dl_uv_prepare_init);
      _dl_uv_prepare_start = dlsym(lib, "uv_prepare_start");
      assert(!!_dl_uv_prepare_start);
      _dl_uv_prepare_stop = dlsym(lib, "uv_prepare_stop");
      assert(!!_dl_uv_prepare_stop);
      _dl_uv_check_init = dlsym(lib, "uv_check_init");
      assert(!!_dl_uv_check_init);
      _dl_uv_check_start = dlsym(lib, "uv_check_start");
      assert(!!_dl_uv_check_start);
      _dl_uv_check_stop = dlsym(lib, "uv_check_stop");
      assert(!!_dl_uv_check_stop);
      _dl_uv_close = dlsym(lib, "uv_close");
      assert(!!_dl_uv_close);
      _dl_uv_loop_alive = dlsym(lib, "uv_loop_alive");
      assert(!!_dl_uv_loop_alive);

      //dlclose(lib);

      DBG("_dl_uv_prepare_init");
      _dl_uv_prepare_init(_dl_uv_default_loop(), &_core_main_uv_prepare);
      DBG("_dl_uv_prepare_start");
      _dl_uv_prepare_start(&_core_main_uv_prepare, &_core_main_loop_uv_prepare);
      DBG("_dl_uv_prepare_started");

      DBG("_dl_uv_check_init");
      _dl_uv_check_init(_dl_uv_default_loop(), &_core_main_uv_check);
      DBG("_dl_uv_check_start");
      _dl_uv_check_start(&_core_main_uv_check, &_core_main_loop_uv_check);
      DBG("_dl_uv_check_started");

      _dl_uv_timer_init(_dl_uv_default_loop(), &_core_main_uv_handle_timers);
    }
    // else
    //   DBG("did not load uv");
    DBG("loaded dlsyms uv");
#endif

    // setup for the g_main_loop only integration
#ifdef USE_G_MAIN_LOOP
    Core_glib_source = g_source_new(&Core_gsource_funcs, sizeof(GSource));
    if (!Core_glib_source) CRI("Failed to create glib source for epoll!");
    else
    {
      g_source_set_priority(Core_glib_source, G_PRIORITY_HIGH_IDLE + 20);
#  ifdef HAVE_SYS_EPOLL_H
      if (pd->epoll_fd >= 0)
      {
        // epoll multiplexes fds into the g_main_loop
        Core_epoll_fd.fd      = pd->epoll_fd;
        Core_epoll_fd.events  = G_IO_IN;
        Core_epoll_fd.revents = 0;
        g_source_add_poll(Core_glib_source, &Core_epoll_fd);
      }
#  endif
      // timerfd gives us better than millisecond accuracy
      pd->timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
      if (pd->timer_fd < 0) WRN("failed to create timer fd!");
      else
      {
        eina_file_close_on_exec(pd->timer_fd, EFL_TRUE);
        core_timer_fd.fd      = pd->timer_fd;
        core_timer_fd.events  = G_IO_IN;
        core_timer_fd.revents = 0;
        g_source_add_poll(Core_glib_source, &core_timer_fd);
      }

      Core_glib_source_id = g_source_attach(Core_glib_source, NULL);
      if (Core_glib_source_id <= 0)
        CRI("Failed to attach glib source to default context");
    }
#endif
  }

  // Please note that this function is being also called in case of a bad
  // fd to reset the main loop.
#ifdef HAVE_SYS_EPOLL_H
  pd->epoll_fd = epoll_create(1);
  if (pd->epoll_fd < 0) WRN("Failed to create epoll fd!");
  else
  {
    eina_file_close_on_exec(pd->epoll_fd, EFL_TRUE);

    pd->epoll_pid = getpid();

    // add polls on all our file descriptors
    Core_Fd_Handler *fdh;
    EINA_INLIST_FOREACH(pd->fd_handlers, fdh)
    {
      if (fdh->delete_me) continue;
      _Core_epoll_add(pd->epoll_fd,
                      fdh->fd,
                      _Core_poll_events_from_fdh(fdh),
                      fdh);
      _core_main_fdh_poll_add(pd, fdh);
    }
  }
#endif

  _core_main_timechanges_start(obj);
}

static void
_core_main_loop_clear(Eo *obj, Efl_Loop_Data *pd)
{
  if (!pd) return;
  // Please note that _core_main_loop_shutdown is called in cycle to
  // restart the main loop in case of a bad fd
  if (obj == ML_OBJ)
  {
    _core_main_timechanges_stop(obj);
#ifdef USE_G_MAIN_LOOP
    if (Core_glib_source)
    {
      g_source_destroy(Core_glib_source);
      Core_glib_source = NULL;
    }
#endif
#ifdef HAVE_LIBUV
    if (_dl_uv_run)
    {
      DBG("_core_main_loop_shutdown");
      _dl_uv_timer_stop(&_core_main_uv_handle_timers);
      _dl_uv_close((uv_handle_t *)&_core_main_uv_handle_timers, 0);
    }
#endif
  }
#ifdef HAVE_SYS_EPOLL_H
  if (pd->epoll_fd >= 0)
  {
    close(pd->epoll_fd);
    pd->epoll_fd = -1;
  }
#endif
  if (pd->timer_fd >= 0)
  {
    close(pd->timer_fd);
    pd->timer_fd = -1;
  }
}

void
_core_main_loop_init(void)
{
  DBG("_core_main_loop_init");
  if (!efl_main_loop_get()) ERR("Cannot create main loop object");
  _core_main_loop_setup(ML_OBJ, ML_DAT);
}

void
_core_main_loop_shutdown(void)
{
  if (!ML_OBJ) return;
  _core_main_loop_clear(ML_OBJ, ML_DAT);
  // XXX: this seemingly closes fd's it shouldn't.... :( fd 0?
  efl_replace(&ML_OBJ, NULL);
  ML_DAT = NULL;
}

void
_core_main_loop_iterate(Eo *obj, Efl_Loop_Data *pd)
{
#ifdef HAVE_LIBUV
  if (!_dl_uv_run)
  {
#endif
#ifndef USE_G_MAIN_LOOP
    _update_loop_time(pd);
    _core_main_loop_iterate_internal(obj, pd, 1);
#else
  g_main_context_iteration(NULL, 0);
#endif
#ifdef HAVE_LIBUV
  }
  else _dl_uv_run(_dl_uv_default_loop(), UV_RUN_ONCE | UV_RUN_NOWAIT);
#endif
}

int
_core_main_loop_iterate_may_block(Eo *obj, Efl_Loop_Data *pd, int may_block)
{
  if (obj == ML_OBJ)
  {
#ifdef HAVE_LIBUV
    if (!_dl_uv_run)
    {
#endif
#ifndef USE_G_MAIN_LOOP
      in_main_loop++;
      pd->in_loop = in_main_loop;
      _update_loop_time(pd);
      _core_main_loop_iterate_internal(obj, pd, !may_block);
      in_main_loop--;
      pd->in_loop = in_main_loop;
      return pd->message_queue ? 1 : 0;
#else
    return g_main_context_iteration(NULL, may_block);
#endif
#ifdef HAVE_LIBUV
    }
    else
      _dl_uv_run(_dl_uv_default_loop(),
                 may_block ? (UV_RUN_ONCE | UV_RUN_NOWAIT) : UV_RUN_ONCE);
#endif
  }
  else
  {
#ifndef USE_G_MAIN_LOOP
    pd->in_loop++;
    _update_loop_time(pd);
    _core_main_loop_iterate_internal(obj, pd, !may_block);
    pd->in_loop--;
    return pd->message_queue ? 1 : 0;
#else
    return g_main_context_iteration(NULL, may_block);
#endif
  }
  return 0;
}

void
_core_main_loop_begin(Eo *obj, Efl_Loop_Data *pd)
{
  pd->loop_active++;
  if (obj == ML_OBJ)
  {
#ifdef HAVE_SYSTEMD
    if (getenv("NOTIFY_SOCKET"))
    {
      _core_sd_init();
      if (_core_sd_notify) _core_sd_notify(0, "READY=1");
    }
#endif
#ifdef HAVE_LIBUV
    if (!_dl_uv_run)
    {
#endif
#ifndef USE_G_MAIN_LOOP
      in_main_loop++;
      pd->in_loop = in_main_loop;
      _update_loop_time(pd);
      while (!pd->do_quit)
        _core_main_loop_iterate_internal(obj, pd, 0);
      pd->do_quit = 0;
      in_main_loop--;
      pd->in_loop = in_main_loop;
#else
    if (!pd->do_quit)
    {
      if (!core_main_loop) core_main_loop = g_main_loop_new(NULL, FALSE);
      g_main_loop_run(core_main_loop);
    }
    pd->do_quit = 0;
#endif
#ifdef HAVE_LIBUV
    }
    else
    {
      DBG("uv_run");
      in_main_loop++;
      pd->in_loop = in_main_loop;
      _update_loop_time(pd);
      while (!pd->do_quit)
        _dl_uv_run(_dl_uv_default_loop(), UV_RUN_DEFAULT);
      in_main_loop--;
      pd->in_loop = in_main_loop;
      pd->do_quit = 0;
      DBG("quit");
    }
#endif
  }
  else
  {
#ifndef USE_G_MAIN_LOOP
    pd->in_loop++;
    _update_loop_time(pd);
    while (!pd->do_quit)
      _core_main_loop_iterate_internal(obj, pd, 0);
    pd->do_quit = 0;
    pd->in_loop--;
#else
    if (!pd->do_quit)
    {
      if (!core_main_loop) core_main_loop = g_main_loop_new(NULL, 1);
      g_main_loop_run(core_main_loop);
    }
    pd->do_quit = 0;
#endif
  }
  pd->loop_active--;
}

void
_core_main_loop_quit(Eo *obj, Efl_Loop_Data *pd)
{
  if (!pd->loop_active) return;
  pd->do_quit = 1;
  if (obj != ML_OBJ) return;
#ifdef USE_G_MAIN_LOOP
  if (core_main_loop) g_main_loop_quit(core_main_loop);
#elif defined(HAVE_LIBUV)
  if (_dl_uv_run) _dl_uv_stop(_dl_uv_default_loop());
#endif
}

EAPI void
core_main_loop_iterate(void)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  efl_loop_iterate(ML_OBJ);
}

EAPI int
core_main_loop_iterate_may_block(int may_block)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  return efl_loop_iterate_may_block(ML_OBJ, may_block);
}

EAPI void
core_main_loop_begin(void)
{
  DBG("core_main_loop_begin");
  EINA_MAIN_LOOP_CHECK_RETURN;
  eina_evlog("+mainloop", NULL, 0.0, NULL);
  efl_loop_begin(ML_OBJ);
  eina_evlog("-mainloop", NULL, 0.0, NULL);
}

EAPI void
core_main_loop_quit(void)
{
  Eina_Value v = EINA_VALUE_EMPTY;

  eina_value_setup(&v, EINA_VALUE_TYPE_INT);
  eina_value_set(&v, 0);
  EINA_MAIN_LOOP_CHECK_RETURN;
  efl_loop_quit(ML_OBJ, v);
}

EAPI int
core_main_loop_nested_get(void)
{
  return in_main_loop;
}

EAPI Efl_Bool
core_main_loop_animator_ticked_get(void)
{
  DBG("core_main_loop_animator_ticked_get");
  return _core_animator_run_get();
}

EAPI void
core_main_loop_select_func_set(Core_Select_Function func)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  main_loop_select = func;
}

EAPI Core_Select_Function
core_main_loop_select_func_get(void)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  return main_loop_select;
}

Core_Fd_Handler *
_core_main_fd_handler_add(Eo                   *obj,
                          Efl_Loop_Data        *pd,
                          Eo                   *handler,
                          int                   fd,
                          Core_Fd_Handler_Flags flags,
                          Core_Fd_Cb            func,
                          const void           *data,
                          Core_Fd_Cb            buf_func,
                          const void           *buf_data,
                          Efl_Bool              is_file)
{
  DBG("_core_main_fd_handler_add");
  Core_Fd_Handler *fdh = NULL;

  if ((fd < 0) || (flags == 0) || (!func)) return NULL;

  fdh = core_fd_handler_calloc(1);
  if (!fdh) return NULL;
  CORE_MAGIC_SET(fdh, CORE_MAGIC_FD_HANDLER);
  fdh->loop      = obj;
  fdh->loop_data = pd;
  fdh->handler   = handler;
  fdh->fd        = fd;
  fdh->flags     = flags;
  fdh->file      = is_file;
  if (_core_main_fdh_poll_add(pd, fdh) < 0)
  {
    int err = errno;
    ERR("Failed to add poll on fd %d (errno = %d: %s)!",
        fd,
        err,
        strerror(err));
    core_fd_handler_mp_free(fdh);
    return NULL;
  }
  fdh->func     = func;
  fdh->data     = (void *)data;
  fdh->buf_func = buf_func;
  if (buf_func)
    pd->fd_handlers_with_buffer =
      eina_list_append(pd->fd_handlers_with_buffer, fdh);
  fdh->buf_data = (void *)buf_data;
  if (is_file)
    pd->file_fd_handlers = eina_list_append(pd->file_fd_handlers, fdh);
  if (fdh->flags & CORE_FD_ALWAYS)
    pd->always_fd_handlers = eina_list_append(pd->always_fd_handlers, fdh);
  pd->fd_handlers =
    (Core_Fd_Handler *)eina_inlist_append(EINA_INLIST_GET(pd->fd_handlers),
                                          EINA_INLIST_GET(fdh));
  return fdh;
}

void *
_core_main_fd_handler_del(Eo *obj          EFL_UNUSED,
                          Efl_Loop_Data   *pd,
                          Core_Fd_Handler *fd_handler)
{
  void *r = fd_handler->data;

  DBG("_core_main_fd_handler_del %p", fd_handler);
  if (fd_handler->delete_me)
  {
    ERR("fdh %p deleted twice", fd_handler);
    return NULL;
  }

  fd_handler->handler   = NULL;
  fd_handler->delete_me = EFL_TRUE;
  if (pd)
  {
    _core_main_fdh_poll_del(pd, fd_handler);
    pd->fd_handlers_to_delete =
      eina_list_append(pd->fd_handlers_to_delete, fd_handler);
    if (fd_handler->prep_func && pd->fd_handlers_with_prep)
      pd->fd_handlers_with_prep =
        eina_list_remove(pd->fd_handlers_with_prep, fd_handler);
    if (fd_handler->buf_func && pd->fd_handlers_with_buffer)
      pd->fd_handlers_with_buffer =
        eina_list_remove(pd->fd_handlers_with_buffer, fd_handler);
  }
  else
  {
    // The main loop is dead by now, so cleanup is required.
    CORE_MAGIC_SET(fd_handler, CORE_MAGIC_NONE);
    core_fd_handler_mp_free(fd_handler);
  }
  return r;
}

EAPI Core_Fd_Handler *
core_main_fd_handler_add(int                   fd,
                         Core_Fd_Handler_Flags flags,
                         Core_Fd_Cb            func,
                         const void           *data,
                         Core_Fd_Cb            buf_func,
                         const void           *buf_data)
{
  Core_Fd_Handler *fdh = NULL;
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  fdh = _core_main_fd_handler_add(efl_main_loop_get(),
                                  ML_DAT,
                                  NULL,
                                  fd,
                                  flags,
                                  func,
                                  data,
                                  buf_func,
                                  buf_data,
                                  EFL_FALSE);
  if (fdh) fdh->legacy = EFL_TRUE;
  return fdh;
}

EAPI Core_Fd_Handler *
core_main_fd_handler_file_add(int                   fd,
                              Core_Fd_Handler_Flags flags,
                              Core_Fd_Cb            func,
                              const void           *data,
                              Core_Fd_Cb            buf_func,
                              const void           *buf_data)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  return _core_main_fd_handler_add(efl_main_loop_get(),
                                   ML_DAT,
                                   NULL,
                                   fd,
                                   flags,
                                   func,
                                   data,
                                   buf_func,
                                   buf_data,
                                   EFL_TRUE);
}

EAPI void *
core_main_fd_handler_del(Core_Fd_Handler *fd_handler)
{
  if (!fd_handler) return NULL;
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

  if (!CORE_MAGIC_CHECK(fd_handler, CORE_MAGIC_FD_HANDLER))
  {
    CORE_MAGIC_FAIL(fd_handler,
                    CORE_MAGIC_FD_HANDLER,
                    "core_main_fd_handler_del");
    return NULL;
  }
  return _core_main_fd_handler_del(ML_OBJ, ML_DAT, fd_handler);
}

#ifdef _WIN32
EAPI Core_Win32_Handler *
_core_main_win32_handler_add(Eo                  *obj,
                             Efl_Loop_Data       *pd,
                             Eo                  *handler,
                             void                *h,
                             Core_Win32_Handle_Cb func,
                             const void          *data)
{
  Core_Win32_Handler *wh;

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  if (!h || !func) return NULL;

  wh = core_win32_handler_calloc(1);
  if (!wh) return NULL;
  CORE_MAGIC_SET(wh, CORE_MAGIC_WIN32_HANDLER);
  wh->loop           = obj;
  wh->loop_data      = pd;
  wh->handler        = handler;
  wh->h              = (HANDLE)h;
  wh->func           = func;
  wh->data           = (void *)data;
  pd->win32_handlers = (Core_Win32_Handler *)eina_inlist_append(
    EINA_INLIST_GET(pd->win32_handlers),
    EINA_INLIST_GET(wh));
  return wh;
}

void *
_core_main_win32_handler_del(Eo *obj             EFL_UNUSED,
                             Efl_Loop_Data      *pd,
                             Core_Win32_Handler *win32_handler)
{
  if (win32_handler->delete_me)
  {
    ERR("win32 handler %p deleted twice", win32_handler);
    return NULL;
  }

  win32_handler->delete_me = EFL_TRUE;
  win32_handler->handler   = NULL;
  pd->win32_handlers_to_delete =
    eina_list_append(pd->win32_handlers_to_delete, win32_handler);
  return win32_handler->data;
}

EAPI Core_Win32_Handler *
core_main_win32_handler_add(void                *h,
                            Core_Win32_Handle_Cb func,
                            const void          *data)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  return _core_main_win32_handler_add(ML_OBJ, ML_DAT, NULL, h, func, data);
}

EAPI void *
core_main_win32_handler_del(Core_Win32_Handler *win32_handler)
{
  void *ret = NULL;

  if (!win32_handler) return NULL;
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  if (!CORE_MAGIC_CHECK(win32_handler, CORE_MAGIC_WIN32_HANDLER))
  {
    CORE_MAGIC_FAIL(win32_handler,
                    CORE_MAGIC_WIN32_HANDLER,
                    "core_main_win32_handler_del");
    return NULL;
  }
  ret = _core_main_win32_handler_del(ML_OBJ, ML_DAT, win32_handler);
  return ret;
}
#else
EAPI Core_Win32_Handler *
_core_main_win32_handler_add(Eo *obj                   EFL_UNUSED,
                             Efl_Loop_Data *pd         EFL_UNUSED,
                             Eo *handler               EFL_UNUSED,
                             void *h                   EFL_UNUSED,
                             Core_Win32_Handle_Cb func EFL_UNUSED,
                             const void *data          EFL_UNUSED)
{
  return NULL;
}

void *
_core_main_win32_handler_del(Eo *obj                           EFL_UNUSED,
                             Efl_Loop_Data *pd                 EFL_UNUSED,
                             Core_Win32_Handler *win32_handler EFL_UNUSED)
{
  return NULL;
}

EAPI Core_Win32_Handler *
core_main_win32_handler_add(void *h                   EFL_UNUSED,
                            Core_Win32_Handle_Cb func EFL_UNUSED,
                            const void *data          EFL_UNUSED)
{
  return NULL;
}

EAPI void *
core_main_win32_handler_del(Core_Win32_Handler *win32_handler EFL_UNUSED)
{
  return NULL;
}
#endif

EAPI void
core_main_fd_handler_prepare_callback_set(Core_Fd_Handler *fd_handler,
                                          Core_Fd_Prep_Cb  func,
                                          const void      *data)
{
  if (!fd_handler) return;
  Efl_Loop_Data *pd = fd_handler->loop_data;
  EINA_MAIN_LOOP_CHECK_RETURN;

  if (!CORE_MAGIC_CHECK(fd_handler, CORE_MAGIC_FD_HANDLER))
  {
    CORE_MAGIC_FAIL(fd_handler,
                    CORE_MAGIC_FD_HANDLER,
                    "core_main_fd_handler_prepare_callback_set");
    return;
  }
  fd_handler->prep_func = func;
  fd_handler->prep_data = (void *)data;
  if ((!pd->fd_handlers_with_prep) ||
      (pd->fd_handlers_with_prep &&
       (!eina_list_data_find(pd->fd_handlers_with_prep, fd_handler))))
    // FIXME: THIS WILL NOT SCALE WITH LOTS OF PREP FUNCTIONS!!!
    pd->fd_handlers_with_prep =
      eina_list_append(pd->fd_handlers_with_prep, fd_handler);
}

EAPI int
core_main_fd_handler_fd_get(Core_Fd_Handler *fd_handler)
{
  if (!fd_handler) return -1;

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(-1);

  if (!CORE_MAGIC_CHECK(fd_handler, CORE_MAGIC_FD_HANDLER))
  {
    CORE_MAGIC_FAIL(fd_handler,
                    CORE_MAGIC_FD_HANDLER,
                    "core_main_fd_handler_fd_get");
    return -1;
  }
  return fd_handler->fd;
}

EAPI Efl_Bool
core_main_fd_handler_active_get(Core_Fd_Handler      *fd_handler,
                                Core_Fd_Handler_Flags flags)
{
  int ret = EFL_FALSE;

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(EFL_FALSE);

  if (!CORE_MAGIC_CHECK(fd_handler, CORE_MAGIC_FD_HANDLER))
  {
    CORE_MAGIC_FAIL(fd_handler,
                    CORE_MAGIC_FD_HANDLER,
                    "core_main_fd_handler_active_get");
    return EFL_FALSE;
  }
  if ((flags & CORE_FD_READ) && (fd_handler->read_active)) ret = EFL_TRUE;
  if ((flags & CORE_FD_WRITE) && (fd_handler->write_active)) ret = EFL_TRUE;
  if ((flags & CORE_FD_ERROR) && (fd_handler->error_active)) ret = EFL_TRUE;
  return ret;
}

EAPI void
core_main_fd_handler_active_set(Core_Fd_Handler      *fd_handler,
                                Core_Fd_Handler_Flags flags)
{
  int ret = -1;

  if (!CORE_MAGIC_CHECK(fd_handler, CORE_MAGIC_FD_HANDLER))
  {
    CORE_MAGIC_FAIL(fd_handler,
                    CORE_MAGIC_FD_HANDLER,
                    "core_main_fd_handler_active_set");
    return;
  }
  fd_handler->flags = flags;
  if (fd_handler->loop_data)
    ret = _core_main_fdh_poll_modify(fd_handler->loop_data, fd_handler);
  if (ret < 0)
    ERR("Failed to mod epoll fd %d, loop data=%p: %s!",
        fd_handler->fd,
        fd_handler->loop_data,
        strerror(errno));
}

void
_core_main_content_clear(Eo *obj, Efl_Loop_Data *pd)
{
  __eina_promise_cancel_data(obj);

  while (pd->fd_handlers)
  {
    Core_Fd_Handler *fdh = pd->fd_handlers;

    pd->fd_handlers =
      (Core_Fd_Handler *)eina_inlist_remove(EINA_INLIST_GET(pd->fd_handlers),
                                            EINA_INLIST_GET(fdh));
    if ((fdh->handler) && (fdh->legacy)) efl_del(fdh->handler);
    else
    {
      // XXX: can't do this because this fd handler is legacy and might
      // be cleaned up later in object destructors
      //             CORE_MAGIC_SET(fdh, CORE_MAGIC_NONE);
      //             core_fd_handler_mp_free(fdh);
    }
  }
  if (pd->fd_handlers_with_buffer)
    pd->fd_handlers_with_buffer = eina_list_free(pd->fd_handlers_with_buffer);
  if (pd->fd_handlers_with_prep)
    pd->fd_handlers_with_prep = eina_list_free(pd->fd_handlers_with_prep);
  if (pd->file_fd_handlers)
    pd->file_fd_handlers = eina_list_free(pd->file_fd_handlers);
  if (pd->always_fd_handlers)
    pd->always_fd_handlers = eina_list_free(pd->always_fd_handlers);
  if (pd->fd_handlers_to_delete)
    pd->fd_handlers_to_delete = eina_list_free(pd->fd_handlers_to_delete);
  pd->fd_handlers_to_call         = NULL;
  pd->fd_handlers_to_call_current = NULL;

  pd->do_quit = 0;

#ifdef _WIN32
  while (pd->win32_handlers)
  {
    Core_Win32_Handler *wh = pd->win32_handlers;

    pd->win32_handlers = (Core_Win32_Handler *)eina_inlist_remove(
      EINA_INLIST_GET(pd->win32_handlers),
      EINA_INLIST_GET(wh));
    if (wh->handler) efl_del(wh->handler);
    else
    {
      CORE_MAGIC_SET(wh, CORE_MAGIC_NONE);
      core_win32_handler_mp_free(wh);
    }
  }
  if (pd->win32_handlers_to_delete)
    pd->win32_handlers_to_delete = eina_list_free(pd->win32_handlers_to_delete);
  pd->win32_handler_current = NULL;
#endif
}

void
_core_main_shutdown(void)
{
  Efl_Loop_Data *pd = ML_DAT;

  if (pd->in_loop)
  {
    ERR("Calling Core_shutdown() while still in the main loop!!!");
    return;
  }
}

static void
_core_main_prepare_handlers(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd)
{
  Core_Fd_Handler *fdh;
  Eina_List       *l, *l2;

  // call the prepare callback for all handlers with prep functions
  EINA_LIST_FOREACH_SAFE(pd->fd_handlers_with_prep, l, l2, fdh)
  {
    if (!fdh)
    {
      pd->fd_handlers_with_prep =
        eina_list_remove_list(l, pd->fd_handlers_with_prep);
      continue;
    }
    if (!fdh->delete_me && fdh->prep_func)
    {
      fdh->references++;
      _core_call_prep_cb(fdh->prep_func, fdh->prep_data, fdh);
      fdh->references--;
    }
    else
      pd->fd_handlers_with_prep =
        eina_list_remove_list(pd->fd_handlers_with_prep, l);
  }
}

static void
_core_main_select_fd_too_big_check(int fd)
{
  if (fd < FD_SETSIZE) return;
  int newsize = fd + 1 - 1;
  newsize    |= newsize >> 1;
  newsize    |= newsize >> 2;
  newsize    |= newsize >> 4;
  newsize    |= newsize >> 8;
  newsize    |= newsize >> 16;
  newsize++;
  fprintf(stderr,
          "ERROR: ecore main loop, fd %i >= max possible fd %i.   "
          "If this continues to be an issue with lots of high value fds "
          "being needed, perhaps consider recompiling EFL with the option:   "
          "-Dmax-fd-size=%i\n",
          fd,
          FD_SETSIZE,
          newsize);
}

#if !defined(USE_G_MAIN_LOOP)
static int
_core_main_select(Eo *obj, Efl_Loop_Data *pd, double timeout)
{
  struct timeval   tv, *t;
  fd_set           rfds, wfds, exfds;
  Core_Fd_Handler *fdh;
  Eina_List       *l;
  int              max_fd, ret, outval;
#  ifndef _WIN32
  int err_no;
#  endif

  t = NULL;
  if ((!Core_FINITE(timeout)) || (EINA_DBL_EQ(timeout, 0.0)))
  {  // finite() tests for NaN, too big, too small, and infinity.
    tv.tv_sec  = 0;
    tv.tv_usec = 0;
    t          = &tv;
  }
  else if (timeout > 0.0)
  {
    int sec, usec;

#  ifdef FIX_HZ
    timeout += (0.5 / HZ);
    sec      = (int)timeout;
    usec     = (int)((timeout - (double)sec) * 1000000);
#  else
    sec  = (int)timeout;
    usec = (int)((timeout - (double)sec) * 1000000);
#  endif
    tv.tv_sec  = sec;
    tv.tv_usec = usec;
    t          = &tv;
  }
  max_fd = 0;
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);
  FD_ZERO(&exfds);

  // call the prepare callback for all handlers
  if (pd->fd_handlers_with_prep) _core_main_prepare_handlers(obj, pd);

#  ifdef HAVE_SYS_EPOLL_H
  if (pd->epoll_fd < 0)
  {
#  endif
    EINA_INLIST_FOREACH(pd->fd_handlers, fdh)
    {
      if (!fdh->delete_me)
      {
        _core_main_select_fd_too_big_check(fdh->fd);
        if ((fdh->flags & CORE_FD_READ) || (fdh->flags & CORE_FD_ALWAYS))
        {
          FD_SET(fdh->fd, &rfds);
          if (fdh->fd > max_fd) max_fd = fdh->fd;
        }
        if ((fdh->flags & CORE_FD_WRITE) || (fdh->flags & CORE_FD_ALWAYS))
        {
          FD_SET(fdh->fd, &wfds);
          if (fdh->fd > max_fd) max_fd = fdh->fd;
        }
        if ((fdh->flags & CORE_FD_ERROR) || (fdh->flags & CORE_FD_ALWAYS))
        {
          FD_SET(fdh->fd, &exfds);
          if (fdh->fd > max_fd) max_fd = fdh->fd;
        }
      }
    }
#  ifdef HAVE_SYS_EPOLL_H
  }
  else
  {
    // polling on the epoll fd will wake when fd in the epoll set is active
    max_fd = _Core_get_epoll_fd(obj, pd);
    if (max_fd > -1) FD_SET(max_fd, &rfds);
  }
#  endif
  EINA_LIST_FOREACH(pd->file_fd_handlers, l, fdh)
  {
    if (!fdh->delete_me)
    {
      _core_main_select_fd_too_big_check(fdh->fd);
      if ((fdh->flags & CORE_FD_READ) || (fdh->flags & CORE_FD_ALWAYS))
      {
        FD_SET(fdh->fd, &rfds);
        if (fdh->fd > max_fd) max_fd = fdh->fd;
      }
      if ((fdh->flags & CORE_FD_WRITE) || (fdh->flags & CORE_FD_ALWAYS))
      {
        FD_SET(fdh->fd, &wfds);
        if (fdh->fd > max_fd) max_fd = fdh->fd;
      }
      if ((fdh->flags & CORE_FD_ERROR) || (fdh->flags & CORE_FD_ALWAYS))
      {
        FD_SET(fdh->fd, &exfds);
        if (fdh->fd > max_fd) max_fd = fdh->fd;
      }
      if (fdh->fd > max_fd) max_fd = fdh->fd;
    }
  }
  if (_core_signal_count_get(obj, pd)) return -1;

  eina_evlog("<RUN", NULL, 0.0, NULL);
  eina_evlog("!SLEEP", NULL, 0.0, t ? "timeout" : "forever");
  if (obj == ML_OBJ)
    ret = main_loop_select(max_fd + 1, &rfds, &wfds, &exfds, t);
  else ret = general_loop_select(max_fd + 1, &rfds, &wfds, &exfds, t);
#  ifndef _WIN32
  err_no = errno;
#  endif
  eina_evlog("!WAKE", NULL, 0.0, NULL);
  eina_evlog(">RUN", NULL, 0.0, NULL);

  _update_loop_time(pd);
  if (ret < 0)
  {
#  ifndef _WIN32
    if (err_no == EINTR)
    {
      outval = -1;
      goto BAIL;
    }
    else if (err_no == EBADF) _core_main_fd_handlers_bads_rem(obj, pd);
#  endif
  }
  if (ret > 0)
  {
#  ifdef HAVE_SYS_EPOLL_H
    if (pd->epoll_fd >= 0) _core_main_fdh_epoll_mark_active(obj, pd);
    else
#  endif
    {
      EINA_INLIST_FOREACH(pd->fd_handlers, fdh)
      {
        if (!fdh->delete_me)
        {
          if (FD_ISSET(fdh->fd, &rfds)) fdh->read_active = EFL_TRUE;
          if (FD_ISSET(fdh->fd, &wfds)) fdh->write_active = EFL_TRUE;
          if (FD_ISSET(fdh->fd, &exfds)) fdh->error_active = EFL_TRUE;
          _Core_try_add_to_call_list(obj, pd, fdh);
        }
      }
    }
    EINA_LIST_FOREACH(pd->file_fd_handlers, l, fdh)
    {
      if (!fdh->delete_me)
      {
        if (FD_ISSET(fdh->fd, &rfds)) fdh->read_active = EFL_TRUE;
        if (FD_ISSET(fdh->fd, &wfds)) fdh->write_active = EFL_TRUE;
        if (FD_ISSET(fdh->fd, &exfds)) fdh->error_active = EFL_TRUE;
        _Core_try_add_to_call_list(obj, pd, fdh);
      }
    }
    outval = 1;
    goto BAIL;
  }
  outval = 0;
BAIL:
  EINA_LIST_FOREACH(pd->always_fd_handlers, l, fdh)
    _Core_try_add_to_call_list(obj, pd, fdh);

  if (ret > 0)
  {
    _core_main_fd_handlers_cleanup(obj, pd);
#  ifdef _WIN32
    _core_main_win32_handlers_cleanup(obj, pd);
#  endif
  }
  return outval || pd->always_fd_handlers;
}

#endif

#ifndef _WIN32
#  ifndef USE_G_MAIN_LOOP
static void
_core_main_fd_handlers_bads_rem(Eo *obj, Efl_Loop_Data *pd)
{
  Core_Fd_Handler *fdh;
  Eina_Inlist     *l;
  int              found = 0;

  ERR("Removing bad fds");
  for (l = EINA_INLIST_GET(pd->fd_handlers); l;)
  {
    fdh   = (Core_Fd_Handler *)l;
    l     = l->next;
    errno = 0;

    if ((fcntl(fdh->fd, F_GETFD) < 0) && (errno == EBADF))
    {
      ERR("Found bad fd at index %d", fdh->fd);
      if (fdh->flags & CORE_FD_ERROR)
      {
        ERR("Fd set for error! calling user");
        fdh->references++;
        if (!_core_call_fd_cb(fdh->func, fdh->data, fdh))
        {
          ERR("Fd function err returned 0, remove it");
          if (!fdh->delete_me)
          {
            fdh->delete_me = EFL_TRUE;
            _core_main_fdh_poll_del(pd, fdh);
            pd->fd_handlers_to_delete =
              eina_list_append(pd->fd_handlers_to_delete, fdh);
          }
          found++;
        }
        fdh->references--;
      }
      else
      {
        ERR("Problematic fd found at %d! setting it for delete", fdh->fd);
        if (!fdh->delete_me)
        {
          fdh->delete_me = EFL_TRUE;
          _core_main_fdh_poll_del(pd, fdh);
          pd->fd_handlers_to_delete =
            eina_list_append(pd->fd_handlers_to_delete, fdh);
        }

        found++;
      }
    }
  }
  if (found == 0)
  {
#    ifdef HAVE_GLIB
    ERR("No bad fd found. Maybe a foreign fd from glib?");
#    else
    ERR("No bad fd found. EEEK!");
#    endif
  }
  _core_main_fd_handlers_cleanup(obj, pd);
}

#  endif
#endif

static void
_core_main_fd_handlers_cleanup(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd)
{
  Core_Fd_Handler *fdh, *last;
  Eina_List       *l, *l2;

  // Cleanup deleted caller from the list
  last = NULL;
  fdh  = pd->fd_handlers_to_call;
  while (fdh)
  {
    if (fdh->delete_me)
    {
      if (!last)
        pd->fd_handlers_to_call =
          fdh == fdh->next_ready ? NULL : fdh->next_ready;
      else last->next_ready = fdh == fdh->next_ready ? last : fdh->next_ready;
    }
    else last = fdh;

    if (fdh == fdh->next_ready) break;
    fdh = fdh->next_ready;
  }

  if (!pd->fd_handlers_to_delete) return;
  EINA_LIST_FOREACH_SAFE(pd->fd_handlers_to_delete, l, l2, fdh)
  {
    if (!fdh)
    {
      pd->fd_handlers_to_delete =
        eina_list_remove_list(l, pd->fd_handlers_to_delete);
      continue;
    }
    if (fdh->references) continue;
    if (pd->fd_handlers_to_call_current == fdh)
      pd->fd_handlers_to_call_current = NULL;
    if (fdh->buf_func && pd->fd_handlers_with_buffer)
      pd->fd_handlers_with_buffer =
        eina_list_remove(pd->fd_handlers_with_buffer, fdh);
    if (fdh->prep_func && pd->fd_handlers_with_prep)
      pd->fd_handlers_with_prep =
        eina_list_remove(pd->fd_handlers_with_prep, fdh);
    pd->fd_handlers =
      (Core_Fd_Handler *)eina_inlist_remove(EINA_INLIST_GET(pd->fd_handlers),
                                            EINA_INLIST_GET(fdh));
    if (fdh->file)
      pd->file_fd_handlers = eina_list_remove(pd->file_fd_handlers, fdh);
    if (fdh->flags & CORE_FD_ALWAYS)
      pd->always_fd_handlers = eina_list_remove(pd->always_fd_handlers, fdh);
    CORE_MAGIC_SET(fdh, CORE_MAGIC_NONE);
    core_fd_handler_mp_free(fdh);
    pd->fd_handlers_to_delete =
      eina_list_remove_list(pd->fd_handlers_to_delete, l);
  }
}

#ifdef _WIN32
static void
_core_main_win32_handlers_cleanup(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd)
{
  Core_Win32_Handler *wh;
  Eina_List          *l, *l2;

  if (!pd->win32_handlers_to_delete) return;
  EINA_LIST_FOREACH_SAFE(pd->win32_handlers_to_delete, l, l2, wh)
  {
    if (!wh)
    {
      pd->win32_handlers_to_delete =
        eina_list_remove_list(l, pd->win32_handlers_to_delete);
      continue;
    }
    // wh->delete_me should be set for all whs at the start of the list
    if (wh->references) continue;
    pd->win32_handlers = (Core_Win32_Handler *)eina_inlist_remove(
      EINA_INLIST_GET(pd->win32_handlers),
      EINA_INLIST_GET(wh));
    CORE_MAGIC_SET(wh, CORE_MAGIC_NONE);
    core_win32_handler_mp_free(wh);
    pd->win32_handlers_to_delete =
      eina_list_remove_list(pd->win32_handlers_to_delete, l);
  }
}
#endif

static void
_core_main_fd_handlers_call(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd)
{
  // grab a new list
  if (!pd->fd_handlers_to_call_current)
  {
    pd->fd_handlers_to_call_current = pd->fd_handlers_to_call;
    pd->fd_handlers_to_call         = NULL;
  }

  if (!pd->fd_handlers_to_call_current) return;
  eina_evlog("+fd_handlers", NULL, 0.0, NULL);

  while (pd->fd_handlers_to_call_current)
  {
    Core_Fd_Handler *fdh = pd->fd_handlers_to_call_current;

    if (!fdh->delete_me)
    {
      if ((fdh->read_active) || (fdh->write_active) || (fdh->error_active) ||
          (fdh->flags & CORE_FD_ALWAYS))
      {
        fdh->references++;
        if (!_core_call_fd_cb(fdh->func, fdh->data, fdh))
        {
          if (!fdh->delete_me)
          {
            fdh->delete_me = EFL_TRUE;
            _core_main_fdh_poll_del(pd, fdh);
            pd->fd_handlers_to_delete =
              eina_list_append(pd->fd_handlers_to_delete, fdh);
          }
        }
        fdh->references--;
#ifdef EFL_EXTRA_SANITY_CHECKS
        _core_fd_valid(obj, pd);
#endif
        fdh->read_active  = EFL_FALSE;
        fdh->write_active = EFL_FALSE;
        fdh->error_active = EFL_FALSE;
      }
    }

    // stop when we point to ourselves
    if (fdh->next_ready == fdh)
    {
      fdh->next_ready                 = NULL;
      pd->fd_handlers_to_call_current = NULL;
      break;
    }

    pd->fd_handlers_to_call_current = fdh->next_ready;
    fdh->next_ready                 = NULL;
  }
  eina_evlog("-fd_handlers", NULL, 0.0, NULL);
}

static int
_core_main_fd_handlers_buf_call(Eo *obj, Efl_Loop_Data *pd)
{
  Core_Fd_Handler *fdh;
  Eina_List       *l, *l2;
  int              ret;

  if (!pd->fd_handlers_with_buffer) return 0;
  eina_evlog("+fd_handlers_buf", NULL, 0.0, NULL);
  ret = 0;
  EINA_LIST_FOREACH_SAFE(pd->fd_handlers_with_buffer, l, l2, fdh)
  {
    if (!fdh)
    {
      pd->fd_handlers_with_buffer =
        eina_list_remove_list(l, pd->fd_handlers_with_buffer);
      continue;
    }
    if ((!fdh->delete_me) && fdh->buf_func)
    {
      fdh->references++;
      if (_core_call_fd_cb(fdh->buf_func, fdh->buf_data, fdh))
      {
        ret             |= _core_call_fd_cb(fdh->func, fdh->data, fdh);
        fdh->read_active = EFL_TRUE;
        _Core_try_add_to_call_list(obj, pd, fdh);
      }
      fdh->references--;
    }
    else
      pd->fd_handlers_with_buffer =
        eina_list_remove_list(pd->fd_handlers_with_buffer, l);
  }
  eina_evlog("-fd_handlers_buf", NULL, 0.0, NULL);
  return ret;
}

#ifdef HAVE_LIBUV
static void
_core_main_loop_uv_prepare(uv_prepare_t *handle EFL_UNUSED)
{
  Eo            *obj = ML_OBJ;
  Efl_Loop_Data *pd  = ML_DAT;
  double         t   = -1;

  _dl_uv_timer_stop(&_core_main_uv_handle_timers);
  if ((pd->in_loop == 0) && (pd->do_quit))
  {
    _core_main_fd_handlers_cleanup(obj, pd);

    while (pd->fd_handlers)
    {
      Core_Fd_Handler *fdh = pd->fd_handlers;
      pd->fd_handlers =
        (Core_Fd_Handler *)eina_inlist_remove(EINA_INLIST_GET(pd->fd_handlers),
                                              EINA_INLIST_GET(fdh));
      fdh->delete_me = 1;
      _core_main_fdh_poll_del(pd, fdh);
      CORE_MAGIC_SET(fdh, CORE_MAGIC_NONE);
      core_fd_handler_mp_free(fdh);
    }
    if (pd->fd_handlers_with_buffer)
      pd->fd_handlers_with_buffer = eina_list_free(pd->fd_handlers_with_buffer);
    if (pd->fd_handlers_with_prep)
      pd->fd_handlers_with_prep = eina_list_free(pd->fd_handlers_with_prep);
    if (pd->fd_handlers_to_delete)
      pd->fd_handlers_to_delete = eina_list_free(pd->fd_handlers_to_delete);
    if (pd->file_fd_handlers)
      pd->file_fd_handlers = eina_list_free(pd->file_fd_handlers);

    pd->fd_handlers_to_call         = NULL;
    pd->fd_handlers_to_call_current = NULL;

    _dl_uv_prepare_stop(&_core_main_uv_prepare);
    _dl_uv_check_stop(&_core_main_uv_check);
    _dl_uv_stop(_dl_uv_default_loop());

    return;
  }

  in_main_loop++;
  pd->in_loop = in_main_loop;

  if (!_core_main_uv_idling)
  {
    _core_main_uv_idling = EFL_TRUE;
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_ENTER, NULL);
    _core_throttle();
    _throttle_do(pd);
  }

  if (_core_main_uv_idling)
  {
    _core_main_idler_all_call(obj, pd);
    DBG("called idles");
    if (_core_main_idlers_exist(pd) || (pd->message_queue)) t = 0.0;
  }

  if (pd->do_quit)
  {
    DBG("do quit outside loop");

    if (_core_main_uv_idling)
    {
      eina_file_statgen_next();
      _core_main_pre_idle_exit();
      efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_EXIT, NULL);
      _core_animator_run_reset();
      _core_main_uv_idling = EFL_FALSE;
    }
    t = -1;
    _update_loop_time(pd);
    _efl_loop_timer_enable_new(obj, pd);
    goto done;
  }

  assert(!pd->fd_handlers_to_call);

  _update_loop_time(pd);
  _efl_loop_timer_enable_new(obj, pd);
  if (_efl_loop_timers_exists(obj, pd) || (t >= 0))
  {
    double tnext = _efl_loop_timer_next_get(obj, pd);

    if ((t < 0) || ((tnext >= 0) && (tnext < t))) t = tnext;
    DBG("Should awake after %f", t);

    if (t >= 0.0)
    {
      // _dl_uv_timer_stop(&_core_main_uv_handle_timers);
      _dl_uv_timer_start(&_core_main_uv_handle_timers,
                         &_core_main_loop_timer_run,
                         t * 1000,
                         0);
    }
    else DBG("Is not going to awake with timer");
  }
  else DBG("Is not going to awake with timer");

done:
  if (pd->fd_handlers_with_prep) _core_main_prepare_handlers(obj, pd);
  in_main_loop--;
  pd->in_loop = in_main_loop;
}
#endif

#if !defined(USE_G_MAIN_LOOP)
enum
{
  SPIN_MORE,
  SPIN_RESTART,
  LOOP_CONTINUE
};

static int
_core_main_loop_spin_core(Eo *obj, Efl_Loop_Data *pd)
{
  // as we are spinning we need to update loop time per spin
  _update_loop_time(pd);
  // call all idlers
  _core_main_idler_all_call(obj, pd);
  // which returns false if no more idelrs exist
  if (!_core_main_idlers_exist(pd)) return SPIN_RESTART;
  // sneaky - drop through or if checks - the first one to succeed
  // drops through and returns "continue" so further ones dont run
  if ((_core_main_select(obj, pd, 0.0) > 0) || (pd->message_queue) ||
      (_core_signal_count_get(obj, pd) > 0) || (pd->do_quit))
    return LOOP_CONTINUE;
  // default - spin more
  return SPIN_MORE;
}

static int
_core_main_loop_spin_no_timers(Eo *obj, Efl_Loop_Data *pd)
{
  // if we have idlers we HAVE to spin and handle everything
  // in a polling way - spin in a tight polling loop
  for (;;)
  {
    int action = _core_main_loop_spin_core(obj, pd);
    if (action != SPIN_MORE) return action;
    // if an idler has added a timer then we need to go through
    // the start of the spin cycle again to handle cases properly
    if (_efl_loop_timers_exists(obj, pd)) return SPIN_RESTART;
  }
  // just contiune handling events etc.
  return LOOP_CONTINUE;
}

static int
_core_main_loop_spin_timers(Eo *obj, Efl_Loop_Data *pd)
{
  // if we have idlers we HAVE to spin and handle everything
  // in a polling way - spin in a tight polling loop
  for (;;)
  {
    int action = _core_main_loop_spin_core(obj, pd);
    if (action != SPIN_MORE) return action;
    // if next timer expires now or in the past - stop spinning and
    // continue the mainloop walk as our "select" timeout has
    // expired now
    if (_efl_loop_timer_next_get(obj, pd) <= 0.0) return LOOP_CONTINUE;
  }
  // just contiune handling events etc.
  return LOOP_CONTINUE;
}

static void
_Core_fps_marker_1(void)
{
  if (!_core_fps_debug) return;
  t2 = core_time_get();
  if ((t1 > 0.0) && (t2 > 0.0)) _core_fps_debug_runtime_add(t2 - t1);
}

static void
_Core_fps_marker_2(void)
{
  if (!_core_fps_debug) return;
  t1 = core_time_get();
}

static void
_core_main_loop_iterate_internal(Eo *obj, Efl_Loop_Data *pd, int once_only)
{
  double next_time = -1.0;

  if (obj == ML_OBJ)
  {
    in_main_loop++;
    pd->in_loop = in_main_loop;
  }
  // expire any timers
  _efl_loop_timer_expired_timers_call(obj, pd, pd->loop_time);
  // process signals into events ....
  if (obj == ML_OBJ) _core_signal_received_process(obj, pd);
  // if as a result of timers/animators or signals we have accumulated
  // events, then instantly handle them
  if (pd->message_queue)
  {
    // but first conceptually enter an idle state
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_ENTER, NULL);
    _core_throttle();
    _throttle_do(pd);
    // now quickly poll to see which input fd's are active
    _core_main_select(obj, pd, 0.0);
    // allow newly queued timers to expire from now on
    _efl_loop_timer_enable_new(obj, pd);
    // go straight to processing the events we had queued
    goto process_all;
  }

  if (once_only)
  {
    // in once_only mode we should quickly poll for inputs, signals
    // if we got any events or signals, allow new timers to process.
    // use bitwise or to force both conditions to be tested and
    // merged together
    if (_core_main_select(obj, pd, 0.0) | _core_signal_count_get(obj, pd))
    {
      _efl_loop_timer_enable_new(obj, pd);
      goto process_all;
    }
  }
  else
  {
    // call idle enterers ...
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_ENTER, NULL);
    _core_throttle();
    _throttle_do(pd);
  }

  // if these calls caused any buffered events to appear - deal with them
  if (pd->fd_handlers_with_buffer) _core_main_fd_handlers_buf_call(obj, pd);

  // if there are any (buffered fd handling may generate them)
  // then jump to processing them */
  if (pd->message_queue)
  {
    _core_main_select(obj, pd, 0.0);
    _efl_loop_timer_enable_new(obj, pd);
    goto process_all;
  }

  if (once_only)
  {
    // in once_only mode enter idle here instead and then return
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_ENTER, NULL);
    _core_throttle();
    _throttle_do(pd);
    _efl_loop_timer_enable_new(obj, pd);
    goto done;
  }

  if (obj == ML_OBJ) _Core_fps_marker_1();

  // start of the sleeping or looping section
start_loop:  //-*************************************************************
  // any timers re-added as a result of these are allowed to go
  _efl_loop_timer_enable_new(obj, pd);
  // if we have been asked to quit the mainloop then exit at this point
  if (pd->do_quit)
  {
    _efl_loop_timer_enable_new(obj, pd);
    goto done;
  }
  if (!pd->message_queue)
  {
    // init flags
    next_time = _efl_loop_timer_next_get(obj, pd);
    // no idlers
    if (!_core_main_idlers_exist(pd))
    {
      // sleep until timeout or forever (-1.0) waiting for on fds
      _core_main_select(obj, pd, next_time);
    }
    else
    {
      int action = LOOP_CONTINUE;

      // no timers - spin
      if (next_time < 0) action = _core_main_loop_spin_no_timers(obj, pd);
      // timers - spin
      else action = _core_main_loop_spin_timers(obj, pd);
      if (action == SPIN_RESTART) goto start_loop;
    }
  }
  if (obj == ML_OBJ) _Core_fps_marker_2();

  // actually wake up and deal with input, events etc.
process_all:  //-*********************************************************

  // we came out of our "wait state" so idle has exited
  if (!once_only)
  {
    _core_animator_run_reset();  // XXX:
    eina_file_statgen_next();
    _core_main_pre_idle_exit();
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_EXIT, NULL);
  }
  // call the fd handler per fd that became alive...
  // this should read or write any data to the monitored fd and then
  // post events onto the ecore event pipe if necessary
  _core_main_fd_handlers_call(obj, pd);
  if (pd->fd_handlers_with_buffer) _core_main_fd_handlers_buf_call(obj, pd);
  // process signals into events ....
  _core_signal_received_process(obj, pd);
  // handle events ...
  efl_loop_message_process(obj);
  _core_main_fd_handlers_cleanup(obj, pd);

  if (once_only)
  {
    // if in once_only mode handle idle exiting
    efl_event_callback_call(obj, EFL_LOOP_EVENT_IDLE_ENTER, NULL);
    _core_throttle();
    _throttle_do(pd);
  }

done:  //-*****************************************************************
  // Agressively flush animator
  _core_animator_flush();
  if (!once_only) eina_slstr_local_clear();  // Free all short lived strings

  if (obj == ML_OBJ)
  {
    in_main_loop--;
    pd->in_loop = in_main_loop;
  }
}

#endif

#ifdef _WIN32
typedef struct
{
  DWORD   objects_nbr;
  HANDLE *objects;
  DWORD   timeout;
} Core_Main_Win32_Thread_Data;

static unsigned int __stdcall _core_main_win32_objects_wait_thread(void *data)
{
  Core_Main_Win32_Thread_Data *td = (Core_Main_Win32_Thread_Data *)data;
  return MsgWaitForMultipleObjects(td->objects_nbr,
                                   (const HANDLE *)td->objects,
                                   FALSE,
                                   td->timeout,
                                   QS_ALLINPUT);
}

static DWORD
_core_main_win32_objects_wait(DWORD         objects_nbr,
                              const HANDLE *objects,
                              DWORD         timeout)
{
  Core_Main_Win32_Thread_Data *threads_data;
  HANDLE                      *threads_handles;
  DWORD threads_nbr, threads_remain, objects_idx, result, i;

  if (objects_nbr < MAXIMUM_WAIT_OBJECTS)
    return MsgWaitForMultipleObjects(objects_nbr,
                                     objects,
                                     EFL_FALSE,
                                     timeout,
                                     QS_ALLINPUT);
  // too much objects, so we launch a bunch of threads to
  // wait for, each one calls MsgWaitForMultipleObjects

  threads_nbr    = objects_nbr / (MAXIMUM_WAIT_OBJECTS - 1);
  threads_remain = objects_nbr % (MAXIMUM_WAIT_OBJECTS - 1);
  if (threads_remain > 0) threads_nbr++;

  if (threads_nbr > MAXIMUM_WAIT_OBJECTS)
  {
    CRI("Too much objects to wait for (%lu).", objects_nbr);
    return WAIT_FAILED;
  }

  threads_handles = (HANDLE *)malloc(threads_nbr * sizeof(HANDLE));
  if (!threads_handles)
  {
    ERR("Can not allocate memory for the waiting thread.");
    return WAIT_FAILED;
  }

  threads_data = (Core_Main_Win32_Thread_Data *)malloc(
    threads_nbr * sizeof(Core_Main_Win32_Thread_Data));
  if (!threads_data)
  {
    ERR("Can not allocate memory for the waiting thread.");
    goto free_threads_handles;
  }

  objects_idx = 0;
  for (i = 0; i < threads_nbr; i++)
  {
    threads_data[i].timeout = timeout;
    threads_data[i].objects = (HANDLE *)objects + objects_idx;

    if ((i == (threads_nbr - 1)) && (threads_remain != 0))
    {
      threads_data[i].objects_nbr = threads_remain;
      objects_idx                += threads_remain;
    }
    else
    {
      threads_data[i].objects_nbr = (MAXIMUM_WAIT_OBJECTS - 1);
      objects_idx                += (MAXIMUM_WAIT_OBJECTS - 1);
    }

    threads_handles[i] =
      (HANDLE)_beginthreadex(NULL,
                             0,
                             _core_main_win32_objects_wait_thread,
                             &threads_data[i],
                             0,
                             NULL);
    if (!threads_handles[i])
    {
      DWORD j;

      ERR("Can not create the waiting threads.");
      WaitForMultipleObjects(i, threads_handles, TRUE, INFINITE);
      for (j = 0; j < i; j++)
        CloseHandle(threads_handles[i]);

      goto free_threads_data;
    }
  }

  result = WaitForMultipleObjects(threads_nbr,
                                  threads_handles,
                                  FALSE,  // we wait until one thread signaled
                                  INFINITE);

  if (result < (WAIT_OBJECT_0 + threads_nbr))
  {
    DWORD wait_res;

    // One of the thread callback has exited so we retrieve
    // its exit status, that is the returned value of
    // MsgWaitForMultipleObjects()
    if (GetExitCodeThread(threads_handles[result - WAIT_OBJECT_0], &wait_res))
    {
      WaitForMultipleObjects(threads_nbr, threads_handles, TRUE, INFINITE);
      for (i = 0; i < threads_nbr; i++)
        CloseHandle(threads_handles[i]);
      free(threads_data);
      free(threads_handles);
      return wait_res;
    }
  }
  else
  {
    ERR("Error when waiting threads.");
    if (result == WAIT_FAILED) ERR("%s", evil_last_error_get());
    goto close_thread;
  }

close_thread:
  WaitForMultipleObjects(threads_nbr, threads_handles, TRUE, INFINITE);
  for (i = 0; i < threads_nbr; i++)
    CloseHandle(threads_handles[i]);
free_threads_data:
  free(threads_data);
free_threads_handles:
  free(threads_handles);

  return WAIT_FAILED;
}

static unsigned int
_stdin_wait_thread(void *data EFL_UNUSED)
{
  int c = getc(stdin);
  ungetc(c, stdin);
  return 0;
}

static int
_core_main_win32_select(int nfds        EFL_UNUSED,
                        fd_set         *readfds,
                        fd_set         *writefds,
                        fd_set         *exceptfds,
                        struct timeval *tv)
{
  Efl_Loop_Data      *pd = ML_DAT;
  HANDLE             *objects;
  int                *sockets;
  Core_Fd_Handler    *fdh;
  Core_Win32_Handler *wh;
  static HANDLE       stdin_wait_thread = INVALID_HANDLE_VALUE;
  HANDLE              stdin_handle;
  DWORD               result, timeout;
  MSG                 msg;
  unsigned int        fds_nbr     = 0;
  unsigned int        objects_nbr = 0;
  unsigned int        events_nbr  = 0;
  unsigned int        i;
  int                 res;
  Efl_Bool            stdin_thread_done = EFL_FALSE;

  fds_nbr = eina_inlist_count(EINA_INLIST_GET(pd->fd_handlers));
  sockets = (int *)malloc(fds_nbr * sizeof(int));
  if (!sockets) return -1;

  objects = (HANDLE)malloc(
    (fds_nbr + eina_inlist_count(EINA_INLIST_GET(pd->win32_handlers))) *
    sizeof(HANDLE));
  if (!objects)
  {
    free(sockets);
    return -1;
  }

  // Create an event object per socket
  EINA_INLIST_FOREACH(pd->fd_handlers, fdh)
  {
    if (fdh->delete_me) continue;

    WSAEVENT event;
    long     network_event;

    network_event = 0;
    if (readfds)
    {
      if (FD_ISSET(fdh->fd, readfds))
        network_event |= FD_READ | FD_CONNECT | FD_ACCEPT;
    }
    if (writefds)
    {
      if (FD_ISSET(fdh->fd, writefds)) network_event |= FD_WRITE | FD_CLOSE;
    }
    if (exceptfds)
    {
      if (FD_ISSET(fdh->fd, exceptfds)) network_event |= FD_OOB;
    }

    if (network_event)
    {
      event = WSACreateEvent();
      WSAEventSelect(fdh->fd, event, network_event);
      objects[objects_nbr] = event;
      sockets[events_nbr]  = fdh->fd;
      events_nbr++;
      objects_nbr++;
    }
  }
  stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
  // store the HANDLEs in the objects to wait for
  EINA_INLIST_FOREACH(pd->win32_handlers, wh)
  {
    if (wh->delete_me) continue;

    if (wh->h == stdin_handle)
    {
      if (stdin_wait_thread == INVALID_HANDLE_VALUE)
        stdin_wait_thread =
          (HANDLE)_beginthreadex(NULL, 0, _stdin_wait_thread, NULL, 0, NULL);
      objects[objects_nbr] = stdin_wait_thread;
    }
    else objects[objects_nbr] = wh->h;
    objects_nbr++;
  }

  // Empty the queue before waiting
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // Wait for any message sent or posted to this queue
  // or for one of the passed handles be set to signaled.
  if (!tv) timeout = INFINITE;
  else timeout = (DWORD)((tv->tv_sec * 1000.0) + (tv->tv_usec / 1000.0));

  if (timeout == 0)
  {
    res = 0;
    goto err;
  }

  result = _core_main_win32_objects_wait(objects_nbr,
                                         (const HANDLE *)objects,
                                         timeout);
  if (readfds) FD_ZERO(readfds);
  if (writefds) FD_ZERO(writefds);
  if (exceptfds) FD_ZERO(exceptfds);
  // The result tells us the type of event we have.
  if (result == WAIT_FAILED)
  {
    WRN("%s", evil_last_error_get());
    res = -1;
  }
  else if (result == WAIT_TIMEOUT)
  {
    INF("time-out interval elapsed.");
    res = 0;
  }
  else if (result == (WAIT_OBJECT_0 + objects_nbr))
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    res = 0;
  }
  else if (result < WAIT_OBJECT_0 + events_nbr)
  {
    WSANETWORKEVENTS network_event;

    WSAEnumNetworkEvents(sockets[result], objects[result], &network_event);
    if ((network_event.lNetworkEvents & (FD_READ | FD_CONNECT | FD_ACCEPT)) &&
        readfds)
      FD_SET(sockets[result], readfds);
    if ((network_event.lNetworkEvents & (FD_WRITE | FD_CLOSE)) && writefds)
      FD_SET(sockets[result], writefds);
    if ((network_event.lNetworkEvents & FD_OOB) && exceptfds)
      FD_SET(sockets[result], exceptfds);
    res = 1;
  }
  else if ((result >= (WAIT_OBJECT_0 + events_nbr)) &&
           (result < (WAIT_OBJECT_0 + objects_nbr)))
  {
    if (!pd->win32_handler_current)
      // regular main loop, start from head
      pd->win32_handler_current = pd->win32_handlers;
    else
      // recursive main loop, continue from where we were
      pd->win32_handler_current =
        (Core_Win32_Handler *)EINA_INLIST_GET(pd->win32_handler_current)->next;

    if (objects[result - WAIT_OBJECT_0] == stdin_wait_thread)
      stdin_thread_done = EFL_TRUE;

    while (pd->win32_handler_current)
    {
      wh = pd->win32_handler_current;

      if ((objects[result - WAIT_OBJECT_0] == wh->h) ||
          ((objects[result - WAIT_OBJECT_0] == stdin_wait_thread) &&
           (wh->h == stdin_handle)))
      {
        if (!wh->delete_me)
        {
          wh->references++;
          if (!wh->func(wh->data, wh))
          {
            wh->delete_me = EFL_TRUE;
            pd->win32_handlers_to_delete =
              eina_list_append(pd->win32_handlers_to_delete, wh);
          }
          wh->references--;
        }
      }
      if (pd->win32_handler_current)
        // may have changed in recursive main loops
        pd->win32_handler_current =
          (Core_Win32_Handler *)EINA_INLIST_GET(pd->win32_handler_current)
            ->next;
    }
    res = 1;
  }
  else
  {
    ERR("unknown result...\n");
    res = -1;
  }

err:
  // Remove event objects again
  for (i = 0; i < events_nbr; i++)
    WSACloseEvent(objects[i]);

  if (stdin_thread_done) stdin_wait_thread = INVALID_HANDLE_VALUE;

  free(objects);
  free(sockets);
  return res;
}
#endif
