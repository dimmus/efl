#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>

#include <Efl_Eo.h>

#include "Efl_Core.h"
#include "core_private.h"

EFL_CALLBACKS_ARRAY_DEFINE(core_idle_exiter_callbacks,
                           { EFL_LOOP_EVENT_IDLE_EXIT,
                             _core_factorized_idle_process },
                           { EFL_EVENT_DEL, _core_factorized_idle_event_del });

EAPI Core_Idle_Exiter *
core_idle_exiter_add(Core_Task_Cb func, const void *data)
{
    return _core_factorized_idle_add(core_idle_exiter_callbacks(), func, data);
}

EAPI void *
core_idle_exiter_del(Core_Idle_Exiter *idle_exiter)
{
    return _core_factorized_idle_del(idle_exiter);
}
