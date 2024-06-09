#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>

#include <Efl_Eo.h>

#include "Efl_Core.h"
#include "core_private.h"

EFL_CALLBACKS_ARRAY_DEFINE(core_idle_enterer_callbacks,
                           { EFL_LOOP_EVENT_IDLE_ENTER,
                             _core_factorized_idle_process },
                           { EFL_EVENT_DEL, _core_factorized_idle_event_del });

EAPI Core_Idle_Enterer *
core_idle_enterer_add(Core_Task_Cb func, const void *data)
{
  return _core_factorized_idle_add(core_idle_enterer_callbacks(), func, data);
}

EAPI Core_Idle_Enterer *
core_idle_enterer_before_add(Core_Task_Cb func, const void *data)
{
  Core_Idle_Enterer *ie = NULL;
  ie = _core_factorized_idle_add(core_idle_enterer_callbacks(), func, data);

   // This avoid us duplicating code and should only be slightly slower
   // due to a useless cycle of callback registration
  efl_event_callback_array_del(_mainloop_singleton,
                               core_idle_enterer_callbacks(),
                               ie);
  efl_event_callback_array_priority_add(_mainloop_singleton,
                                        core_idle_enterer_callbacks(),
                                        EFL_CALLBACK_PRIORITY_BEFORE,
                                        ie);

  return ie;
}

EAPI void *
core_idle_enterer_del(Core_Idle_Enterer *idle_enterer)
{
  return _core_factorized_idle_del(idle_enterer);
}
