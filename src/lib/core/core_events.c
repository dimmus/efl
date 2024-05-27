#include <stdlib.h>

#include "Efl_Core.h"
#include "core_private.h"

static Core_Event_Message_Handler *_event_msg_handler = NULL;

EAPI Core_Event_Handler *
core_event_handler_add(int type, Core_Event_Handler_Cb func, const void *data)
{
    return core_event_message_handler_add(_event_msg_handler,
                                          type,
                                          func,
                                          (void *)data);
}

EAPI Core_Event_Handler *
core_event_handler_prepend(int                   type,
                           Core_Event_Handler_Cb func,
                           const void           *data)
{
    return core_event_message_handler_prepend(_event_msg_handler,
                                              type,
                                              func,
                                              (void *)data);
}

EAPI void *
core_event_handler_del(Core_Event_Handler *event_handler)
{
    return core_event_message_handler_del(_event_msg_handler, event_handler);
}

EAPI void *
core_event_handler_data_get(Core_Event_Handler *eh)
{
    return core_event_message_handler_data_get(_event_msg_handler, eh);
}

EAPI void *
core_event_handler_data_set(Core_Event_Handler *eh, const void *data)
{
    return core_event_message_handler_data_set(_event_msg_handler,
                                               eh,
                                               (void *)data);
}

EAPI Core_Event *
core_event_add(int type, void *ev, Core_End_Cb func_free, void *data)
{
    Core_Event_Message *msg;
    if (type <= CORE_EVENT_NONE) return NULL;

    msg = core_event_message_handler_message_type_add(_event_msg_handler);
    if (msg)
    {
        core_event_message_data_set(msg, type, ev, func_free, data);
        efl_loop_message_handler_message_send(_event_msg_handler, msg);
    }
    return (Core_Event *)msg;
}

EAPI void *
core_event_del(Core_Event *event)
{
    void *data = NULL;
    if (!event) return data;
    core_event_message_data_get((Eo *)event, NULL, NULL, NULL, &data);
    _efl_loop_message_unsend((Eo *)event);
    return data;
}

EAPI int
core_event_type_new(void)
{
    return core_event_message_handler_type_new(_event_msg_handler);
}

EAPI Core_Event_Filter *
core_event_filter_add(Core_Data_Cb   func_start,
                      Core_Filter_Cb func_filter,
                      Core_End_Cb    func_end,
                      const void    *data)
{
    return core_event_message_handler_filter_add(_event_msg_handler,
                                                 func_start,
                                                 func_filter,
                                                 func_end,
                                                 (void *)data);
}

EAPI void *
core_event_filter_del(Core_Event_Filter *ef)
{
    return core_event_message_handler_filter_del(_event_msg_handler, ef);
}

EAPI int
core_event_current_type_get(void)
{
    return core_event_message_handler_current_type_get(_event_msg_handler);
}

EAPI void *
core_event_current_event_get(void)
{
    return core_event_message_handler_current_event_get(_event_msg_handler);
}

Efl_Bool
_core_event_init(void)
{
    const char *choice = getenv("EINA_MEMPOOL");
    if ((!choice) || (!choice[0])) choice = "chained_mempool";

    _event_msg_handler =
        efl_add(CORE_EVENT_MESSAGE_HANDLER_CLASS, _mainloop_singleton);
    efl_provider_register(_mainloop_singleton,
                          CORE_EVENT_MESSAGE_HANDLER_CLASS,
                          _event_msg_handler);

    if (!_event_msg_handler)
    {
        ERR("Cannot create legacy ecore event message handler");
        return EFL_FALSE;
    }
    // init some core legacy event types in t he same order and numbering as before
    // CORE_EVENT_NONE                     0
    // no need to do as ev types start at 1

    // CORE_EVENT_SIGNAL_USER              1
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_SIGNAL_HUP               2
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_SIGNAL_EXIT              3
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_SIGNAL_POWER             4
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_SIGNAL_REALTIME          5
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_MEMORY_STATE             6
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_POWER_STATE              7
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_LOCALE_CHANGED           8
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_HOSTNAME_CHANGED         9
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_SYSTEM_TIMEDATE_CHANGED  10
    core_event_message_handler_type_new(_event_msg_handler);
    // CORE_EVENT_COUNT                    11
    // no need to do as it was a count, nto an event

    return EFL_TRUE;
}

void
_core_event_shutdown(void)
{
    efl_loop_message_handler_message_clear(_event_msg_handler);
    _event_msg_handler = NULL;
}

void *
_core_event_signal_user_new(void)
{
    return calloc(1, sizeof(Core_Event_Signal_User));
}

void *
_core_event_signal_hup_new(void)
{
    return calloc(1, sizeof(Core_Event_Signal_Hup));
}

void *
_core_event_signal_exit_new(void)
{
    return calloc(1, sizeof(Core_Event_Signal_Exit));
}

void *
_core_event_signal_power_new(void)
{
    return calloc(1, sizeof(Core_Event_Signal_Power));
}

void *
_core_event_signal_realtime_new(void)
{
    return calloc(1, sizeof(Core_Event_Signal_Realtime));
}

EAPI void
core_event_type_flush_internal(int type, ...)
{
    va_list args;

    if (type == CORE_EVENT_NONE) return;
    core_event_message_handler_type_flush(_event_msg_handler, type);

    va_start(args, type);
    for (;;)
    {
        type = va_arg(args, int);
        if (type == CORE_EVENT_NONE) break;
        core_event_message_handler_type_flush(_event_msg_handler, type);
    }
    va_end(args);
}
