#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>

#include <Efl_Eo.h>

#include "Efl_Core.h"
#include "core_private.h"

static Efl_Bool _core_job_event_handler(void *data, int type, void *ev);
static void     _core_job_event_free(void *data, void *ev);

static int                  core_event_job_type = 0;
static Core_Event_Handler *_core_job_handler   = NULL;

struct _Core_Job
{
    Core_Event *event;
    Core_Cb     func;
    void        *data;
};

void
_core_job_init(void)
{
    core_event_job_type = core_event_type_new();
    _core_job_handler   = core_event_handler_add(core_event_job_type,
                                                 _core_job_event_handler,
                                                 NULL);
}

void
_core_job_shutdown(void)
{
    core_event_handler_del(_core_job_handler);
    _core_job_handler = NULL;
}

EAPI Core_Job *
core_job_add(Core_Cb func, const void *data)
{
    Core_Job *job;

    EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

    if (!func)
    {
        ERR("Callback function must be set up for an Core_Job object");
        return NULL;
    }

    job = calloc(1, sizeof(Core_Job));
    if (!job) return NULL;

    job->event =
        core_event_add(core_event_job_type, job, _core_job_event_free, job);
    if (!job->event)
    {
        ERR("No event was assigned to Core_Job '%p'", job);
        free(job);
        return NULL;
    }
    job->func = func;
    job->data = (void *)data;

    return job;
}

EAPI void *
core_job_del(Core_Job *job)
{
    void *data;

    if (!job) return NULL;

    EINA_MAIN_LOOP_CHECK_RETURN_VAL(NULL);

    data = job->data;
    core_event_del(job->event);

    return data;
}

static Efl_Bool
_core_job_event_handler(void *data EFL_UNUSED, int type EFL_UNUSED, void *ev)
{
    Core_Job *job;

    job = ev;
    job->func(job->data);
    return CORE_CALLBACK_DONE;
}

static void
_core_job_event_free(void *data, void *job EFL_UNUSED)
{
    free(data);
}
