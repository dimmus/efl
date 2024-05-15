#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>

#include <Efl_Shared.h>

#include "Efl_Core.h"
#include "core_private.h"

typedef struct _Core_Mempool Core_Mempool;

struct _Core_Mempool
{
    const char   *name;
    Eina_Mempool *mp;
    size_t        size;
};

#define GENERIC_ALLOC_FREE(TYPE, Type)                                        \
    extern size_t _core_sizeof_##TYPE;                                       \
    Core_Mempool Type##_mp = { #TYPE, NULL, 0 };                             \
    TYPE         *Type##_calloc(unsigned int num)                             \
    {                                                                         \
        return eina_mempool_calloc(Type##_mp.mp, num * _core_sizeof_##TYPE); \
    }                                                                         \
    void Type##_mp_free(TYPE *e)                                              \
    {                                                                         \
        eina_mempool_free(Type##_mp.mp, e);                                   \
    }

//GENERIC_ALLOC_FREE(Core_Animator, core_animator);
//GENERIC_ALLOC_FREE(Core_Event_Handler, core_event_handler);
//GENERIC_ALLOC_FREE(Core_Event_Filter, core_event_filter);
//GENERIC_ALLOC_FREE(Core_Event, core_event);
//GENERIC_ALLOC_FREE(Core_Idle_Exiter, core_idle_exiter);
//GENERIC_ALLOC_FREE(Core_Idle_Enterer, core_idle_enterer);
//GENERIC_ALLOC_FREE(Core_Idler, core_idler);
//GENERIC_ALLOC_FREE(Core_Job, core_job);
//GENERIC_ALLOC_FREE(Core_Timer, core_timer);
//GENERIC_ALLOC_FREE(Core_Poller, core_poller);
GENERIC_ALLOC_FREE(Core_Pipe, core_pipe);
GENERIC_ALLOC_FREE(Core_Fd_Handler, core_fd_handler);
GENERIC_ALLOC_FREE(Efl_Loop_Promise_Simple_Data, efl_loop_promise_simple_data);
#ifdef _WIN32
GENERIC_ALLOC_FREE(Core_Win32_Handler, Core_win32_handler);
#endif

static Core_Mempool *mempool_array[] = {
//  &core_animator_mp,
//  &core_event_handler_mp,
//  &core_event_filter_mp,
//  &core_event_mp,
//  &core_idle_exiter_mp,
//  &core_idle_enterer_mp,
//  &core_idler_mp,
//  &core_job_mp,
//  &core_timer_mp,
//  &core_poller_mp,
    &core_pipe_mp,
    &core_fd_handler_mp,
    &efl_loop_promise_simple_data_mp,
#ifdef _WIN32
    &Core_win32_handler_mp
#endif
};

Efl_Bool
core_mempool_init(void)
{
    const char  *choice;
    unsigned int i;

#define MP_SIZE_INIT(TYPE, Type) \
    Type##_mp.size = _core_sizeof_##TYPE

//   MP_SIZE_INIT(Core_Animator, Core_animator);
//   MP_SIZE_INIT(Core_Event_Handler, core_event_handler);
//   MP_SIZE_INIT(Core_Event_Filter, core_event_filter);
//   MP_SIZE_INIT(Core_Event, core_event);
//   MP_SIZE_INIT(Core_Idle_Exiter, Core_idle_exiter);
//   MP_SIZE_INIT(Core_Idle_Enterer, Core_idle_enterer);
//   MP_SIZE_INIT(Core_Idler, Core_idler);
//   MP_SIZE_INIT(Core_Job, Core_job);
//   MP_SIZE_INIT(Core_Timer, core_timer);
//   MP_SIZE_INIT(Core_Poller, Core_poller);
    MP_SIZE_INIT(Core_Pipe, core_pipe);
    MP_SIZE_INIT(Core_Fd_Handler, core_fd_handler);
    MP_SIZE_INIT(Efl_Loop_Promise_Simple_Data, efl_loop_promise_simple_data);
#ifdef _WIN32
    MP_SIZE_INIT(Core_Win32_Handler, Core_win32_handler);
#endif
#undef MP_SIZE_INIT

    choice = getenv("EINA_MEMPOOL");
    if ((!choice) || (!choice[0])) choice = "chained_mempool";

    for (i = 0; i < sizeof(mempool_array) / sizeof(mempool_array[0]); ++i)
    {
retry:
        mempool_array[i]->mp = eina_mempool_add(choice,
                                                mempool_array[i]->name,
                                                NULL,
                                                mempool_array[i]->size,
                                                16);
        if (!mempool_array[i]->mp)
        {
            if (!(!strcmp(choice, "pass_through")))
            {
                ERR("Falling back to pass through ! Previously tried '%s' "
                    "mempool.",
                    choice);
                choice = "pass_through";
                goto retry;
            }
            else
            {
                ERR("Impossible to allocate mempool '%s' !", choice);
                return EFL_FALSE;
            }
        }
    }
    return EFL_TRUE;
}

void
core_mempool_shutdown(void)
{
    unsigned int i;

    for (i = 0; i < sizeof(mempool_array) / sizeof(mempool_array[0]); ++i)
    {
        eina_mempool_del(mempool_array[i]->mp);
        mempool_array[i]->mp = NULL;
    }
}
