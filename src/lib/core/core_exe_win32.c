#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <process.h>

#include <evil_private.h> /* evil_last_error_get */

#include "Efl_Core.h"
#include "core_private.h"

#include "core_exe_private.h"

/*
 * TESTS
 *
 * [X] add event
 * [X] data event
 * [X] error event
 * [X] data event buffered
 * [X] del event
 * [ ] batch files
 * [X] exit code
 * [X] inherited env var
 */

/*
 * FIXME :
 *
 * [ ] child program with ecore main loop does not exit and nothing is sent
 * [X] core_exe_send fails (race condition ? same problem as above ?)
 */

#define CORE_EXE_WIN32_TIMEOUT 3000

static int run_pri = NORMAL_PRIORITY_CLASS;

static void
_core_exe_threads_terminate(Core_Exe *obj)
{
  Core_Exe_Data *exe = efl_data_scope_get(obj, CORE_EXE_CLASS);

  if (!exe) return;
  if (!exe->th) return;
  core_thread_cancel(exe->th);
  core_thread_wait(exe->th, 0.3);
  exe->th = NULL;
}

static Efl_Bool
_core_exe_close_cb(void *data, Core_Win32_Handler *wh EFL_UNUSED)
{
  Core_Exe_Event_Del *e;
  Core_Exe           *obj       = data;
  Core_Exe_Data      *exe       = efl_data_scope_get(obj, CORE_EXE_CLASS);
  DWORD               exit_code = 0;

  if (!exe) return 0;

  e = calloc(1, sizeof(Core_Exe_Event_Del));
  if (!e) return 0;

   /* FIXME : manage the STILL_ACTIVE returned error */
  if (!GetExitCodeProcess(exe->process, &exit_code))
    DBG("%s", evil_last_error_get());

  e->exit_code = exit_code;
  e->exited    = 1;
  e->pid       = exe->pid;
  e->exe       = obj;
  exe->h_close = NULL; // It's going to get deleted in the next callback.

  core_event_add(CORE_EXE_EVENT_DEL, e, _core_exe_event_del_free, NULL);

  DBG("Exiting process %s with exit code %d\n", exe->cmd, e->exit_code);
  return 0;
}

typedef struct
{
  Core_Exe *obj;
  HANDLE    read_pipe;
  HANDLE    error_pipe;
  Efl_Bool  read  : 1;
  Efl_Bool  error : 1;
} Threaddata;

typedef struct
{
  Core_Exe      *obj;
  unsigned char *buf;
  int            buf_size;
  Efl_Bool       read  : 1;
  Efl_Bool       error : 1;
} Threadreply;

static void
_core_exe_win32_io_poll_thread(void *data, Core_Thread *th)
{
  Threaddata  *tdat = data;
  Threadreply *trep;
  Efl_Bool     data_read, data_error;
  char         buf[4096];
  DWORD        size, current_size;
  BOOL         res;

  while (EFL_TRUE)
  {
    data_read  = EFL_FALSE;
    data_error = EFL_FALSE;

    if (tdat->read)
    {
      res = PeekNamedPipe(tdat->read_pipe,
                          buf,
                          sizeof(buf),
                          &size,
                          &current_size,
                          NULL);
      if (res && (size != 0))
      {
        trep = calloc(1, sizeof(Threadreply));
        if (trep)
        {
          trep->obj = tdat->obj;
          trep->buf = malloc(current_size);
          if (trep->buf)
          {
            res =
              ReadFile(tdat->read_pipe, trep->buf, current_size, &size, NULL);
            if (!res || (size == 0))
            {
              free(trep->buf);
              free(trep);
            }
            trep->buf_size = size;
            trep->read     = EFL_TRUE;
            core_thread_feedback(th, trep);
            data_read = EFL_TRUE;
          }
        }
      }
    }
    if (tdat->error)
    {
      res = PeekNamedPipe(tdat->error_pipe,
                          buf,
                          sizeof(buf),
                          &size,
                          &current_size,
                          NULL);
      if (res && (size != 0))
      {
        trep = calloc(1, sizeof(Threadreply));
        if (trep)
        {
          trep->obj = tdat->obj;
          trep->buf = malloc(current_size);
          if (trep->buf)
          {
            res =
              ReadFile(tdat->error_pipe, trep->buf, current_size, &size, NULL);
            if (!res || (size == 0))
            {
              free(trep->buf);
              free(trep);
            }
            trep->buf_size = size;
            trep->error    = EFL_TRUE;
            core_thread_feedback(th, trep);
            data_error = EFL_TRUE;
          }
        }
      }
    }
    if (core_thread_check(th)) break;
    if (!(data_read || data_error)) Sleep(100);
    else if (core_thread_check(th)) break;
  }
  free(tdat);
}

