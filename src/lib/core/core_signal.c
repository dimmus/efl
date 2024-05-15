#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include "Efl_Core.h"
#include "core_private.h"

/* make mono happy - this is evil though... */
#undef SIGPWR

typedef struct _Pid_Info Pid_Info;

struct _Pid_Info
{
    pid_t pid;
    int   fd;
};

static void _Core_signal_exe_exit_delay(void *data, const Efl_Event *event);
static void _Core_signal_waitpid(Efl_Bool once, siginfo_t info);
static void _Core_signal_generic_free(void *data, void *event);

typedef void (*Signal_Handler)(int sig, siginfo_t *si, void *foo);

#define NUM_PIPES 5

static int sig_pipe[NUM_PIPES][2] = { { -1 } }; // [0] == read, [1] == write
static Eo *sig_pipe_handler[NUM_PIPES] = { NULL };
static Eina_Spinlock sig_pid_lock;
static Eina_List    *sig_pid_info_list = NULL;

volatile int pipe_dead            = 0;
volatile int exit_signal_received = 0;

typedef struct _Signal_Data
{
    int       sig;
    siginfo_t info;
} Signal_Data;

static void
_Core_signal_pipe_read(Eo *obj)
{
    Signal_Data sdata;
    int         ret;

    if (pipe_dead) return;
    for (unsigned int i = 0; i < NUM_PIPES; i++)
    {
        while (1)
        {
            ret = read(sig_pipe[i][0], &sdata, sizeof(sdata));

            /* read as many signals as we can, trying again if we get interrupted */
            if ((ret != sizeof(sdata)) && (errno != EINTR)) break;
            switch (sdata.sig)
            {
                case SIGPIPE:
                    break;
                case SIGALRM:
                    break;
                case SIGCHLD:
                    _Core_signal_waitpid(EFL_FALSE, sdata.info);
                    break;
                case SIGUSR1:
                case SIGUSR2:
                    {
                        Core_Event_Signal_User *e =
                            _core_event_signal_user_new();
                        if (e)
                        {
                            if (sdata.sig == SIGUSR1) e->number = 1;
                            else e->number = 2;
                            e->data = sdata.info;
                            core_event_add(CORE_EVENT_SIGNAL_USER,
                                            e,
                                            _Core_signal_generic_free,
                                            NULL);
                        }
                        Eo *loop = efl_provider_find(obj, EFL_LOOP_CLASS);
                        if (loop)
                        {
                            if (sdata.sig == SIGUSR1)
                                efl_event_callback_call(
                                    loop,
                                    EFL_APP_EVENT_SIGNAL_USR1,
                                    NULL);
                            else
                                efl_event_callback_call(
                                    loop,
                                    EFL_APP_EVENT_SIGNAL_USR2,
                                    NULL);
                        }
                    }
                    break;
                case SIGHUP:
                    {
                        Core_Event_Signal_Hup *e =
                            _core_event_signal_hup_new();
                        if (e)
                        {
                            e->data = sdata.info;
                            core_event_add(CORE_EVENT_SIGNAL_HUP,
                                            e,
                                            _Core_signal_generic_free,
                                            NULL);
                        }
                        Eo *loop = efl_provider_find(obj, EFL_LOOP_CLASS);
                        if (loop)
                            efl_event_callback_call(loop,
                                                    EFL_APP_EVENT_SIGNAL_HUP,
                                                    NULL);
                    }
                    break;
                case SIGQUIT:
                case SIGINT:
                case SIGTERM:
                    {
                        Core_Event_Signal_Exit *e =
                            _core_event_signal_exit_new();
                        if (e)
                        {
                            if (sdata.sig == SIGQUIT) e->quit = 1;
                            else if (sdata.sig == SIGINT) e->interrupt = 1;
                            else e->terminate = 1;
                            e->data = sdata.info;
                            core_event_add(CORE_EVENT_SIGNAL_EXIT,
                                            e,
                                            _Core_signal_generic_free,
                                            NULL);
                        }
                        Eo *loop = efl_provider_find(obj, EFL_LOOP_CLASS);
                        if (loop)
                            efl_event_callback_call(loop,
                                                    EFL_LOOP_EVENT_QUIT,
                                                    NULL);
                    }
                    break;
#ifdef SIGPWR
                case SIGPWR:
                    {
                        Core_Event_Signal_Power *e =
                            _core_event_signal_power_new();
                        if (e)
                        {
                            e->data = sdata.info;
                            core_event_add(CORE_EVENT_SIGNAL_POWER,
                                            e,
                                            _Core_signal_generic_free,
                                            NULL);
                        }
                    }
                    break;
#endif
                default:
                    break;
            }
        }
    }
}

