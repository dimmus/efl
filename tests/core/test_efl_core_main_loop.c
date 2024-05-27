#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>

#ifdef _WIN32
#  include <evil_private.h> /* pipe */
#endif

#include <Efl_Shared.h>
#include <Efl_Core.h>

#include "efl_core_suite.h"

#define CORE_EVENT_CUSTOM_1 1
#define CORE_EVENT_CUSTOM_2 2

static Efl_Bool
_quit_cb(void *data)
{
    Efl_Bool *val = data;
    if (val) *val = EFL_TRUE;
    core_main_loop_quit();
    return EFL_FALSE;
}

EFL_START_TEST(core_test_core_init)
{}

EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop)
{
    Efl_Bool    did = EFL_FALSE;
    Core_Timer *timer;

    timer = core_timer_add(0.0, _quit_cb, &did);
    fail_if(timer == NULL);

   /* ensure that this does not auto-cancel main loop */
    core_main_loop_quit();
    core_main_loop_begin();

    fail_if(did == EFL_FALSE);
}

EFL_END_TEST

// Disabled tests: inner main loops are not supposed to work!
#if 0
static Efl_Bool _timer3(void *data EFL_UNUSED)
{
   /* timer 3, do nothing */
   return EFL_FALSE;
}

static Efl_Bool _timer2(void *data EFL_UNUSED)
{
   /* timer 2, quit inner mainloop */
   core_main_loop_quit();
   return EFL_FALSE;
}

static Efl_Bool _timer1(void *data)
{
   /* timer 1, begin inner mainloop */
   int *times = data;
   (*times)++;

   core_timer_add(0.3, _timer2, NULL);
   core_timer_add(0.1, _timer3, NULL);
   core_main_loop_begin();

   core_main_loop_quit();

   return EFL_FALSE;
}

EFL_START_TEST(core_test_core_main_loop_timer_inner)
{
   Core_Timer *timer;
   int times = 0;

   timer = core_timer_add(1.0, _timer1, &times);
   fail_if(timer == NULL);

   /* BEGIN: outer mainloop */
   core_main_loop_begin();
   /*END: outer mainloop */

   fail_if(times != 1);
}
EFL_END_TEST
#endif

static Efl_Bool
_fd_handler_cb(void *data, Core_Fd_Handler *handler EFL_UNUSED)
{
    /* FIXME: why setting val if it is overwritten just after and what is its purpose ??? */
    Efl_Bool *val = data;

    *val = EFL_TRUE;
    core_main_loop_quit();
    return EFL_FALSE;
}

EFL_START_TEST(core_test_core_main_loop_fd_handler)
{
    Efl_Bool         did = EFL_FALSE;
    Core_Fd_Handler *fd_handler;
    int              comm[2];
    int              ret;

    ret = pipe(comm);
    fail_if(ret != 0);

    fd_handler = core_main_fd_handler_add(comm[0],
                                          CORE_FD_READ,
                                          _fd_handler_cb,
                                          &did,
                                          NULL,
                                          NULL);
    fail_if(fd_handler == NULL);

    ret = write(comm[1], &did, 1);
    fail_if(ret != 1);

    core_main_loop_begin();

    close(comm[0]);
    close(comm[1]);

    fail_if(did == EFL_FALSE);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop_fd_handler_valid_flags)
{
    Core_Fd_Handler *fd_handler;
    int              comm[2];
    int              ret;

    ret = pipe(comm);
    fail_if(ret != 0);

    fd_handler =
        core_main_fd_handler_add(comm[0], 0, _fd_handler_cb, NULL, NULL, NULL);
    fail_if(fd_handler != NULL);

    if (fd_handler) core_main_fd_handler_del(fd_handler);

    close(comm[0]);
    close(comm[1]);
}

EFL_END_TEST

EFL_START_TEST(core_test_core_main_loop_fd_handler_activate_modify)
{
    Efl_Bool         did = EFL_FALSE;
    Core_Fd_Handler *fd_handler;
    int              comm[2];
    int              ret;

    ret = pipe(comm);
    fail_if(ret != 0);

    fd_handler = core_main_fd_handler_add(comm[0],
                                          CORE_FD_ERROR,
                                          _fd_handler_cb,
                                          &did,
                                          NULL,
                                          NULL);
    fail_if(fd_handler == NULL);

    core_main_fd_handler_active_set(fd_handler, CORE_FD_READ);

    ret = write(comm[1], "e", 1);
    fail_if(ret != 1);

    core_main_loop_begin();

    close(comm[0]);
    close(comm[1]);

    fail_if(did != EFL_TRUE);
}

