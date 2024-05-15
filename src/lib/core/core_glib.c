#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif
#ifdef _WIN32
#  include <ws2tcpip.h>
#endif
#ifdef HAVE_NETDB_H
#  include <netdb.h>
#endif
#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif

#include "Efl_Core.h"
#include "core_private.h"

#ifdef HAVE_GLIB
#  include <glib.h>

static Efl_Bool              _Core_glib_active = EFL_FALSE;
static Core_Select_Function _Core_glib_select_original;
static GPollFD              *_Core_glib_fds         = NULL;
static size_t                _Core_glib_fds_size    = 0;
static const size_t          Core_GLIB_FDS_INITIAL  = 128;
static const size_t          Core_GLIB_FDS_STEP     = 8;
static const size_t          Core_GLIB_FDS_MAX_FREE = 256;
#  if GLIB_CHECK_VERSION(2, 32, 0)
static GRecMutex *_Core_glib_select_lock;
#  else
static GStaticRecMutex *_Core_glib_select_lock;
#  endif

static Efl_Bool
_Core_glib_fds_resize(size_t size)
{
    void *tmp = realloc(_Core_glib_fds, sizeof(GPollFD) * size);

    if (!tmp)
    {
        ERR("Could not realloc from %zu to %zu buckets.",
            _Core_glib_fds_size,
            size);
        return EFL_FALSE;
    }

    _Core_glib_fds      = tmp;
    _Core_glib_fds_size = size;
    return EFL_TRUE;
}

static int
_Core_glib_context_query(GMainContext *ctx, int priority, int *p_timer)
{
    int reqfds;

    if (_Core_glib_fds_size == 0)
    {
        if (!_Core_glib_fds_resize(Core_GLIB_FDS_INITIAL)) return -1;
    }

    while (1)
    {
        size_t size;

        reqfds = g_main_context_query(ctx,
                                      priority,
                                      p_timer,
                                      _Core_glib_fds,
                                      _Core_glib_fds_size);
        if (reqfds <= (int)_Core_glib_fds_size) break;

        size = (1 + reqfds / Core_GLIB_FDS_STEP) * Core_GLIB_FDS_STEP;
        if (!_Core_glib_fds_resize(size)) return -1;
    }

    if (reqfds + Core_GLIB_FDS_MAX_FREE < _Core_glib_fds_size)
    {
        size_t size;

        size = (1 + reqfds / Core_GLIB_FDS_MAX_FREE) * Core_GLIB_FDS_MAX_FREE;
        _Core_glib_fds_resize(size);
    }

    return reqfds;
}

static int
_Core_glib_context_poll_from(const GPollFD *pfds,
                              int            count,
                              fd_set        *rfds,
                              fd_set        *wfds,
                              fd_set        *efds)
{
    const GPollFD *itr = pfds, *itr_end = pfds + count;
    int            glib_fds = -1;

    for (; itr < itr_end; itr++)
    {
        if (glib_fds < itr->fd) glib_fds = itr->fd;

        if (itr->events & G_IO_IN) FD_SET(itr->fd, rfds);
        if (itr->events & G_IO_OUT) FD_SET(itr->fd, wfds);
        if (itr->events & (G_IO_HUP | G_IO_ERR)) FD_SET(itr->fd, efds);
    }

    return glib_fds + 1;
}

static int
_Core_glib_context_poll_to(GPollFD      *pfds,
                            int           count,
                            const fd_set *rfds,
                            const fd_set *wfds,
                            const fd_set *efds,
                            int           ready)
{
    GPollFD    *itr = pfds, *itr_end = pfds + count;
    struct stat st;

    for (; (itr < itr_end) && (ready > 0); itr++)
    {
        itr->revents = 0;
        if (FD_ISSET(itr->fd, rfds) && (itr->events & G_IO_IN))
        {
            itr->revents |= G_IO_IN;
            ready--;
        }
        if (FD_ISSET(itr->fd, wfds) && (itr->events & G_IO_OUT))
        {
            itr->revents |= G_IO_OUT;
            ready--;
            if (!fstat(itr->fd, &st))
            {
                if (S_ISSOCK(st.st_mode))
                {
                    struct sockaddr_in peer;
                    socklen_t          length = sizeof(peer);

                    memset(&peer, 0, sizeof(peer));
                    if (getpeername(itr->fd, (struct sockaddr *)&peer, &length))
                        itr->revents |= G_IO_ERR;
                }
            }
        }
        if (FD_ISSET(itr->fd, efds) && (itr->events & (G_IO_HUP | G_IO_ERR)))
        {
            itr->revents |= G_IO_ERR;
            ready--;
        }
    }
    return ready;
}

