#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>

#include <Efl_Eo.h>

#include "Efl_Core.h"
#include "core_private.h"

struct _Core_Factorized_Idle
{
    Core_Task_Cb func;
    void         *data;

    const Efl_Callback_Array_Item *desc;

    short    references;
    Efl_Bool delete_me : 1;
};

void
_core_factorized_idle_event_del(void *data, const Efl_Event *event EFL_UNUSED)
{
    _core_factorized_idle_del(data);
}

void
_core_factorized_idle_process(void *data, const Efl_Event *event EFL_UNUSED)
{
    Core_Factorized_Idle *idler = data;

    idler->references++;
    if (!_core_call_task_cb(idler->func, idler->data))
        idler->delete_me = EFL_TRUE;
    idler->references--;

    if (idler->delete_me && idler->references == 0)
        _core_factorized_idle_del(idler);
}

static Eina_Mempool *idler_mp = NULL;

void *
_core_factorized_idle_del(Core_Idler *idler)
{
    void *data;

    if (!idler) return NULL;
    EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

    if (idler->references > 0)
    {
        idler->delete_me = EFL_TRUE;
        return idler->data;
    }

    efl_event_callback_array_del(_mainloop_singleton, idler->desc, idler);

    data = idler->data;
    eina_mempool_free(idler_mp, idler);
    return data;
}

Core_Factorized_Idle *
_core_factorized_idle_add(const Efl_Callback_Array_Item *desc,
                           Core_Task_Cb                  func,
                           const void                    *data)
{
    Core_Factorized_Idle *ret;

    EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

    if (!func)
    {
        ERR("callback function must be set up for an object of Core_Idler.");
        return NULL;
    }

    if (!idler_mp)
    {
        idler_mp = eina_mempool_add("chained_mempool",
                                    "Core_Idle*",
                                    NULL,
                                    sizeof(Core_Factorized_Idle),
                                    23);
        if (!idler_mp) return NULL;
    }

    ret = eina_mempool_malloc(idler_mp, sizeof(Core_Factorized_Idle));
    if (!ret) return NULL;

    ret->func       = func;
    ret->data       = (void *)data;
    ret->desc       = desc;
    ret->references = 0;
    ret->delete_me  = EFL_FALSE;

    efl_event_callback_array_add(_mainloop_singleton, desc, ret);

    return ret;
}

/* Specific to Core_Idler implementation */

EFL_CALLBACKS_ARRAY_DEFINE(core_idler_callbacks,
                           { EFL_LOOP_EVENT_IDLE,
                             _core_factorized_idle_process },
                           { EFL_EVENT_DEL, _core_factorized_idle_event_del });

EAPI Core_Idler *
core_idler_add(Core_Task_Cb func, const void *data)
{
    return _core_factorized_idle_add(core_idler_callbacks(), func, data);
}

EAPI void *
core_idler_del(Core_Idler *idler)
{
    return _core_factorized_idle_del(idler);
}
