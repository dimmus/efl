#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

struct _core_exe_dead_exe
{
  pid_t pid;
  char *cmd;
};

#ifdef _WIN32
/*
 * this job is used to close child processes when parent one is closed
 * see https://stackoverflow.com/a/53214/688348
 */
HANDLE _core_exe_win32_job = NULL;
#endif

EAPI int CORE_EXE_EVENT_ADD   = 0;
EAPI int CORE_EXE_EVENT_DEL   = 0;
EAPI int CORE_EXE_EVENT_DATA  = 0;
EAPI int CORE_EXE_EVENT_ERROR = 0;

EAPI void
core_exe_run_priority_set(int pri)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  _impl_core_exe_run_priority_set(pri);
}

EAPI int
core_exe_run_priority_get(void)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  return _impl_core_exe_run_priority_get();
}

EAPI Core_Exe *
core_exe_run(const char *exe_cmd, const void *data)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  return core_exe_pipe_run(exe_cmd, 0, data);
}

EAPI Core_Exe *
core_exe_pipe_run(const char *exe_cmd, Core_Exe_Flags flags, const void *data)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  Core_Exe *ret = efl_add(MY_CLASS,
                          efl_main_loop_get(),
                          Core_obj_exe_command_set(efl_added, exe_cmd, flags));
  if (ret)
  {
    Core_Exe_Data *pd = efl_data_scope_get(ret, MY_CLASS);
    pd->data          = (void *)data;
  }
  return ret;
}

EOLIAN static void
_core_exe_command_set(Eo *obj        EFL_UNUSED,
                      Core_Exe_Data *pd,
                      const char    *cmd,
                      Core_Exe_Flags flags)
{
  if (!cmd) return;
  pd->cmd   = strdup(cmd);
  pd->flags = flags;
}

EOLIAN static void
_core_exe_command_get(const Eo *obj   EFL_UNUSED,
                      Core_Exe_Data  *pd,
                      const char    **cmd,
                      Core_Exe_Flags *flags)
{
  if (cmd) *cmd = pd->cmd;
  if (flags) *flags = pd->flags;
}

EOLIAN static Eo *
_core_exe_efl_object_finalize(Eo *obj, Core_Exe_Data *exe)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  obj = efl_finalize(efl_super(obj, MY_CLASS));
  if (!obj) return obj;
  return _impl_core_exe_efl_object_finalize(obj, exe);
}

EAPI void
core_exe_callback_pre_free_set(Core_Exe *obj, Core_Exe_Cb func)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  exe->pre_free_cb = func;
}

EAPI Efl_Bool
core_exe_send(Core_Exe *obj, const void *data, int size)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(EFL_FALSE);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return EFL_FALSE;

  EINA_SAFETY_ON_TRUE_RETURN_VAL(size == 0, EFL_TRUE);
  if (exe->close_stdin)
  {
    ERR("Core_Exe %p stdin is closed! Cannot send %d bytes from %p",
        exe,
        size,
        data);
    return EFL_FALSE;
  }
  return _impl_core_exe_send(obj, exe, data, size);
}

EAPI void
core_exe_close_stdin(Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  exe->close_stdin = 1;
}

EAPI void
core_exe_auto_limits_set(Core_Exe *obj,
                         int       start_bytes,
                         int       end_bytes,
                         int       start_lines,
                         int       end_lines)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  _impl_core_exe_auto_limits_set(obj,
                                 exe,
                                 start_bytes,
                                 end_bytes,
                                 start_lines,
                                 end_lines);
}

EAPI Core_Exe_Event_Data *
core_exe_event_data_get(Core_Exe *obj, Core_Exe_Flags flags)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return NULL;
  return _impl_core_exe_event_data_get(obj, exe, flags);
}

EAPI void
core_exe_tag_set(Core_Exe *obj, const char *tag)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  IF_FREE(exe->tag);
  if (tag) exe->tag = strdup(tag);
  else exe->tag = NULL;
}

EAPI const char *
core_exe_tag_get(const Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return NULL;
  return exe->tag;
}

EAPI void *
core_exe_free(Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return NULL;
  void *data = exe->data;
  efl_del(obj);
  return data;
}

EOLIAN static void
_core_exe_efl_object_destructor(Eo *obj, Core_Exe_Data *exe)
{
  efl_destructor(efl_super(obj, CORE_EXE_CLASS));
  _impl_core_exe_efl_object_destructor(obj, exe);
}

EAPI void
core_exe_event_data_free(Core_Exe_Event_Data *e)
{
  if (!e) return;
  IF_FREE(e->lines);
  IF_FREE(e->data);
  free(e);
}

EAPI pid_t
core_exe_pid_get(const Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return -1;
  return exe->pid;
}

