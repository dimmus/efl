#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <signal.h>

#ifdef HAVE_PRCTL
#  include <sys/prctl.h>
#elif defined(HAVE_PROCCTL)
#  include <sys/procctl.h>
#endif

#ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
#endif

#include <Efl_Eo_Interfaces.h>

#include "Efl_Core.h"
#include "core_private.h"

#define MY_CLASS CORE_EXE_CLASS

#include "core_exe_private.h"

/* TODO: Something to let people build a command line and does auto escaping -
 *
 * core_exe_snprintf()
 *
 *   OR
 *
 * cmd = core_exe_comand_parameter_append(cmd, "firefox");
 * cmd = core_exe_comand_parameter_append(cmd, "http://www.foo.com/bar.html?baz=yes");
 * each parameter appended is one argument, and it gets escaped, quoted, and
 * appended with a preceding space.  The first is the command off course.
 */

static inline void _core_exe_exec_it(const char *exe_cmd, Core_Exe_Flags flags);
static Efl_Bool    _core_exe_data_generic_handler(void            *data,
                                                  Core_Fd_Handler *fd_handler,
                                                  Core_Exe_Flags   flags);
static Efl_Bool    _core_exe_data_error_handler(void            *data,
                                                Core_Fd_Handler *fd_handler);
static Efl_Bool    _core_exe_data_read_handler(void            *data,
                                               Core_Fd_Handler *fd_handler);
static Efl_Bool    _core_exe_data_write_handler(void            *data,
                                                Core_Fd_Handler *fd_handler);
static void        _core_exe_flush(Core_Exe *obj);
static void _core_exe_make_sure_its_dead(void *data, const Efl_Event *event);
static void _core_exe_make_sure_its_really_dead(void            *data,
                                                const Efl_Event *event);
static void _core_exe_dead_attach(Core_Exe *obj);

static const char *shell = NULL;

/* FIXME: This errno checking stuff should be put elsewhere for everybody to use.
 * For now it lives here though, just to make testing easier.
 */
static int _core_exe_check_errno(int result, const char *file, int line);

#define E_IF_NO_ERRNO(result, foo, ok)                     \
    while (((ok) = _core_exe_check_errno((result) = (foo), \
                                         __FILE__,         \
                                         __LINE__)) == -1) \
        sleep(1);                                          \
    if (ok)

#define E_NO_ERRNO(result, foo, ok)                        \
    while (((ok) = _core_exe_check_errno((result) = (foo), \
                                         __FILE__,         \
                                         __LINE__)) == -1) \
    sleep(1)

#define E_IF_NO_ERRNO_NOLOOP(result, foo, ok) \
    if (((ok) = _core_exe_check_errno((result) = (foo), __FILE__, __LINE__)))

static int
_core_exe_check_errno(int              result,
                      const char *file EFL_UNUSED,
                      int line         EFL_UNUSED)
{
  int saved_errno = errno;

  if (result == -1)
  {
    perror("*** errno reports ");
    /* What is currently supported -
 *
 *   pipe
 *     EFAULT  Argument is not valid.
 *     EMFILE  Too many file descriptors used by process.
 *     ENFILE  Too many open files by system.
 *   read
 *     EAGAIN  No data now, try again.
 *     EBADF   This is not an fd that can be read.
 *     EFAULT  This is not a valid buffer.
 *     EINTR   Interupted by signal, try again.
 *     EINVAL  This is not an fd that can be read.
 *     EIO     I/O error.
 *     EISDIR  This is a directory, and cannot be read.
 *     others  Depending on what sort of thing we are reading from.
 *   close
 *     EBADF   This is not an fd that can be closed.
 *     EINTR   Interupted by signal, try again.
 *     EIO     I/O error.
 *   dup2
 *     EBADF   This is not an fd that can be dup2'ed.
 *     EBUSY   Race condition between open() and dup()
 *     EINTR   Interupted by signal, try again.
 *     EMFILE  Too many file descriptors used by process.
 *   fcntl
 *     EACCES, EAGAIN  Locked or mapped by something else, try again later.
 *     EBADF   This is not an fd that can be fcntl'ed.
 *     EDEADLK This will cause a deadlock.
 *     EFAULT  This is not a valid lock.
 *     EINTR   Interupted by signal, try again.
 *     EINVAL  This is not a valid arg.
 *     EMFILE  Too many file descriptors used by process.
 *     ENOLCK  Problem getting a lock.
 *     EPERM   Not allowed to do that.
 *   fsync
 *     EBADF   This is not an fd that is open for writing.
 *     EINVAL, EROFS  This is not an fd that can be fsynced.
 *     EIO     I/O error.
 *
 * How to use it -
 *    int ok = 0;
 *    int result;
 *
 *    E_IF_NO_ERRNO(result, foo(bar), ok)
 *      {
 *         E_IF_NO_ERRNO_NOLOOP(result, foo(bar), ok)
 *            {
 *            }
 *      }
 *
 *   if (!ok)
 *     {
 *        // Something failed, cleanup.
 *     }
 */
    switch (saved_errno)
    {
      case EACCES:
      case EAGAIN:
      case EINTR:
        { /* Not now, try later. */
          ERR("*** Must try again in %s @%u.", file, line);
          result = -1;
          break;
        }

      case EMFILE:
      case ENFILE:
      case ENOLCK:
        { /* Low on resources. */
          ERR("*** Low on resources in %s @%u.", file, line);
          result = 0;
          break;
        }

      case EIO:
        { /* I/O error. */
          ERR("*** I/O error in %s @%u.", file, line);
          result = 0;
          break;
        }

      case EFAULT:
      case EBADF:
      case EINVAL:
      case EROFS:
      case EISDIR:
      case EDEADLK:
      case EPERM:
      case EBUSY:
        { /* Programmer fucked up. */
          ERR("*** Race condition between open() and dup() created "
              "in %s @%u.",
              file,
              line);
          result = 0;
          break;
        }

      default:
        { /* Unsupported errno code, please add this one. */
          ERR("*** Unsupported errno code %d, found in %s @%u, from "
              "%s @%u.",
              saved_errno,
              __FILE__,
              __LINE__,
              file,
              line);
          result = 0;
          break;
        }
    }
  }
  else /* Everything is fine. */ result = 1;

  errno = saved_errno;
  return result;
}