static void
_core_exe_win32_io_poll_notify(void *data      EFL_UNUSED,
                               Core_Thread *th EFL_UNUSED,
                               void           *msg)
{
  Threadreply   *trep = msg;
  Core_Exe      *obj  = trep->obj;
  Core_Exe_Data *exe  = efl_data_scope_get(obj, CORE_EXE_CLASS);
  unsigned char *b;

  if (exe)
  {
    Core_Exe_Event_Data *event_data;

    if (trep->read)
    {
      if (!exe->pipe_read.data_buf)
      {
        exe->pipe_read.data_buf  = trep->buf;
        exe->pipe_read.data_size = trep->buf_size;
        trep->buf                = NULL;
      }
      else
      {
        b = realloc(exe->pipe_read.data_buf,
                    exe->pipe_read.data_size + trep->buf_size);
        if (b)
        {
          memcpy(b + exe->pipe_read.data_size, trep->buf, trep->buf_size);
          exe->pipe_read.data_buf   = b;
          exe->pipe_read.data_size += trep->buf_size;
        }
        else ERR("Out of memory in reading exe pipe data");
      }
      event_data = core_exe_event_data_get(obj, CORE_EXE_PIPE_READ);
      if (event_data)
      {
        core_event_add(CORE_EXE_EVENT_DATA,
                       event_data,
                       _core_exe_event_exe_data_free,
                       NULL);
        efl_event_callback_call(obj, CORE_EXE_EVENT_DATA_GET, event_data);
      }
    }
    else if (trep->error)
    {
      if (!exe->pipe_error.data_buf)
      {
        exe->pipe_error.data_buf  = trep->buf;
        exe->pipe_error.data_size = trep->buf_size;
        trep->buf                 = NULL;
      }
      else
      {
        b = realloc(exe->pipe_error.data_buf,
                    exe->pipe_error.data_size + trep->buf_size);
        if (b)
        {
          memcpy(b + exe->pipe_error.data_size, trep->buf, trep->buf_size);
          exe->pipe_error.data_buf   = b;
          exe->pipe_error.data_size += trep->buf_size;
        }
        else ERR("Out of memory in reading exe pipe data");
      }
      event_data = core_exe_event_data_get(obj, CORE_EXE_PIPE_ERROR);
      if (event_data)
      {
        core_event_add(CORE_EXE_EVENT_ERROR,
                       event_data,
                       _core_exe_event_exe_data_free,
                       NULL);
        efl_event_callback_call(obj, CORE_EXE_EVENT_DATA_ERROR, event_data);
      }
    }
  }
  free(trep->buf);
  free(trep);
}

static DWORD WINAPI
_core_exe_thread_procedure(LPVOID data EFL_UNUSED)
{
  GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
  GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, 0);
  return 1;
}

static DWORD __stdcall _core_exe_exit_process(void *data)
{
  UINT *code;
  code = (UINT *)data;
  ExitProcess(*code);
  return 1;
}

static BOOL CALLBACK
_core_exe_enum_windows_procedure(HWND window, LPARAM data)
{
  Core_Exe      *obj = (Core_Exe *)data;
  Core_Exe_Data *exe = efl_data_scope_get(obj, CORE_EXE_CLASS);
  DWORD          thread_id;
  UINT           code = 0;

  if (!exe) return EFL_FALSE;
  thread_id = GetWindowThreadProcessId(window, NULL);

  if (thread_id == exe->thread_id)
  {
        /* Ctrl-C or Ctrl-Break */
    if (CreateRemoteThread(exe->process,
                           NULL,
                           0,
                           (LPTHREAD_START_ROUTINE)_core_exe_thread_procedure,
                           NULL,
                           0,
                           NULL))
      return EFL_FALSE;

    if ((exe->sig == CORE_EXE_WIN32_SIGINT) ||
        (exe->sig == CORE_EXE_WIN32_SIGQUIT))
      return EFL_FALSE;

        /* WM_CLOSE message */
    PostMessage(window, WM_CLOSE, 0, 0);
    if (WaitForSingleObject(exe->process, CORE_EXE_WIN32_TIMEOUT) ==
        WAIT_OBJECT_0)
      return EFL_FALSE;

        /* WM_QUIT message */
    PostMessage(window, WM_QUIT, 0, 0);
    if (WaitForSingleObject(exe->process, CORE_EXE_WIN32_TIMEOUT) ==
        WAIT_OBJECT_0)
      return EFL_FALSE;

        /* Exit process */
    if (CreateRemoteThread(exe->process,
                           NULL,
                           0,
                           (LPTHREAD_START_ROUTINE)_core_exe_exit_process,
                           &code,
                           0,
                           NULL))
      return EFL_FALSE;

    if (exe->sig == CORE_EXE_WIN32_SIGTERM) return EFL_FALSE;

    TerminateProcess(exe->process, 0);
    return EFL_FALSE;
  }
  return EFL_TRUE;
}