EAPI const char *
core_exe_cmd_get(const Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  const char *ret = NULL;
  Core_obj_exe_command_get(obj, &ret, NULL);
  return ret;
}

EAPI void *
core_exe_data_get(const Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return NULL;
  return exe->data;
}

EAPI void *
core_exe_data_set(Core_Exe *obj, void *data)
{
  void *ret;
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return NULL;
  ret       = exe->data;
  exe->data = data;
  return ret;
}

EAPI Core_Exe_Flags
core_exe_flags_get(const Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN_VAL(0);
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return 0;
  return exe->flags;
}

EAPI void
core_exe_pause(Core_Exe *obj)
{
  efl_control_suspend_set(obj, EFL_TRUE);
}

EAPI void
core_exe_continue(Core_Exe *obj)
{
  efl_control_suspend_set(obj, EFL_FALSE);
}

EOLIAN static void
_core_exe_efl_control_suspend_set(Eo *obj        EFL_UNUSED,
                                  Core_Exe_Data *exe,
                                  Efl_Bool       suspend)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  if (suspend) _impl_core_exe_pause(obj, exe);
  else _impl_core_exe_continue(obj, exe);
}

EAPI void
core_exe_interrupt(Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  _impl_core_exe_interrupt(obj, exe);
}

EAPI void
core_exe_quit(Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  _impl_core_exe_quit(obj, exe);
}

EAPI void
core_exe_terminate(Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  _impl_core_exe_terminate(obj, exe);
}

EAPI void
core_exe_kill(Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  _impl_core_exe_kill(obj, exe);
}

EAPI void
core_exe_signal(Core_Exe *obj, int num)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  _impl_core_exe_signal(obj, exe, num);
}

EAPI void
core_exe_hup(Core_Exe *obj)
{
  EINA_MAIN_LOOP_CHECK_RETURN;
  Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
  if (!efl_isa(obj, MY_CLASS)) return;
  _impl_core_exe_hup(obj, exe);
}

void
_core_exe_init(void)
{
#ifdef _WIN32
  _core_exe_win32_job = CreateJobObject(NULL, NULL);
  if (_core_exe_win32_job)
  {
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli;

    memset(&jeli, 0, sizeof(jeli));
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(_core_exe_win32_job,
                                 JobObjectExtendedLimitInformation,
                                 &jeli,
                                 sizeof(jeli)))
    {
      CloseHandle(_core_exe_win32_job);
      _core_exe_win32_job = NULL;
    }
  }
#endif
  CORE_EXE_EVENT_ADD   = core_event_type_new();
  CORE_EXE_EVENT_DEL   = core_event_type_new();
  CORE_EXE_EVENT_DATA  = core_event_type_new();
  CORE_EXE_EVENT_ERROR = core_event_type_new();
}

void
_core_exe_shutdown(void)
{
  Core_Exe      *exe = NULL;
  Eina_List     *l1, *l2;
  Efl_Loop_Data *loop = EFL_LOOP_DATA;

  EINA_LIST_FOREACH_SAFE(loop->exes, l1, l2, exe)
  core_exe_free(exe);

  core_event_type_flush(CORE_EXE_EVENT_ADD,
                        CORE_EXE_EVENT_DEL,
                        CORE_EXE_EVENT_DATA,
                        CORE_EXE_EVENT_ERROR);

#ifdef _WIN32
  if (_core_exe_win32_job) CloseHandle(_core_exe_win32_job);
#endif
}

Core_Exe *
_core_exe_find(pid_t pid)
{
  Eina_List     *itr;
  Core_Exe      *obj;
  Efl_Loop_Data *loop = EFL_LOOP_DATA;

  EINA_LIST_FOREACH(loop->exes, itr, obj)
  {
    Core_Exe_Data *exe = efl_data_scope_get(obj, MY_CLASS);
    if (exe->pid == pid) return obj;
  }
  return NULL;
}

void *
_core_exe_event_del_new(void)
{
  Core_Exe_Event_Del *e = calloc(1, sizeof(Core_Exe_Event_Del));
  return e;
}

void
_core_exe_event_del_free(void *data EFL_UNUSED, void *ev)
{
  Core_Exe_Event_Del *e = ev;
  if (e->exe) core_exe_free(e->exe);
  free(e);
}

void
_core_exe_event_exe_data_free(void *data EFL_UNUSED, void *ev)
{
  Core_Exe_Event_Data *e = ev;
  core_exe_event_data_free(e);
}

Core_Exe_Event_Add *
_core_exe_event_add_new(void)
{
  Core_Exe_Event_Add *e = calloc(1, sizeof(Core_Exe_Event_Add));
  return e;
}

void
_core_exe_event_add_free(void *data EFL_UNUSED, void *ev)
{
  Core_Exe_Event_Add *e = ev;
  free(e);
}

#include "core_exe_eo.c"