static void
_Core_signal_cb_read(void *data EFL_UNUSED, const Efl_Event *event EFL_UNUSED)
{
    _Core_signal_pipe_read(event->object);
}

static void
_Core_signal_callback(int sig, siginfo_t *si, void *foo EFL_UNUSED)
{
    Signal_Data sdata;

    memset(&sdata, 0, sizeof(Signal_Data));
    sdata.sig  = sig;
    sdata.info = *si;
    if (sdata.sig >= 0)
    {
        int err = errno;
        if (pipe_dead) return;
        for (unsigned int i = 0; i < NUM_PIPES; i++)
        {
            do
            {
                err = 0;
                const ssize_t bytes =
                    write(sig_pipe[i][1], &sdata, sizeof(sdata));
                if (EINA_UNLIKELY(bytes != sizeof(sdata)))
                {
                    err = errno;
                    if (err == EINTR) DBG("signal pipe %u full", i);
                    else if (i ==
                             NUM_PIPES - 1)  //only print errors on last pipe
                        ERR("write() failed: %d: %s", err, strerror(err));
                }
                errno = err;
                /* loop if we got preempted */
            }
            while (err == EINTR);
            if (!err) break;
        }
    }
    switch (sig)
    {
        case SIGQUIT:
        case SIGINT:
        case SIGTERM:
            exit_signal_received = 1;
            break;
        default:
            break;
    }
}

static void
_Core_signal_callback_set(int sig, Signal_Handler func)
{
    struct sigaction sa;

    sa.sa_sigaction = func;
    sa.sa_flags     = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(sig, &sa, NULL);
}

static void
_signalhandler_setup(void)
{
    sigset_t newset;

    _Core_signal_callback_set(SIGPIPE, _Core_signal_callback);
    _Core_signal_callback_set(SIGALRM, _Core_signal_callback);
    _Core_signal_callback_set(SIGCHLD, _Core_signal_callback);
    _Core_signal_callback_set(SIGUSR1, _Core_signal_callback);
    _Core_signal_callback_set(SIGUSR2, _Core_signal_callback);
    _Core_signal_callback_set(SIGHUP, _Core_signal_callback);
    _Core_signal_callback_set(SIGQUIT, _Core_signal_callback);
    _Core_signal_callback_set(SIGINT, _Core_signal_callback);
    _Core_signal_callback_set(SIGTERM, _Core_signal_callback);
#ifdef SIGPWR
    _Core_signal_callback_set(SIGPWR, _Core_signal_callback);
#endif

#ifndef _WIN32
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
#  ifdef SIGPWR
    sigaddset(&newset, SIGPWR);
#  endif
    pthread_sigmask(SIG_UNBLOCK, &newset, NULL);
#endif
}