void
_impl_core_exe_run_priority_set(int pri)
{
  switch (pri)
  {
    case CORE_EXE_WIN32_PRIORITY_IDLE:
      run_pri = IDLE_PRIORITY_CLASS;
      break;

    case CORE_EXE_WIN32_PRIORITY_BELOW_NORMAL:
      run_pri = BELOW_NORMAL_PRIORITY_CLASS;
      break;

    case CORE_EXE_WIN32_PRIORITY_NORMAL:
      run_pri = NORMAL_PRIORITY_CLASS;
      break;

    case CORE_EXE_WIN32_PRIORITY_ABOVE_NORMAL:
      run_pri = ABOVE_NORMAL_PRIORITY_CLASS;
      break;

    case CORE_EXE_WIN32_PRIORITY_HIGH:
      run_pri = HIGH_PRIORITY_CLASS;
      break;

    case CORE_EXE_WIN32_PRIORITY_REALTIME:
      run_pri = REALTIME_PRIORITY_CLASS;
      break;

    default:
      break;
  }
}

int
_impl_core_exe_run_priority_get(void)
{
  switch (run_pri)
  {
    case IDLE_PRIORITY_CLASS:
      return CORE_EXE_WIN32_PRIORITY_IDLE;

    case BELOW_NORMAL_PRIORITY_CLASS:
      return CORE_EXE_WIN32_PRIORITY_BELOW_NORMAL;

    case NORMAL_PRIORITY_CLASS:
      return CORE_EXE_WIN32_PRIORITY_NORMAL;

    case ABOVE_NORMAL_PRIORITY_CLASS:
      return CORE_EXE_WIN32_PRIORITY_ABOVE_NORMAL;

    case HIGH_PRIORITY_CLASS:
      return CORE_EXE_WIN32_PRIORITY_HIGH;

    case REALTIME_PRIORITY_CLASS:
      return CORE_EXE_WIN32_PRIORITY_REALTIME;

      /* default should not be reached */
    default:
      return CORE_EXE_WIN32_PRIORITY_NORMAL;
  }
}