EFL_END_TEST

static Efl_Bool
_event_handler_cb(void *data, int type, void *event)
{
    int *did = data;

    int  t1 = type;
    int *e1 = event;

    int  t2 = core_event_current_type_get();
    int *e2 = core_event_current_event_get();

    if (t1 == t2) (*did)++;

    if (*e1 == *e2 &&
        (CORE_EVENT_CUSTOM_1 == *e1 || CORE_EVENT_CUSTOM_2 == *e1))
        (*did)++;

    return EFL_TRUE;
}

static Efl_Bool
_event_handler_cb2(void *data, int type EFL_UNUSED, void *event EFL_UNUSED)
{
    int *did = data;
    (*did)++;
    return EFL_TRUE;
}

static Efl_Bool
_event_handler_cb3(void *data, int type EFL_UNUSED, void *event EFL_UNUSED)
{
    int *did = data;
    (*did)++;
    return EFL_TRUE;
}

static int *
_event_new(int id)
{
    int *ev = malloc(sizeof(int));

    switch (id)
    {
        case CORE_EVENT_CUSTOM_1:
            *ev = CORE_EVENT_CUSTOM_1;
            break;

        case CORE_EVENT_CUSTOM_2:
            *ev = CORE_EVENT_CUSTOM_2;
            break;

        default:
            *ev = CORE_EVENT_NONE;
    }

    return ev;
}

static void
_event_free(void *user_data, void *func_data)
{
    int *did = user_data;
    int *ev  = func_data;

    if (CORE_EVENT_CUSTOM_1 == *ev || CORE_EVENT_CUSTOM_2 == *ev) (*did)++;

    free(ev);
}

static void *
_filter_start(void *data)
{
    int *fdid = data;
    (*fdid)++;
    return NULL;
}

static Efl_Bool
_filter(void           *data,
        void *loop_data EFL_UNUSED,
        int type        EFL_UNUSED,
        void           *event)
{
    Efl_Bool res  = EFL_TRUE;
    int     *fdid = data;
    int     *ev   = event;

    if (NULL != event)
    {
        /* Ignore second event */
        if (CORE_EVENT_CUSTOM_2 == *ev)
        {
            res = EFL_FALSE;
        }
    }

    (*fdid)++;

    return res;
}

static void
_filter_end(void *user_data, void *func_data EFL_UNUSED)
{
    int *fdid = user_data;
    (*fdid)++;
}

EFL_START_TEST(core_test_core_main_loop_event)
{
    Core_Event_Handler *handler, *handler2, *handler3;
    Core_Event_Filter  *filter_handler;
    Core_Event         *event;
    Core_Event         *event2;
    int                 type, type2;
    int                *ev     = NULL;
    int                 did    = 0;
    int                 filter = 0;

    /* Create 2 new event types */
    type = core_event_type_new();
    fail_if(type < 1);

    type2 = core_event_type_new();
    fail_if(type < 1);

    /* Add handler for new type of event */
    handler = core_event_handler_add(type, _event_handler_cb, &did);
    fail_if(handler == NULL);

    /* Add another handler for event which will be deleted in next step */
    handler2 = core_event_handler_add(type, _event_handler_cb2, &did);
    fail_if(handler2 == NULL);

    /* Add handler for event which will be filtered */
    handler3 = core_event_handler_add(type2, _event_handler_cb3, &did);
    fail_if(handler3 == NULL);

    /* Add filtering mechanism */
    filter_handler =
        core_event_filter_add(_filter_start, _filter, _filter_end, &filter);

    /* Add into main loop three events: one to process, one to filter, one to quit */
    ev    = _event_new(CORE_EVENT_CUSTOM_1);
    event = core_event_add(type, ev, _event_free, &did);
    fail_if(event == NULL);

    ev    = _event_new(CORE_EVENT_CUSTOM_2);
    event = core_event_add(type2, ev, _event_free, &did);
    fail_if(event == NULL);

    event = core_event_add(CORE_EVENT_SIGNAL_EXIT, NULL, NULL, NULL);
    fail_if(event == NULL);

    /* Add one more events: to check a type */
    event2 = core_event_add(CORE_EVENT_NONE, NULL, NULL, NULL);
    fail_if(event2 != NULL);

    core_main_loop_begin();

    /*
      Check internal fail cases:
       event_cbx - 3 increments (4th should be ignored)
       free      - 2 increments
   */
    fail_if(did != 3 + 2);  // 5

    /*
      Check filter procedures calls:
       start  - 1 call
       filter - 3 calls
       end    - 1 call
   */
    fail_if(filter != 1 + 3 + 1);  // 5

    /* New loop but with new data and without filter and one callback procedure */
    int did2 = 0;
    filter   = 0;

    int *old = core_event_handler_data_set(handler, &did2);
    int *new = core_event_handler_data_get(handler);

    core_event_handler_del(handler2);
    core_event_filter_del(filter_handler);

    fail_if(*old != did);
    fail_if(*new != did2);

    ev    = _event_new(CORE_EVENT_CUSTOM_1);
    event = core_event_add(type, ev, _event_free, &did2);
    fail_if(event == NULL);

    event = core_event_add(CORE_EVENT_SIGNAL_EXIT, NULL, NULL, NULL);
    fail_if(event == NULL);

    core_main_loop_begin();

    /*
      Check internal fail cases:
       event_cb - 2 increments in first callback (another one was deleted)
       free     - 1 increment
   */
    fail_if(did2 != 2 + 1);  // 3

    /* Filter counter shouldn't change */
    fail_if(filter != 0);  // 0
}