static void
_Core_signal_pipe_init(void)
{
    eina_spinlock_new(&sig_pid_lock);
    _signalhandler_setup();
    if (sig_pipe[0][0] == -1)
    {
        for (unsigned int i = 0; i < NUM_PIPES; i++)
        {
            if (pipe(sig_pipe[i]) != 0)
            {
                CRI("failed setting up signal pipes! %s", strerror(errno));
                for (unsigned int j = 0; j < i; j++)
                {
                    close(sig_pipe[j][0]);
                    close(sig_pipe[j][1]);
                }
                memset(sig_pipe, -1, sizeof(sig_pipe));
                return;
            }
            eina_file_close_on_exec(sig_pipe[i][0], EFL_TRUE);
            eina_file_close_on_exec(sig_pipe[i][1], EFL_TRUE);
            if (fcntl(sig_pipe[i][0], F_SETFL, O_NONBLOCK) < 0)
                ERR("can't set pipe to NONBLOCK");
            if (fcntl(sig_pipe[i][1], F_SETFL, O_NONBLOCK) < 0)
                ERR("can't set pipe to NONBLOCK");
            efl_add(EFL_LOOP_HANDLER_CLASS,
                    ML_OBJ,
                    efl_loop_handler_fd_set(efl_added, sig_pipe[i][0]),
                    efl_loop_handler_active_set(efl_added,
                                                EFL_LOOP_HANDLER_FLAGS_READ),
                    efl_event_callback_add(efl_added,
                                           EFL_LOOP_HANDLER_EVENT_READ,
                                           _Core_signal_cb_read,
                                           NULL),
                    efl_wref_add(efl_added, &sig_pipe_handler[i]));
        }
    }
}

static void
_Core_signal_pipe_shutdown(void)
{
    if (sig_pipe[0][0] != -1)
    {
        for (unsigned int i = 0; i < NUM_PIPES; i++)
        {
            close(sig_pipe[i][0]);
            close(sig_pipe[i][1]);
            efl_del(sig_pipe_handler[i]);
        }
    }
    memset(sig_pipe, -1, sizeof(sig_pipe));
    eina_spinlock_free(&sig_pid_lock);
}

static void
_Core_signal_cb_fork(void *data EFL_UNUSED)
{
    _Core_signal_pipe_shutdown();
    _Core_signal_pipe_init();
}

void
_core_signal_init(void)
{
    pipe_dead = 0;
    _Core_signal_pipe_init();
    core_fork_reset_callback_add(_Core_signal_cb_fork, NULL);
}

void
_core_signal_shutdown(void)
{
    sigset_t newset;

    core_fork_reset_callback_del(_Core_signal_cb_fork, NULL);
    pipe_dead = 1;
    // we probably should restore.. but not a good idea
    // pthread_sigmask(SIG_SETMASK, &sig_oldset, NULL);
    // at least do not trigger signal callback after shutdown
#ifndef _WIN32
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
#  ifdef SIGPWR
    sigaddset(&newset, SIGPWR);
#  endif
    pthread_sigmask(SIG_BLOCK, &newset, NULL);
#endif
    _Core_signal_pipe_shutdown();
    exit_signal_received = 0;
}

void
_core_signal_received_process(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd EFL_UNUSED)
{
    // do nothing - the efl loop handler read event will handle it
}

int
_core_signal_count_get(Eo *obj EFL_UNUSED, Efl_Loop_Data *pd EFL_UNUSED)
{
    // we will always have 0 signals be3cause they will instead be read from
    // a pipe fd and placed in a queue/list that
    // _core_signal_received_process() will then walk and process/do
    return 0;
}

void
_core_signal_pid_lock(void)
{
    eina_spinlock_take(&sig_pid_lock);
}

void
_core_signal_pid_unlock(void)
{
    eina_spinlock_release(&sig_pid_lock);
}

void
_core_signal_pid_register(pid_t pid, int fd)
{
    Pid_Info *pi = calloc(1, sizeof(Pid_Info));
    if (!pi) return;
    pi->pid           = pid;
    pi->fd            = fd;
    sig_pid_info_list = eina_list_append(sig_pid_info_list, pi);
}

void
_core_signal_pid_unregister(pid_t pid, int fd)
{
    Eina_List *l;
    Pid_Info  *pi;

    EINA_LIST_FOREACH(sig_pid_info_list, l, pi)
    {
        if ((pi->pid == pid) && (pi->fd == fd))
        {
            sig_pid_info_list = eina_list_remove_list(sig_pid_info_list, l);
            free(pi);
            return;
        }
    }
}

static void
_Core_signal_exe_exit_delay(void *data, const Efl_Event *event)
{
    Core_Exe_Event_Del *e = data;

    if (!e) return;
    _core_exe_doomsday_clock_set(e->exe, NULL);
    core_event_add(CORE_EXE_EVENT_DEL, e, _core_exe_event_del_free, NULL);
    efl_del(event->object);
}