Eo *
_impl_core_exe_efl_object_finalize(Eo *obj, Core_Exe_Data *exe)
{
  char                exe_cmd_buf[32768];
  SECURITY_ATTRIBUTES sa;
  STARTUPINFO         si;
  PROCESS_INFORMATION pi;
  HANDLE              child_pipe_read  = NULL;
  HANDLE              child_pipe_error = NULL;
  const char         *shell            = NULL;
  Core_Exe_Event_Add *e;
  Core_Exe_Flags      flags;
  Efl_Bool            use_sh = EFL_FALSE;

  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

  flags = exe->flags;
  DBG("Creating process %s with flags %d", exe->cmd, flags);
  if (!exe->cmd) goto error;

  if ((flags & CORE_EXE_PIPE_AUTO) && (!(flags & CORE_EXE_PIPE_ERROR)) &&
      (!(flags & CORE_EXE_PIPE_READ)))
     /* We need something to auto pipe. */
    flags |= CORE_EXE_PIPE_READ | CORE_EXE_PIPE_ERROR;

  if ((flags & CORE_EXE_USE_SH)) use_sh = EFL_TRUE;
  else use_sh = eina_str_has_extension(exe->cmd, ".bat");

  if (use_sh)
  {
    int len;

    shell = "cmd.exe";
    len   = snprintf(exe_cmd_buf, sizeof(exe_cmd_buf), "/c %s", exe->cmd);
    if (len >= (int)sizeof(exe_cmd_buf))
      exe_cmd_buf[sizeof(exe_cmd_buf) - 1] = '\0';
  }
  else
  {
    int len;

    /* FIXME : faster with memset() but one must be careful with size */
    len = snprintf(exe_cmd_buf, sizeof(exe_cmd_buf), "%s", exe->cmd);
    if (len >= (int)sizeof(exe_cmd_buf))
      exe_cmd_buf[sizeof(exe_cmd_buf) - 1] = '\0';
  }

  /* stdout, stderr and stdin pipes */
  sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle       = EFL_TRUE;
  sa.lpSecurityDescriptor = NULL;

  if ((exe->flags & CORE_EXE_PIPE_READ) || (exe->flags & CORE_EXE_PIPE_ERROR))
  {
    Threaddata *tdat;

    tdat = calloc(1, sizeof(Threaddata));
    if (tdat)
    {
      tdat->obj = obj;
      /* stdout pipe */
      if (exe->flags & CORE_EXE_PIPE_READ)
      {
        if (!CreatePipe(&exe->pipe_read.child_pipe, &child_pipe_read, &sa, 0))
          goto error;
        if (!SetHandleInformation(exe->pipe_read.child_pipe,
                                  HANDLE_FLAG_INHERIT,
                                  0))
          goto error;
        tdat->read      = EFL_TRUE;
        tdat->read_pipe = exe->pipe_read.child_pipe;
      }
      /* stderr pipe */
      if (exe->flags & CORE_EXE_PIPE_ERROR)
      {
        if (!CreatePipe(&exe->pipe_error.child_pipe, &child_pipe_error, &sa, 0))
          goto error;
        if (!SetHandleInformation(exe->pipe_error.child_pipe,
                                  HANDLE_FLAG_INHERIT,
                                  0))
          goto error;
        tdat->error      = EFL_TRUE;
        tdat->error_pipe = exe->pipe_error.child_pipe;
      }
      exe->th = core_thread_feedback_run(_core_exe_win32_io_poll_thread,
                                         _core_exe_win32_io_poll_notify,
                                         NULL,
                                         NULL,
                                         tdat,
                                         EFL_TRUE);
      if (!exe->th)
      {
        free(tdat);
        goto error;
      }
    }
  }

  /* stdin pipe */
  if (exe->flags & CORE_EXE_PIPE_WRITE)
  {
    if (!CreatePipe(&exe->pipe_write.child_pipe,
                    &exe->pipe_write.child_pipe_x,
                    &sa,
                    0))
      goto error;
    if (!SetHandleInformation(exe->pipe_write.child_pipe_x,
                              HANDLE_FLAG_INHERIT,
                              0))
      goto error;
  }

  /* create child process */

  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb         = sizeof(STARTUPINFO);
  si.hStdOutput = child_pipe_read;
  si.hStdInput  = exe->pipe_write.child_pipe;
  si.hStdError  = child_pipe_error;
  si.dwFlags   |= STARTF_USESTDHANDLES;

  DBG("CreateProcess: shell:%s child:%s", use_sh ? "yes" : "no", exe_cmd_buf);
  if (!CreateProcess(shell,
                     exe_cmd_buf,
                     NULL,
                     NULL,
                     EFL_TRUE,
                     run_pri | CREATE_SUSPENDED,
                     NULL,
                     NULL,
                     &si,
                     &pi))
  {
    WRN("Failed to create process %s: %s", exe_cmd_buf, evil_last_error_get());
    goto error;
  }

  if ((flags & CORE_EXE_TERM_WITH_PARENT) && _core_exe_win32_job)
  {
    if (!AssignProcessToJobObject(_core_exe_win32_job, pi.hProcess))
      WRN("AssignProcessToJobObject failed (job: %p, process: %p",
          _core_exe_win32_job,
          pi.hProcess);
  }

  /*
    * Close pipe handles (do not continue to modify the parent).
    * We need to make sure that no handles to the write end of the
    * output and error pipes are maintained in this process or else
    * the pipe will not close when the child process exits and the
    * ReadFile will hang.
    */
  IF_FN_DEL(CloseHandle, child_pipe_read);
  IF_FN_DEL(CloseHandle, child_pipe_error);

  /* be sure that the child process is running */
  /* FIXME: This does not work if the child is an EFL-based app */
  /* if (WaitForInputIdle(pi.hProcess, INFINITE) == WAIT_FAILED) */
  /*   goto error; */
  exe->process        = pi.hProcess;
  exe->process_thread = pi.hThread;
  exe->pid            = pi.dwProcessId;
  exe->thread_id      = pi.dwThreadId;

  exe->h_close =
    core_main_win32_handler_add(exe->process, _core_exe_close_cb, obj);
  if (!exe->h_close) goto error;

  if (ResumeThread(exe->process_thread) == ((DWORD)-1))
  {
    ERR("Could not resume process");
    goto error;
  }

  exe->loop           = efl_provider_find(obj, EFL_LOOP_CLASS);
  Efl_Loop_Data *loop = efl_data_scope_get(exe->loop, EFL_LOOP_CLASS);
  if (loop) loop->exes = eina_list_append(loop->exes, obj);

  e = calloc(1, sizeof(Core_Exe_Event_Add));
  if (!e) goto error;
  e->exe = obj;
  core_event_add(CORE_EXE_EVENT_ADD, e, _core_exe_event_add_free, NULL);
  return obj;

error:
  return NULL;
}