static int run_pri = CORE_EXE_PRIORITY_INHERIT;

void
_impl_core_exe_run_priority_set(int pri)
{
  run_pri = pri;
}

int
_impl_core_exe_run_priority_get(void)
{
  return run_pri;
}

#if defined(__FreeBSD__) || defined(__OpenBSD__)
#  include <dlfcn.h>
static char ***_dl_environ;
#else
extern char **environ;
#endif

Eo *
_impl_core_exe_efl_object_finalize(Eo *obj, Core_Exe_Data *exe)
{
  int statusPipe[2] = { -1, -1 };
  int errorPipe[2]  = { -1, -1 };
  int readPipe[2]   = { -1, -1 };
  int writePipe[2]  = { -1, -1 };
  int n             = 0;
  int ok            = 1;
  int result;

  if (!exe->cmd) return NULL;

  const char    *exe_cmd = exe->cmd;
  Core_Exe_Flags flags   = exe->flags;

  if ((flags & CORE_EXE_PIPE_AUTO) && (!(flags & CORE_EXE_PIPE_ERROR)) &&
      (!(flags & CORE_EXE_PIPE_READ)))
    /* We need something to auto pipe. */
    flags |= CORE_EXE_PIPE_READ | CORE_EXE_PIPE_ERROR;

  exe->child_fd_error   = -1;
  exe->child_fd_read    = -1;
  exe->child_fd_write   = -1;
  exe->child_fd_error_x = -1;
  exe->child_fd_read_x  = -1;
  exe->child_fd_write_x = -1;

  /*  Create some pipes. */
  if (ok)
  {
    E_IF_NO_ERRNO_NOLOOP(result, pipe(statusPipe), ok)
    {}
  }
  if (ok && (flags & CORE_EXE_PIPE_ERROR))
  {
    E_IF_NO_ERRNO_NOLOOP(result, pipe(errorPipe), ok)
    {
      exe->child_fd_error   = errorPipe[0];
      exe->child_fd_error_x = errorPipe[1];
    }
  }
  if (ok && (flags & CORE_EXE_PIPE_READ))
  {
    E_IF_NO_ERRNO_NOLOOP(result, pipe(readPipe), ok)
    {
      exe->child_fd_read   = readPipe[0];
      exe->child_fd_read_x = readPipe[1];
    }
  }
  if (ok && (flags & CORE_EXE_PIPE_WRITE))
  {
    E_IF_NO_ERRNO_NOLOOP(result, pipe(writePipe), ok)
    {
      exe->child_fd_write   = writePipe[1];
      exe->child_fd_write_x = writePipe[0];
    }
  }
  if (ok)
  {
    pid_t        pid              = 0;
    volatile int vfork_exec_errno = 0;

    sigset_t newset, oldset;

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
    sigaddset(&newset, SIGBUS);
    sigaddset(&newset, SIGCONT);
    sigaddset(&newset, SIGWINCH);
#ifdef SIGEMT
    sigaddset(&newset, SIGEMT);
#endif
#ifdef SIGIO
    sigaddset(&newset, SIGIO);
#endif
#ifdef SIGTSTP
    sigaddset(&newset, SIGTSTP);
#endif
#ifdef SIGTTIN
    sigaddset(&newset, SIGTTIN);
#endif
#ifdef SIGTTOU
    sigaddset(&newset, SIGTTOU);
#endif
#ifdef SIGVTALRM
    sigaddset(&newset, SIGVTALRM);
#endif
#ifdef SIGPWR
    sigaddset(&newset, SIGPWR);
#endif
    // block all those nasty signals we don't want messing with things
    // in signal handlers while we go from fork to exec in the child
    pthread_sigmask(SIG_BLOCK, &newset, &oldset);
    /* FIXME: I should double check this.  After a quick look around, this is already done, but via a more modern method. */
    /* signal(SIGPIPE, SIG_IGN);    We only want EPIPE on errors */
    pid = fork();

    if (pid == -1)
    {
      pthread_sigmask(SIG_SETMASK, &oldset, NULL);
      ERR("Failed to fork process");
      pid = 0;
    }
    else if (pid == 0) /* child */
    {
      struct sigaction sa;
      int              sig;

      sa.sa_handler = SIG_DFL;
      sa.sa_flags   = 0;
      sigemptyset(&sa.sa_mask);
      for (sig = 0; sig < 32; sig++)
        sigaction(sig, &sa, NULL);
#ifdef HAVE_SYSTEMD
      char **env = NULL, **e;

#  if defined(__FreeBSD__) || defined(__OpenBSD__)
      _dl_environ = dlsym(NULL, "environ");
      env         = *_dl_environ;
#  else
      env = environ;
#  endif
      // find NOTIFY_SOCKET env var and remove it without any heap work
      if (env)
      {
        Efl_Bool shuffle = EFL_FALSE;

        for (e = env; *e; e++)
        {
          if (!shuffle)
          {
            if (!strncmp(e[0], "NOTIFY_SOCKET=", 14)) shuffle = EFL_TRUE;
          }
          if (shuffle) e[0] = e[1];
        }
      }
#endif
      if (run_pri != CORE_EXE_PRIORITY_INHERIT)
      {
#ifdef PRIO_PROCESS
        if ((run_pri >= -20) && (run_pri <= 19))
          setpriority(PRIO_PROCESS, 0, run_pri);
#else
#  warning \
    "Your OS/libc does not provide PRIO_PROCESS (and possibly setpriority())"
#  warning \
    "This is a POSIX-1.2001 standard and it is highly encouraged that you"
#  warning "Have support for this"
#endif
      }
      if (ok && (flags & CORE_EXE_ISOLATE_IO))
      {
        int devnull;

        /* we want to isolatie the stdin/out/err of the process so
               * it can't share those of the parent, so close and replace with
               * /dev/null */
        devnull = open("/dev/null", O_RDONLY);
        if (devnull >= 0)
        {
          E_NO_ERRNO(result, close(STDIN_FILENO), ok);
          E_NO_ERRNO(result, dup2(devnull, STDIN_FILENO), ok);
          E_NO_ERRNO(result, close(devnull), ok);
        }

        devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0)
        {
          E_NO_ERRNO(result, close(STDOUT_FILENO), ok);
          E_NO_ERRNO(result, dup2(devnull, STDOUT_FILENO), ok);
          E_NO_ERRNO(result, close(devnull), ok);
        }

        devnull = open("/dev/null", O_WRONLY);
        if (devnull >= 0)
        {
          E_NO_ERRNO(result, close(STDERR_FILENO), ok);
          E_NO_ERRNO(result, dup2(devnull, STDERR_FILENO), ok);
          E_NO_ERRNO(result, close(devnull), ok);
        }
      }
      else
      {
        /* dup2 STDERR, STDIN, and STDOUT.  dup2() allegedly closes the
               * second pipe if it's open. On the other hand, there was the
               * Great FD Leak Scare of '06, so let's be paranoid. */
        if (ok && (flags & CORE_EXE_PIPE_ERROR))
        {
          E_NO_ERRNO(result, close(STDERR_FILENO), ok);
          E_NO_ERRNO(result, dup2(errorPipe[1], STDERR_FILENO), ok);
        }
        if (ok && (flags & CORE_EXE_PIPE_READ))
        {
          E_NO_ERRNO(result, close(STDOUT_FILENO), ok);
          E_NO_ERRNO(result, dup2(readPipe[1], STDOUT_FILENO), ok);
        }
        if (ok && (flags & CORE_EXE_PIPE_WRITE))
        {
          E_NO_ERRNO(result, close(STDIN_FILENO), ok);
          E_NO_ERRNO(result, dup2(writePipe[0], STDIN_FILENO), ok);
        }
      }

      if (ok)
      {
        /* Setup the status pipe. */
        E_NO_ERRNO(result, close(statusPipe[0]), ok);
        E_IF_NO_ERRNO(result,
                      eina_file_close_on_exec(statusPipe[1], EFL_TRUE),
                      ok) /* close on exec shows success */
        {
          int except[2] = { 0, -1 };

          except[0] = statusPipe[1];
          eina_file_close_from(3, except);
          /* Run the actual command. */
          pthread_sigmask(SIG_SETMASK, &oldset, NULL);
          _core_exe_exec_it(exe_cmd, flags); /* no return */
        }
      }
      _exit(-1);
    }
    else /* parent */
    {
      pthread_sigmask(SIG_SETMASK, &oldset, NULL);
      /* Close the unused pipes. */
      E_NO_ERRNO(result, close(statusPipe[1]), ok);

      /* FIXME: after having a good look at the current e fd
           * handling, investigate fcntl(dataPipe[x], F_SETSIG, ...) */
      /* FIXME: above F_SETSIG etc. - this is async SIGIO based IO
           * which is also linux specific so we probably don't want to
           * do this as long as select() is working fine. the only time
           * we really want to think of SIGIO async IO is when it all
           * actually works basically everywhere and we can turn all
           * IO into DMA async activities (i.e. you do a read() then
           * the read is complete not on return but when you get a
           * SIGIO - the read() just starts the transfer and it is
           * completed in the background by DMA (or whatever mechanism
           * the kernel choses)) */

      /* Wait for it to start executing. */
      /* FIXME: this doesn't seem very nice - we sit and block
           * waiting on a child process... even though it's just
           * the segment between the fork() and the exec) it just feels
           * wrong */
      for (;;)
      {
        char buf;

        E_NO_ERRNO(result, read(statusPipe[0], &buf, 1), ok);
        if (result == 0)
        {
          if (vfork_exec_errno != 0)
          {
            n = vfork_exec_errno;
            ERR("Could not start \"%s\"", exe_cmd);
            pid = 0;
          }
          break;
        }
      }

      /* Close the status pipe. */
      E_NO_ERRNO(result, close(statusPipe[0]), ok);
    }

    if (pid)
    {
      /* Setup the exe structure. */
      exe->start_bytes = -1;
      exe->end_bytes   = -1;
      exe->start_lines = -1;
      exe->end_lines   = -1;
      exe->pid         = pid;
      exe->flags       = flags;
      if (exe->cmd)
      {
        if (flags & CORE_EXE_PIPE_ERROR) /* Setup the error stuff. */
        {
          E_IF_NO_ERRNO(result,
                        fcntl(exe->child_fd_error, F_SETFL, O_NONBLOCK),
                        ok)
          {}
          E_IF_NO_ERRNO(result,
                        eina_file_close_on_exec(exe->child_fd_error, EFL_TRUE),
                        ok)
          {}
          E_IF_NO_ERRNO(
            result,
            eina_file_close_on_exec(exe->child_fd_error_x, EFL_TRUE),
            ok)
          {}
          {
            // XXX: eoify ecore fd handlers
            exe->error_fd_handler =
              core_main_fd_handler_add(exe->child_fd_error,
                                       CORE_FD_READ,
                                       _core_exe_data_error_handler,
                                       obj,
                                       NULL,
                                       NULL);
            if (!exe->error_fd_handler) ok = 0;
          }
        }
        if (ok && (flags & CORE_EXE_PIPE_READ)) /* Setup the read stuff. */
        {
          E_IF_NO_ERRNO(result,
                        fcntl(exe->child_fd_read, F_SETFL, O_NONBLOCK),
                        ok)
          {}
          E_IF_NO_ERRNO(result,
                        eina_file_close_on_exec(exe->child_fd_read, EFL_TRUE),
                        ok)
          {}
          E_IF_NO_ERRNO(result,
                        eina_file_close_on_exec(exe->child_fd_read_x, EFL_TRUE),
                        ok)
          {}
          {
            // XXX: eoify ecore fd handlers
            exe->read_fd_handler =
              core_main_fd_handler_add(exe->child_fd_read,
                                       CORE_FD_READ,
                                       _core_exe_data_read_handler,
                                       obj,
                                       NULL,
                                       NULL);
            if (!exe->read_fd_handler) ok = 0;
          }
        }
        if (ok && (flags & CORE_EXE_PIPE_WRITE)) /* Setup the write stuff. */
        {
          E_IF_NO_ERRNO(result,
                        fcntl(exe->child_fd_write, F_SETFL, O_NONBLOCK),
                        ok)
          {}
          E_IF_NO_ERRNO(result,
                        eina_file_close_on_exec(exe->child_fd_write, EFL_TRUE),
                        ok)
          {}
          E_IF_NO_ERRNO(
            result,
            eina_file_close_on_exec(exe->child_fd_write_x, EFL_TRUE),
            ok)
          {}
          {
            // XXX: eoify ecore fd handlers
            exe->write_fd_handler =
              core_main_fd_handler_add(exe->child_fd_write,
                                       CORE_FD_WRITE,
                                       _core_exe_data_write_handler,
                                       obj,
                                       NULL,
                                       NULL);
            // XXX: eoify ecore fd handlers
            if (exe->write_fd_handler)
              core_main_fd_handler_active_set(
                exe->write_fd_handler,
                0); /* Nothing to write to start with. */
            else ok = 0;
          }
        }

        exe->loop           = efl_provider_find(obj, EFL_LOOP_CLASS);
        Efl_Loop_Data *loop = efl_data_scope_get(exe->loop, EFL_LOOP_CLASS);
        if (loop) loop->exes = eina_list_append(loop->exes, obj);
        n = 0;
      }
      else ok = 0;
    }
    else ok = 0;
  }

  if (!ok) /* Something went wrong, so pull down everything. */
  {
    // XXX: eoify terminate method
    if (exe->pid) core_exe_terminate(obj);
    obj = NULL;
  }
  else
  {
    Core_Exe_Event_Add *e;

    e = _core_exe_event_add_new();
    if (e)
    {
      e->exe = obj;
      // XXX: eoify core_event_add
      /* Send the event. */
      core_event_add(CORE_EXE_EVENT_ADD, e, _core_exe_event_add_free, NULL);
    }
    /* INF("Running as %d for %s.\n", exe->pid, exe->cmd); */
  }

  errno = n;
  return obj;
}