static int
_Core_glib_select__locked(GMainContext   *ctx,
                           int             Core_fds,
                           fd_set         *rfds,
                           fd_set         *wfds,
                           fd_set         *efds,
                           struct timeval *core_timeout)
{
    int             priority, maxfds, glib_fds, reqfds, reqtimeout, ret;
    struct timeval *timeout, glib_timeout;

    g_main_context_prepare(ctx, &priority);
    reqfds = _Core_glib_context_query(ctx, priority, &reqtimeout);
    if (reqfds < 0) goto error;

    glib_fds = _Core_glib_context_poll_from(_Core_glib_fds,
                                             reqfds,
                                             rfds,
                                             wfds,
                                             efds);

    if (reqtimeout == -1) timeout = core_timeout;
    else
    {
        glib_timeout.tv_sec  = reqtimeout / 1000;
        glib_timeout.tv_usec = (reqtimeout % 1000) * 1000;

        if (!core_timeout || timercmp(core_timeout, &glib_timeout, >))
            timeout = &glib_timeout;
        else timeout = core_timeout;
    }

    maxfds = (Core_fds >= glib_fds) ? Core_fds : glib_fds;
    ret    = _Core_glib_select_original(maxfds, rfds, wfds, efds, timeout);

    ret = _Core_glib_context_poll_to(_Core_glib_fds,
                                      reqfds,
                                      rfds,
                                      wfds,
                                      efds,
                                      ret);

    if (g_main_context_check(ctx, priority, _Core_glib_fds, reqfds))
        g_main_context_dispatch(ctx);

    return ret;

error:
    return _Core_glib_select_original(Core_fds,
                                       rfds,
                                       wfds,
                                       efds,
                                       core_timeout);
}

static int
_Core_glib_select(int             Core_fds,
                   fd_set         *rfds,
                   fd_set         *wfds,
                   fd_set         *efds,
                   struct timeval *core_timeout)
{
    GMainContext *ctx;
    int           ret;

    ctx = g_main_context_default();

    while (!g_main_context_acquire(ctx))
        g_thread_yield();

#  if GLIB_CHECK_VERSION(2, 32, 0)
    g_rec_mutex_lock(_Core_glib_select_lock);
#  else
    g_static_rec_mutex_lock(_Core_glib_select_lock);
#  endif

    ret = _Core_glib_select__locked(ctx,
                                     Core_fds,
                                     rfds,
                                     wfds,
                                     efds,
                                     core_timeout);

#  if GLIB_CHECK_VERSION(2, 32, 0)
    g_rec_mutex_unlock(_Core_glib_select_lock);
#  else
    g_static_rec_mutex_unlock(_Core_glib_select_lock);
#  endif
    g_main_context_release(ctx);

    return ret;
}

#endif

void
_Core_glib_init(void)
{
#ifdef HAVE_GLIB
#  if GLIB_CHECK_VERSION(2, 32, 0)
    _Core_glib_select_lock = malloc(sizeof(GRecMutex));
    g_rec_mutex_init(_Core_glib_select_lock);
#  else
    if (!g_thread_get_initialized()) g_thread_init(NULL);
    _Core_glib_select_lock = malloc(sizeof(GStaticRecMutex));
    g_static_rec_mutex_init(_Core_glib_select_lock);
#  endif
#endif
}

void
_core_glib_shutdown(void)
{
#ifdef HAVE_GLIB
    if (!_Core_glib_active) return;
    _Core_glib_active = EFL_FALSE;

    if (core_main_loop_select_func_get() == _Core_glib_select)
        core_main_loop_select_func_set(_Core_glib_select_original);

    if (_Core_glib_fds)
    {
        free(_Core_glib_fds);
        _Core_glib_fds = NULL;
    }
    _Core_glib_fds_size = 0;

#  if GLIB_CHECK_VERSION(2, 32, 0)
    g_rec_mutex_clear(_Core_glib_select_lock);
    free(_Core_glib_select_lock);
    _Core_glib_select_lock = NULL;
#  else
    g_static_rec_mutex_free(_Core_glib_select_lock);
    _Core_glib_select_lock = NULL;
#  endif
#endif
}

EAPI Efl_Bool
core_main_loop_glib_integrate(void)
{
#ifdef HAVE_GLIB
    void *func;

    if (_Core_glib_active) return EFL_TRUE;
    func = core_main_loop_select_func_get();
    if (func == _Core_glib_select) return EFL_TRUE;
    _Core_glib_select_original = func;
    core_main_loop_select_func_set(_Core_glib_select);
    _Core_glib_active = EFL_TRUE;

   /* Init only when requested */
    _Core_glib_init();
    return EFL_TRUE;
#else
    ERR("No glib support");
    return EFL_FALSE;
#endif
}

Efl_Bool _Core_glib_always_integrate = 1;

EAPI void
core_main_loop_glib_always_integrate_disable(void)
{
    _Core_glib_always_integrate = 0;
}
