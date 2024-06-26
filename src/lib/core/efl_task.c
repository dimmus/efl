#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>

#include "core_private.h"

#define MY_CLASS EFL_TASK_CLASS

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

EOLIAN static void
_efl_task_priority_set(Eo *obj           EFL_UNUSED,
                       Efl_Task_Data    *pd,
                       Efl_Task_Priority priority)
{
  pd->priority = priority;
}

EOLIAN static Efl_Task_Priority
_efl_task_priority_get(const Eo *obj EFL_UNUSED, Efl_Task_Data *pd)
{
  return pd->priority;
}

EOLIAN static int
_efl_task_exit_code_get(const Eo *obj EFL_UNUSED, Efl_Task_Data *pd)
{
  return pd->exit_code;
}

EOLIAN static void
_efl_task_flags_set(Eo *obj EFL_UNUSED, Efl_Task_Data *pd, Efl_Task_Flags flags)
{
  pd->flags = flags;
}

EOLIAN static Efl_Task_Flags
_efl_task_flags_get(const Eo *obj EFL_UNUSED, Efl_Task_Data *pd)
{
  return pd->flags;
}

EOLIAN static void
_efl_task_efl_object_destructor(Eo *obj EFL_UNUSED, Efl_Task_Data *pd)
{
  eina_stringshare_del(pd->command);
  pd->command = NULL;
  efl_destructor(efl_super(obj, MY_CLASS));
}

EOLIAN static Efl_Object *
_efl_task_efl_object_constructor(Eo *obj, Efl_Task_Data *pd)
{
  obj       = efl_constructor(efl_super(obj, EFL_TASK_CLASS));
  pd->flags = EFL_TASK_FLAGS_EXIT_WITH_PARENT;
  return obj;
}

EOLIAN static void
_efl_task_efl_object_parent_set(Eo               *obj,
                                Efl_Task_Data *pd EFL_UNUSED,
                                Efl_Object       *parent)
{
  efl_parent_set(efl_super(obj, MY_CLASS), parent);
}

//////////////////////////////////////////////////////////////////////////

#include "efl_task.eo.c"