Efl_Bool
_impl_core_exe_send(Core_Exe *obj  EFL_UNUSED,
                    Core_Exe_Data *exe,
                    const void    *data,
                    int            size)
{
  void *buf;

  if (exe->child_fd_write == -1)
  {
    ERR("Core_Exe %p created without CORE_EXE_PIPE_WRITE! "
        "Cannot send %d bytes from %p",
        exe,
        size,
        data);
    return EFL_FALSE;
  }

  buf = realloc(exe->write_data_buf, exe->write_data_size + size);
  if (!buf) return EFL_FALSE;

  exe->write_data_buf = buf;
  memcpy((char *)exe->write_data_buf + exe->write_data_size, data, size);
  exe->write_data_size += size;

  // XXX: eoify core_fd_handlers
  if (exe->write_fd_handler)
    core_main_fd_handler_active_set(exe->write_fd_handler, CORE_FD_WRITE);

  return EFL_TRUE;
}

void
_impl_core_exe_auto_limits_set(Core_Exe *obj  EFL_UNUSED,
                               Core_Exe_Data *exe,
                               int            start_bytes,
                               int            end_bytes,
                               int            start_lines,
                               int            end_lines)
{
  /* FIXME: sanitize the input. */
  exe->start_bytes = start_bytes;
  exe->end_bytes   = end_bytes;
  exe->start_lines = start_lines;
  exe->end_lines   = end_lines;

  /* FIXME: get this can of worms working.
    *
    * capture stderr & stdout internally
    *
    * raster and onefang keep moving the goal posts on this one.  It started out as
    * "show users the error output if an exe fails" and is rapidly approaching
    * "alternative method of getting the data, poll vs event driven".  Some serious
    * thinking needs to be applied to this.  Do we really want to go that far?  If
    * so, we should change the names.  The basic design will probably remain the
    * same which ever way we go.  The constant goal post moving is probably due to
    * generic design methods leading to feature creep as we inspired each other to
    * more generic designs.  It does seem like the closer we get to poll driven,
    * the more issues and corner cases there are.
    *
    * Instead of doing the usual register an event handler thing, we are core_exe,
    * we can take some short cuts.  Don't send the events, just leave the exe buffers
    * as is until the user asks for them, then return the event.
    *
    * start = 0,  end = 0;   clogged arteries get flushed, everything is ignored.
    * start = -1, end = -1;  clogged arteries get transferred to internal buffers.  Actually, either == -1 means buffer everything.
    * start = X,  end = 0;   buffer first X out of clogged arteries, flush and ignore rest.
    * start = 0,  end = X;   circular buffer X
    * start = X,  end = Y;   buffer first X out of clogged arteries, circular buffer Y from beginning.
    *
    * bytes vs lines, which ever one reaches the limit first.
    * Before we go beyond the start+end limit, leave the end buffer empty, and store both in the start buffer, coz they overlap.
    * After we pass the the start+end limit, insert "\n...\n" at the end of the start buffer, copy the rest to the end buffer, then store in the end buffer.
    *
    * Other issues -
    * Spank programmer for polling data if polling is not turned on.
    * Spank programmer for setting up event callbacks if polling is turned on.
    * Spank programmer for freeing the event data if it came from the event system, as that autofrees.
    * Spank the programmer if they try to set the limits bigger than what has been gathered & ignored already, coz they just lost data.
    * Spank onefang and raster for opening this can of worms.
    * Should we have separate out/err limits?
    * Should we remove from the internal buffer the data that was delivered already?
    * If so, what to do about limits, start, and end?  They could loose their meaning.
    */
}