Efl_Bool
_impl_core_exe_send(Core_Exe      *obj,
                    Core_Exe_Data *exe,
                    const void    *data,
                    int            size)
{
  DWORD num_exe;
  BOOL  res;

  res = WriteFile(exe->pipe_write.child_pipe_x, data, size, &num_exe, NULL);
  if (!res || num_exe == 0)
  {
    ERR("Core_Exe %p stdin is closed! Cannot send %d bytes from %p",
        obj,
        size,
        data);
    return EFL_FALSE;
  }

  return EFL_TRUE;
}

Core_Exe_Event_Data *
_impl_core_exe_event_data_get(Core_Exe      *obj,
                              Core_Exe_Data *exe,
                              Core_Exe_Flags flags)
{
  Core_Exe_Event_Data *e = NULL;
  unsigned char       *inbuf;
  DWORD                inbuf_num;
  Efl_Bool             is_buffered = EFL_FALSE;

  /* Sort out what sort of event we are. */
  if (flags & CORE_EXE_PIPE_READ)
  {
    flags = CORE_EXE_PIPE_READ;
    if (exe->flags & CORE_EXE_PIPE_READ_LINE_BUFFERED) is_buffered = EFL_TRUE;
  }
  else
  {
    flags = CORE_EXE_PIPE_ERROR;
    if (exe->flags & CORE_EXE_PIPE_ERROR_LINE_BUFFERED) is_buffered = EFL_TRUE;
  }

  /* Get the data. */
  if (flags & CORE_EXE_PIPE_READ)
  {
    inbuf                    = exe->pipe_read.data_buf;
    inbuf_num                = exe->pipe_read.data_size;
    exe->pipe_read.data_buf  = NULL;
    exe->pipe_read.data_size = 0;
  }
  else
  {
    inbuf                     = exe->pipe_error.data_buf;
    inbuf_num                 = exe->pipe_error.data_size;
    exe->pipe_error.data_buf  = NULL;
    exe->pipe_error.data_size = 0;
  }

  e = calloc(1, sizeof(Core_Exe_Event_Data));
  if (e)
  {
    e->exe  = obj;
    e->data = inbuf;
    e->size = inbuf_num;

    if (is_buffered) /* Deal with line buffering. */
    {
      char *c;
      DWORD i;
      DWORD max   = 0;
      DWORD count = 0;
      DWORD last  = 0;

      c = (char *)inbuf;
      for (i = 0; i < inbuf_num; i++)
      {
        if (inbuf[i] == '\n')
        {
          int end;

          if (count >= max)
          {
            Core_Exe_Event_Data_Line *lines;

            max += 10;
            lines =
              realloc(e->lines, sizeof(Core_Exe_Event_Data_Line) * (max + 1));
            if (lines) e->lines = lines;
            else
            {
              ERR("Out of memory in allocating exe lines");
              break;
            }
          }

          if ((i >= 1) && (inbuf[i - 1] == '\r')) end = i - 1;
          else end = i;
          inbuf[end]           = '\0';
          e->lines[count].line = c;
          e->lines[count].size = end - last;
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
          exe->pipe_read.data_size = i - last;
          exe->pipe_read.data_buf  = malloc(exe->pipe_read.data_size);
          if (exe->pipe_read.data_buf)
            memcpy(exe->pipe_read.data_buf, c, exe->pipe_read.data_size);
          else
          {
            exe->pipe_read.data_size = 0;
            ERR("Out of memory in allocating exe pipe data");
          }
        }
        else
        {
          exe->pipe_error.data_size = i - last;
          exe->pipe_error.data_buf  = malloc(exe->pipe_error.data_size);
          if (exe->pipe_error.data_buf)
            memcpy(exe->pipe_error.data_buf, c, exe->pipe_error.data_size);
          else
          {
            exe->pipe_error.data_size = 0;
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

  _core_exe_threads_terminate(obj);

  data = exe->data;
  if (exe->pre_free_cb) exe->pre_free_cb(data, obj);

  IF_FN_DEL(core_main_win32_handler_del, exe->h_close);

  IF_FN_DEL(CloseHandle, exe->process_thread);
  IF_FN_DEL(CloseHandle, exe->process);
  IF_FN_DEL(CloseHandle, exe->pipe_write.child_pipe);
  IF_FN_DEL(CloseHandle, exe->pipe_write.child_pipe_x);
  IF_FN_DEL(CloseHandle, exe->pipe_error.child_pipe);
  IF_FN_DEL(CloseHandle, exe->pipe_read.child_pipe);

  IF_FREE(exe->cmd);

  Efl_Loop_Data *loop = efl_data_scope_get(exe->loop, EFL_LOOP_CLASS);
  if (loop) loop->exes = eina_list_remove(loop->exes, obj);
  IF_FREE(exe->tag);
}

void
_impl_core_exe_pause(Core_Exe *obj EFL_UNUSED, Core_Exe_Data *exe)
{
  if (exe->is_suspended) return;
  if (SuspendThread(exe->process_thread) != (DWORD)-1) exe->is_suspended = 1;
}

void
_impl_core_exe_continue(Core_Exe *obj EFL_UNUSED, Core_Exe_Data *exe)
{
  if (!exe->is_suspended) return;
  if (ResumeThread(exe->process_thread) != (DWORD)-1) exe->is_suspended = 0;
}

void
_impl_core_exe_interrupt(Core_Exe *obj, Core_Exe_Data *exe)
{
  CloseHandle(exe->process_thread);
  exe->process_thread = NULL;
  CloseHandle(exe->process);
  exe->process = NULL;
  exe->sig     = CORE_EXE_WIN32_SIGINT;
  EnumWindows(_core_exe_enum_windows_procedure, (LPARAM)obj);
}

void
_impl_core_exe_quit(Core_Exe *obj, Core_Exe_Data *exe)
{
  CloseHandle(exe->process_thread);
  exe->process_thread = NULL;
  CloseHandle(exe->process);
  exe->process = NULL;
  exe->sig     = CORE_EXE_WIN32_SIGQUIT;
  EnumWindows(_core_exe_enum_windows_procedure, (LPARAM)obj);
}

void
_impl_core_exe_terminate(Core_Exe *obj, Core_Exe_Data *exe)
{
  /*    CloseHandle(exe->thread); */
  CloseHandle(exe->process);
  exe->process = NULL;
  exe->sig     = CORE_EXE_WIN32_SIGTERM;
  while (EnumWindows(_core_exe_enum_windows_procedure, (LPARAM)obj))
    ;
}

void
_impl_core_exe_kill(Core_Exe *obj, Core_Exe_Data *exe)
{
  CloseHandle(exe->process_thread);
  exe->process_thread = NULL;
  CloseHandle(exe->process);
  exe->process = NULL;
  exe->sig     = CORE_EXE_WIN32_SIGKILL;
  while (EnumWindows(_core_exe_enum_windows_procedure, (LPARAM)obj))
    ;
}

void
_impl_core_exe_auto_limits_set(Core_Exe *obj      EFL_UNUSED,
                               Core_Exe_Data *exe EFL_UNUSED,
                               int start_bytes    EFL_UNUSED,
                               int end_bytes      EFL_UNUSED,
                               int start_lines    EFL_UNUSED,
                               int end_lines      EFL_UNUSED)
{
  ERR("Not implemented on windows!");
}

void
_impl_core_exe_signal(Core_Exe *obj      EFL_UNUSED,
                      Core_Exe_Data *exe EFL_UNUSED,
                      int num            EFL_UNUSED)
{
  ERR("Not implemented on windows!");
}

void
_impl_core_exe_hup(Core_Exe *obj EFL_UNUSED, Core_Exe_Data *exe EFL_UNUSED)
{
  ERR("Not implemented on windows!");
}
