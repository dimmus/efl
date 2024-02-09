#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>

#ifdef _WIN32
#  include <evil_private.h> /* pipe */
#endif

#define EFL_NOLEGACY_API_SUPPORT
#include <Efl_Core.h>
#include "efl_app_suite.h"
#include "../efl_check.h"

static void
_eo_read_cb(void *data, const Efl_Event *info EFL_UNUSED)
{
    Efl_Bool *did = data;

    *did = EFL_TRUE;
    ecore_main_loop_quit();
}

EFL_START_TEST(ecore_test_efl_loop_fd)
{
    Efl_Bool did = EFL_FALSE;
    Eo      *fd;
    int      comm[2];
    int      ret;

    ret = pipe(comm);
    fail_if(ret != 0);

    fd = efl_add(EFL_LOOP_FD_CLASS,
                 efl_main_loop_get(),
                 efl_loop_fd_set(efl_added, comm[0]),
                 efl_event_callback_add(efl_added,
                                        EFL_LOOP_FD_EVENT_READ,
                                        _eo_read_cb,
                                        &did));
    fail_if(fd == NULL);

    ret = write(comm[1], &did, 1);
    fail_if(ret != 1);

    efl_loop_begin(efl_main_loop_get());

    close(comm[0]);
    close(comm[1]);

    fail_if(did == EFL_FALSE);
}

EFL_END_TEST

static void
_efl_del_cb(void *data, const Efl_Event *ev EFL_UNUSED)
{
    Efl_Bool *dead = data;

    *dead = EFL_TRUE;
}

EFL_START_TEST(ecore_test_efl_loop_fd_lifecycle)
{
    Efl_Bool did  = EFL_FALSE;
    Efl_Bool dead = EFL_FALSE;
    Eo      *fd;
    int      comm[2];
    int      ret;

    ret = pipe(comm);
    fail_if(ret != 0);

    fd = efl_add(
        EFL_LOOP_FD_CLASS,
        efl_main_loop_get(),
        efl_loop_fd_set(efl_added, comm[0]),
        efl_event_callback_add(efl_added,
                               EFL_LOOP_FD_EVENT_READ,
                               _eo_read_cb,
                               &did),
        efl_event_callback_add(efl_added, EFL_EVENT_DEL, _efl_del_cb, &dead));
    fail_if(fd == NULL);

    ret = write(comm[1], &did, 1);
    fail_if(ret != 1);

    efl_loop_begin(efl_main_loop_get());

    close(comm[0]);
    close(comm[1]);

    fail_if(did == EFL_FALSE);
    fail_if(dead == EFL_TRUE);

    efl_del(fd);
    fail_if(dead == EFL_FALSE);
}

EFL_END_TEST

void
efl_app_test_efl_loop_fd(TCase *tc)
{
    tcase_add_test(tc, ecore_test_efl_loop_fd);
    tcase_add_test(tc, ecore_test_efl_loop_fd_lifecycle);
}