Core_Exe_Event_Data *
_impl_core_exe_event_data_get(Core_Exe      *obj,
                              Core_Exe_Data *exe,
                              Core_Exe_Flags flags)
{
  Core_Exe_Event_Data *e           = NULL;
  int                  is_buffered = 0;
  unsigned char       *inbuf;
  int                  inbuf_num;

  /* Sort out what sort of event we are. */
  if (flags & CORE_EXE_PIPE_READ)
  {
    flags = CORE_EXE_PIPE_READ;
    if (exe->flags & CORE_EXE_PIPE_READ_LINE_BUFFERED) is_buffered = 1;
  }
  else
  {
    flags = CORE_EXE_PIPE_ERROR;
    if (exe->flags & CORE_EXE_PIPE_ERROR_LINE_BUFFERED) is_buffered = 1;
  }

  /* Get the data. */
  if (flags & CORE_EXE_PIPE_READ)
  {
    inbuf               = exe->read_data_buf;
    inbuf_num           = exe->read_data_size;
    exe->read_data_buf  = NULL;
    exe->read_data_size = 0;
  }
  else
  {
    inbuf                = exe->error_data_buf;
    inbuf_num            = exe->error_data_size;
    exe->error_data_buf  = NULL;
    exe->error_data_size = 0;
  }

  e = calloc(1, sizeof(Core_Exe_Event_Data));
  if (e)
  {
    e->exe  = obj;
    e->data = inbuf;
    e->size = inbuf_num;

    if (is_buffered) /* Deal with line buffering. */
    {
      int   max   = 0;
      int   count = 0;
      int   i;
      int   last = 0;
      char *c;

      c = (char *)inbuf;
      for (i = 0; i < inbuf_num; i++) /* Find the lines. */
      {
        if (inbuf[i] == '\n')
        {
          if (count >= max)
          {
            Core_Exe_Event_Data_Line *lines;

            /* In testing, the lines seem to arrive in batches of 500 to 1000 lines at most, roughly speaking. */
            max +=
              10; /* FIXME: Maybe keep track of the largest number of lines ever sent, and add half that many instead of 10. */
            lines =
              realloc(e->lines,
                      sizeof(Core_Exe_Event_Data_Line) *
                        (max + 1)); /* Allow room for the NULL termination. */
            if (!lines)
            {
              ERR("Out of memory adding exe data lines to event");
              break;
            }
            e->lines = lines;
          }
          /* raster said to leave the line endings as line endings, however -
                * This is line buffered mode, we are not dealing with binary here, but lines.
                * If we are not dealing with binary, we must be dealing with ASCII, unicode, or some other text format.
                * Thus the user is most likely gonna deal with this text as strings.
                * Thus the user is most likely gonna pass this data to str functions.
                * rasters way - the endings are always gonna be '\n';  onefangs way - they will always be '\0'
                * We are handing them the string length as a convenience.
                * Thus if they really want it in raw format, they can e->lines[i].line[e->lines[i].size - 1] = '\n'; easily enough.
                * In the default case, we can do this conversion quicker than the user can, as we already have the index and pointer.
                * Let's make it easy on them to use these as standard C strings.
                *
                * onefang is proud to announce that he has just set a new personal record for the
                * most over documentation of a simple assignment statement.  B-)
                */
          inbuf[i]             = '\0';
          e->lines[count].line = c;
          e->lines[count].size = i - last;
          last                 = i + 1;
          c                    = (char *)&inbuf[last];
          count++;
        }
      }
      if (i > last) /* Partial line left over, save it for next time. */
      {
        if (count != 0) e->size = last;
        if (flags & CORE_EXE_PIPE_READ)
        {
          exe->read_data_size = i - last;
          exe->read_data_buf  = malloc(exe->read_data_size);
          if (exe->read_data_buf)
            memcpy(exe->read_data_buf, c, exe->read_data_size);
          else
          {
            exe->read_data_size = 0;
            ERR("Out of memory in allocating exe pipe data");
          }
        }
        else
        {
          exe->error_data_size = i - last;
          exe->error_data_buf  = malloc(exe->error_data_size);
          if (exe->error_data_buf)
            memcpy(exe->error_data_buf, c, exe->error_data_size);
          else
          {
            exe->error_data_size = 0;
            ERR("Out of memory in allocating exe pipe data");
          }
        }
      }
      if (count == 0) /* No lines to send, cancel the event. */
      {
        _core_exe_event_exe_data_free(NULL, e);
        e = NULL;
      }
      else /* NULL terminate the array, so that people know where the end is. */
      {
        e->lines[count].line = NULL;
        e->lines[count].size = 0;
      }
    }
  }

  return e;
}