EFL_END_TEST

#if 0
static int _log_dom;
#  define INF(...) EINA_LOG_DOM_INFO(_log_dom, __VA_ARGS__)

static Efl_Bool
_timer_quit_recursive(void *data EFL_UNUSED)
{
   INF("   _timer_quit_recursive: begin");
   core_main_loop_quit(); /* quits inner main loop */
   INF("   _timer_quit_recursive: end");
   return EFL_FALSE;
}

static Efl_Bool
_event_recursive_cb(void *data EFL_UNUSED, int type EFL_UNUSED, void *event EFL_UNUSED)
{
   static int guard = 0;

   /* If we enter this callback more than once, it's wrong! */
   fail_if(guard != 0);
   guard++;

   INF("  event_recursive_cb: begin");

   core_timer_add(1.0, _timer_quit_recursive, NULL);
   INF("   add 1.0s timer (once) to trigger _timer_quit_recursive");

   INF("   inner main loop begin (recurse)");
   core_main_loop_begin();
   INF("   inner main loop end (recurse)");

   core_main_loop_quit(); /* quits outer main loop */

   INF("   guard = %d", guard);
   INF("  event_recursive_cb: end");
   return EFL_FALSE;
}

EFL_START_TEST(core_test_core_main_loop_event_recursive)
{
   /* This test tests if the event handlers are really called only once when
    * recursive main loops are used and any number of events may have occurred
    * between the beginning and the end of recursive main loop.
    */
   Core_Event *e;
   int type;

   _log_dom = eina_log_domain_register("test", EINA_COLOR_CYAN);

   INF("main: begin");

   type = core_event_type_new();
   core_event_handler_add(type, _event_recursive_cb, NULL);
   e = core_event_add(type, NULL, NULL, NULL);
   INF(" add event to trigger cb1: event=%p", e);
   INF(" main loop begin");
   core_main_loop_begin();
   INF(" main loop end");

   INF("main: end");
}
EFL_END_TEST
#endif

EFL_START_TEST(core_test_core_app)
{
    int         argc_in   = 2;
    const char *argv_in[] = { "arg_str_1", "arg_str2" };

    int    argc_out = 0;
    char **argv_out = NULL;

    int    pargc;
    char **pargv;

    core_app_args_get(&pargc, &pargv);

    core_app_args_set(argc_in, argv_in);
    core_app_args_get(&argc_out, &argv_out);

    core_app_args_set(pargc, (const char **)pargv);

    fail_if(argc_in != argc_out);
    int i;
    for (i = 0; i < argc_out; i++)
    {
        fail_if(0 != strcmp(argv_in[i], argv_out[i]));
    }
}

EFL_END_TEST

void
core_test_ecore(TCase *tc)
{
    tcase_add_test(tc, core_test_core_init);
    tcase_add_test(tc, core_test_core_main_loop);
    tcase_add_test(tc, core_test_core_main_loop_fd_handler);
    tcase_add_test(tc, core_test_core_main_loop_fd_handler_valid_flags);
    tcase_add_test(tc, core_test_core_main_loop_fd_handler_activate_modify);
    tcase_add_test(tc, core_test_core_main_loop_event);
#if 0
   tcase_add_test(tc, core_test_core_main_loop_timer_inner);
   tcase_add_test(tc, core_test_core_main_loop_event_recursive);
#endif
    tcase_add_test(tc, core_test_core_app);
}