static void
_Core_signal_waitpid(Efl_Bool once, siginfo_t info)
{
    pid_t pid;
    int   status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        Core_Exe_Event_Del *e = _core_exe_event_del_new();

        //FIXME: If this process is set respawn, respawn with a suitable backoff
        // period for those that need too much respawning.
        if (e)
        {
            if (WIFEXITED(status))
            {
                e->exit_code = WEXITSTATUS(status);
                e->exited    = 1;
            }
            else if (WIFSIGNALED(status))
            {
                e->exit_signal = WTERMSIG(status);
                e->signalled   = 1;
            }
            e->pid  = pid;
            e->exe  = _core_exe_find(pid);
            e->data = info;  // No need to clone this.
            if ((e->exe) && (core_exe_flags_get(e->exe) &
                             (CORE_EXE_PIPE_READ | CORE_EXE_PIPE_ERROR)))
            {
                /* We want to report the Last Words of the exe, so delay this event.
                   * This is twice as relevant for stderr.
                   * There are three possibilities here -
                   *  1 There are no Last Words.
                   *  2 There are Last Words, they are not ready to be read.
                   *  3 There are Last Words, they are ready to be read.
                   *
                   * For 1 we don't want to delay, for 3 we want to delay.
                   * 2 is the problem.  If we check for data now and there
                   * is none, then there is no way to differentiate 1 and 2.
                   * If we don't delay, we may loose data, but if we do delay,
                   * there may not be data and the exit event never gets sent.
                   *
                   * Any way you look at it, there has to be some time passed
                   * before the exit event gets sent.  So the strategy here is
                   * to setup a timer event that will send the exit event after
                   * an arbitrary, but brief, time.
                   *
                   * This is probably paranoid, for the less paraniod, we could
                   * check to see for Last Words, and only delay if there are any.
                   * This has it's own set of problems. */
                efl_del(_core_exe_doomsday_clock_get(e->exe));

                Efl_Loop_Timer *doomsday_clock = efl_add(
                    EFL_LOOP_TIMER_CLASS,
                    ML_OBJ,
                    efl_loop_timer_interval_set(efl_added, 0.1),
                    efl_event_callback_add(efl_added,
                                           EFL_LOOP_TIMER_EVENT_TIMER_TICK,
                                           _Core_signal_exe_exit_delay,
                                           e));
                _core_exe_doomsday_clock_set(e->exe, doomsday_clock);
            }
            else
                core_event_add(CORE_EXE_EVENT_DEL,
                                e,
                                _core_exe_event_del_free,
                                NULL);
        }

        // XXX: this is not brilliant. this ends up running from the main loop
        // reading the signal pipe to handle signals. that means handling
        // exe exits from children will be bottlenecked by how often
        // the main loop can wake up (or well latency may not be great).
        // this should probably have a dedicated thread ythat does a waitpid()
        // and blocks and waits sending results to the resulting pipe
        Eina_List *l, *ll;
        Pid_Info  *pi;

        EINA_LIST_FOREACH_SAFE(sig_pid_info_list, l, ll, pi)
        {
            if (pi->pid == pid)
            {
                Core_Signal_Pid_Info pinfo;

                sig_pid_info_list =
                    eina_list_remove_list(sig_pid_info_list, ll);
                pinfo.pid  = pid;
                pinfo.info = info;
                if (WIFEXITED(status))
                {
                    pinfo.exit_code   = WEXITSTATUS(status);
                    pinfo.exit_signal = -1;
                }
                else if (WIFSIGNALED(status))
                {
                    pinfo.exit_code   = -1;
                    pinfo.exit_signal = WTERMSIG(status);
                }
                if (write(pi->fd, &pinfo, sizeof(Core_Signal_Pid_Info)) !=
                    sizeof(Core_Signal_Pid_Info))
                {
                    ERR("Can't write to custom exe exit info pipe");
                }
                free(pi);
                break;
            }
        }
        if (once) break;
    }
}

static void
_Core_signal_generic_free(void *data EFL_UNUSED, void *event)
{
    free(event);
}