void
_impl_core_exe_efl_object_destructor(Eo *obj, Core_Exe_Data *exe)
{
  void *data;
  int   ok = 0;
  int   result;

  data = exe->data;

  if (exe->pre_free_cb) exe->pre_free_cb(data, obj);

  exe->doomsday_clock = NULL;

  // XXX: eoify ecore fd handlers
  IF_FN_DEL(core_main_fd_handler_del, exe->write_fd_handler);
  IF_FN_DEL(core_main_fd_handler_del, exe->read_fd_handler);
  IF_FN_DEL(core_main_fd_handler_del, exe->error_fd_handler);
  if (exe->child_fd_write_x != -1)
    E_NO_ERRNO(result, close(exe->child_fd_write_x), ok);
  if (exe->child_fd_read_x != -1)
    E_NO_ERRNO(result, close(exe->child_fd_read_x), ok);
  if (exe->child_fd_error_x != -1)
    E_NO_ERRNO(result, close(exe->child_fd_error_x), ok);
  if (exe->child_fd_write != -1)
    E_NO_ERRNO(result, close(exe->child_fd_write), ok);
  if (exe->child_fd_read != -1)
    E_NO_ERRNO(result, close(exe->child_fd_read), ok);
  if (exe->child_fd_error != -1)
    E_NO_ERRNO(result, close(exe->child_fd_error), ok);
  IF_FREE(exe->write_data_buf);
  IF_FREE(exe->read_data_buf);
  IF_FREE(exe->error_data_buf);
  IF_FREE(exe->cmd);

  Efl_Loop_Data *loop = efl_data_scope_get(exe->loop, EFL_LOOP_CLASS);
  if (loop) loop->exes = eina_list_remove(loop->exes, obj);
  IF_FREE(exe->tag);
}

void
_impl_core_exe_pause(Eo *obj EFL_UNUSED, Core_Exe_Data *exe)
{
  kill(exe->pid, SIGSTOP);
}

void
_impl_core_exe_continue(Eo *obj EFL_UNUSED, Core_Exe_Data *exe)
{
  kill(exe->pid, SIGCONT);
}

void
_impl_core_exe_interrupt(Core_Exe *obj, Core_Exe_Data *exe)
{
  _core_exe_dead_attach(obj);
  kill(exe->pid, SIGINT);
}

void
_impl_core_exe_quit(Core_Exe *obj, Core_Exe_Data *exe)
{
  _core_exe_dead_attach(obj);
  kill(exe->pid, SIGQUIT);
}

void
_impl_core_exe_terminate(Core_Exe *obj, Core_Exe_Data *exe)
{
  _core_exe_dead_attach(obj);
  INF("Sending TERM signal to %s (%d).", exe->cmd, exe->pid);
  kill(exe->pid, SIGTERM);
}

void
_impl_core_exe_kill(Core_Exe *obj EFL_UNUSED, Core_Exe_Data *exe)
{
  efl_del(exe->doomsday_clock);
  exe->doomsday_clock =
    efl_add(EFL_LOOP_TIMER_CLASS,
            obj,
            efl_event_callback_add(efl_added,
                                   EFL_LOOP_TIMER_EVENT_TIMER_TICK,
                                   _core_exe_make_sure_its_really_dead,
                                   obj),
            efl_loop_timer_interval_set(efl_added, 10.0));
  INF("Sending KILL signal to %s (%d).", exe->cmd, exe->pid);
  kill(exe->pid, SIGKILL);
}

void
_impl_core_exe_signal(Core_Exe *obj EFL_UNUSED, Core_Exe_Data *exe, int num)
{
  if (num == 1) kill(exe->pid, SIGUSR1);
  else if (num == 2) kill(exe->pid, SIGUSR2);
}

void
_impl_core_exe_hup(Core_Exe *obj EFL_UNUSED, Core_Exe_Data *exe)
{
  kill(exe->pid, SIGHUP);
}

static void
_core_exe_make_sure_its_dead(void *data, const Efl_Event *event)
{
  Eo            *exe_obj = data;
  Core_Exe_Data *exe     = efl_data_scope_get(exe_obj, MY_CLASS);
  if (exe)
  {
    if (exe->cmd)
      INF("Sending KILL signal to allegedly dead %s (%d).", exe->cmd, exe->pid);
    else INF("Sending KILL signal to allegedly dead PID %d.", exe->pid);
    exe->doomsday_clock =
      efl_add(EFL_LOOP_TIMER_CLASS,
              exe_obj,
              efl_event_callback_add(efl_added,
                                     EFL_LOOP_TIMER_EVENT_TIMER_TICK,
                                     _core_exe_make_sure_its_really_dead,
                                     exe_obj),
              efl_loop_timer_interval_set(efl_added, 10.0));
    kill(exe->pid, SIGKILL);
  }
  efl_del(event->object);
}

static void
_core_exe_make_sure_its_really_dead(void *data, const Efl_Event *event)
{
  Eo            *exe_obj = data;
  Core_Exe_Data *exe     = efl_data_scope_get(exe_obj, MY_CLASS);
  if (exe)
  {
    ERR("RUN!  The zombie wants to eat your brains!  And your CPU!");
    if (exe->cmd) INF("%s (%d) is not really dead.", exe->cmd, exe->pid);
    else INF("PID %d is not really dead.", exe->pid);
    exe->doomsday_clock = NULL;
  }
  efl_del(event->object);
}

Efl_Loop_Timer *
_core_exe_doomsday_clock_get(Core_Exe *obj)
{
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!exe) return NULL;
  return exe->doomsday_clock;
}

void
_core_exe_doomsday_clock_set(Core_Exe *obj, Efl_Loop_Timer *dc)
{
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!exe) return;
  exe->doomsday_clock = dc;
}

static inline void
_core_exe_exec_it(const char *exe_cmd, Core_Exe_Flags flags)
{
  char   use_sh     = 1;
  char  *buf        = NULL;
  char **args       = NULL;
  int    save_errno = 0;

  /* So what is this doing?
    *
    * We are trying to avoid wrapping the exe call with /bin/sh -c.
    * We conservatively search for certain shell meta characters,
    * If we don't find them, we can call the exe directly.
    */
  if (!strpbrk(exe_cmd, "|&;<>()$`\\\"'*?#"))
  {
    char *token;
    char  pre_command = 1;
    int   num_tokens  = 0;
    int   len;

    len = strlen(exe_cmd);
    buf = alloca(len + 1);
    strcpy(buf, exe_cmd);
    buf[len] = 0;

    token = strtok(buf, " \t\n\v");
    while (token)
    {
      if (token[0] == '~') break;
      if (pre_command)
      {
        if (token[0] == '[') break;
        if (strchr(token, '=')) break;
        else pre_command = 0;
      }
      num_tokens++;
      token = strtok(NULL, " \t\n\v");
    }
    if ((!token) && (num_tokens))
    {
      int i = 0;

      len = strlen(exe_cmd);
      buf = alloca(len + 1);
      strcpy(buf, exe_cmd);
      buf[len] = 0;

      token  = strtok(buf, " \t\n\v");
      use_sh = 0;
      args   = alloca((num_tokens + 1) * sizeof(char *));
      for (i = 0; i < num_tokens; i++)
      {
        if (token) args[i] = token;
        token = strtok(NULL, " \t\n\v");
      }
      args[num_tokens] = NULL;
    }
  }

#ifdef HAVE_PRCTL
  if ((flags & CORE_EXE_TERM_WITH_PARENT))
  {
    prctl(PR_SET_PDEATHSIG, SIGTERM);
  }
#elif defined(HAVE_PROCCTL)
  if ((flags & CORE_EXE_TERM_WITH_PARENT))
  {
    int sig = SIGTERM;
    procctl(P_PID, 0, PROC_PDEATHSIG_CTL, &sig);
  }
#endif

  if (!(flags & CORE_EXE_NOT_LEADER)) setsid();
  if ((flags & CORE_EXE_USE_SH))
  {
    errno = 0;
    execl("/bin/sh", "/bin/sh", "-c", exe_cmd, (char *)NULL);
  }
  else if (use_sh) /* We have to use a shell to run this. */
  {
    if (!shell) /* Find users preferred shell. */
    {
#if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
      if (getuid() == geteuid())
#endif
        shell = getenv("SHELL");
      if (!shell) shell = "/bin/sh";
    }
    errno = 0;
    execl(shell, shell, "-c", exe_cmd, (char *)NULL);
  }
  else
  { /* We can run this directly. */
    if (!args)
    {
      ERR("arg[0] is NULL!");
      return;
    }
    errno = 0;
    if (args[0]) execvp(args[0], args);
  }

  save_errno = errno;
  errno      = save_errno;
  return;
}

static Efl_Bool
_core_exe_data_generic_handler(void            *data,
                               Core_Fd_Handler *fd_handler,
                               Core_Exe_Flags   flags)
{
  Core_Exe                    *obj = data;
  int                          child_fd;
  int                          event_type;
  const Efl_Event_Description *eo_event = NULL;

  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!exe) return EFL_FALSE;

  /* Sort out what sort of handler we are. */
  if (flags & CORE_EXE_PIPE_READ)
  {
    flags      = CORE_EXE_PIPE_READ;
    event_type = CORE_EXE_EVENT_DATA;
    eo_event   = CORE_EXE_EVENT_DATA_GET;
    child_fd   = exe->child_fd_read;
  }
  else
  {
    flags      = CORE_EXE_PIPE_ERROR;
    event_type = CORE_EXE_EVENT_ERROR;
    eo_event   = CORE_EXE_EVENT_DATA_ERROR;
    child_fd   = exe->child_fd_error;
  }

  if ((fd_handler) &&
      (core_main_fd_handler_active_get(fd_handler, CORE_FD_READ)))
  {
    unsigned char *inbuf, *temp;
    int            inbuf_num;

    /* Get any left over data from last time. */
    if (flags & CORE_EXE_PIPE_READ)
    {
      inbuf               = exe->read_data_buf;
      inbuf_num           = exe->read_data_size;
      exe->read_data_buf  = NULL;
      exe->read_data_size = 0;
    }
    else
    {
      inbuf                = exe->error_data_buf;
      inbuf_num            = exe->error_data_size;
      exe->error_data_buf  = NULL;
      exe->error_data_size = 0;
    }

    for (;;)
    {
      int  num, lost_exe;
      char buf[READBUFSIZ];

      lost_exe = 0;
      errno    = 0;
      if ((num = read(child_fd, buf, READBUFSIZ)) < 1)
      {
        /* FIXME: SPEED/SIZE TRADE OFF - add a smaller READBUFSIZE
             * (currently 64k) to inbuf, use that instead of buf, and
             * save ourselves a memcpy(). */
        lost_exe = ((errno == EIO) || (errno == EBADF) || (errno == EPIPE) ||
                    (errno == EINVAL) || (errno == ENOSPC));
        if ((errno != EAGAIN) && (errno != EINTR))
          perror("_core_exe_generic_handler() read problem ");
      }
      if (num > 0) /* data got read. */
      {
        temp  = inbuf;
        inbuf = realloc(inbuf, inbuf_num + num);
        if (inbuf)
        {
          memcpy(inbuf + inbuf_num, buf, num);
          inbuf_num += num;
        }
        else  // realloc fails and returns NULL.
        {
          ERR("Out of memory in exe generic data handler");
          inbuf = temp;
        }
      }
      else
      { /* No more data to read. */
        if (inbuf)
        {
          Core_Exe_Event_Data *e;

          /* Stash the data away for later. */
          if (flags & CORE_EXE_PIPE_READ)
          {
            exe->read_data_buf  = inbuf;
            exe->read_data_size = inbuf_num;
          }
          else
          {
            exe->error_data_buf  = inbuf;
            exe->error_data_size = inbuf_num;
          }

          if (!(exe->flags & CORE_EXE_PIPE_AUTO))
          {
            // XXX: use eo method
            e = core_exe_event_data_get(obj, flags);
            if (e) /* Send the event. */
            {
              // XXX: eoify ecore event
              core_event_add(event_type,
                             e,
                             _core_exe_event_exe_data_free,
                             NULL);
              efl_event_callback_call(obj, eo_event, e);
            }
          }
        }
        if (lost_exe)
        {
          if (flags & CORE_EXE_PIPE_READ)
          {
            if (exe->read_data_size)
              INF("There are %d bytes left unsent from the dead "
                  "exe %s.",
                  exe->read_data_size,
                  exe->cmd);
          }
          else
          {
            if (exe->error_data_size)
              INF("There are %d bytes left unsent from the dead "
                  "exe %s.",
                  exe->error_data_size,
                  exe->cmd);
          }
          /* Thought about this a bit.  If the exe has actually
                * died, this won't do any harm as it must have died
                * recently and the pid has not had a chance to recycle.
                * It is also a paranoid catchall, coz the usual Core_signal
                * mechanism should kick in.  But let's give it a good
                * kick in the head anyway.
                */
          // XXX: use eo method
          core_exe_terminate(obj);
        }
        break;
      }
    }
  }

  return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_core_exe_data_error_handler(void *data, Core_Fd_Handler *fd_handler)
{
  return _core_exe_data_generic_handler(data, fd_handler, CORE_EXE_PIPE_ERROR);
}

static Efl_Bool
_core_exe_data_read_handler(void *data, Core_Fd_Handler *fd_handler)
{
  return _core_exe_data_generic_handler(data, fd_handler, CORE_EXE_PIPE_READ);
}

static Efl_Bool
_core_exe_data_write_handler(void *data, Core_Fd_Handler *fd_handler EFL_UNUSED)
{
  Core_Exe      *obj = data;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);

  if (!exe) return EFL_FALSE;

  // XXX: use eo method
  if ((exe->write_fd_handler) &&
      (core_main_fd_handler_active_get(exe->write_fd_handler, CORE_FD_WRITE)))
    _core_exe_flush(obj);

  /* If we have sent all there is to send, and we need to close the pipe, then close it. */
  if ((exe->close_stdin == 1) &&
      (exe->write_data_size == exe->write_data_offset))
  {
    int ok = 0;
    int result;

    INF("Closing stdin for %s", exe->cmd);
    /* if (exe->child_fd_write != -1)  E_NO_ERRNO(result, fsync(exe->child_fd_write), ok);   This a) doesn't work, and b) isn't needed. */
    IF_FN_DEL(core_main_fd_handler_del, exe->write_fd_handler);
    if (exe->child_fd_write != -1)
      E_NO_ERRNO(result, close(exe->child_fd_write), ok);
    exe->child_fd_write = -1;
    IF_FREE(exe->write_data_buf);
  }

  return CORE_CALLBACK_RENEW;
}

static void
_core_exe_flush(Core_Exe *obj)
{
  int            count;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);

  if (!exe) return;
  /* check whether we need to write anything at all. */
  if ((exe->child_fd_write == -1) || (!exe->write_data_buf)) return;
  if (exe->write_data_size == exe->write_data_offset) return;

  count = write(exe->child_fd_write,
                (char *)exe->write_data_buf + exe->write_data_offset,
                exe->write_data_size - exe->write_data_offset);
  if (count < 1)
  {
    if (errno == EIO || errno == EBADF || errno == EPIPE || errno == EINVAL ||
        errno == ENOSPC) /* we lost our exe! */
    {
      // XXX: use eo method (eoify)
      core_exe_terminate(obj);
      // XXX: eoify fd handlers
      if (exe->write_fd_handler)
        core_main_fd_handler_active_set(exe->write_fd_handler, 0);
    }
  }
  else
  {
    exe->write_data_offset += count;
    if (exe->write_data_offset >=
        exe->write_data_size) /* Nothing left to write, clean up. */
    {
      exe->write_data_size   = 0;
      exe->write_data_offset = 0;
      IF_FREE(exe->write_data_buf);
      // XXX: eoify fd handlers
      if (exe->write_fd_handler)
        core_main_fd_handler_active_set(exe->write_fd_handler, 0);
    }
  }
}

static void
_core_exe_dead_attach(Core_Exe *obj)
{
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);

  if (!exe) return;
  if (exe->doomsday_clock) return;
  exe->doomsday_clock =
    efl_add(EFL_LOOP_TIMER_CLASS,
            obj,
            efl_event_callback_add(efl_added,
                                   EFL_LOOP_TIMER_EVENT_TIMER_TICK,
                                   _core_exe_make_sure_its_dead,
                                   obj),
            efl_loop_timer_interval_set(efl_added, 10.0));
}
