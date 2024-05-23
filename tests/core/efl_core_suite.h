#ifndef _CORE_SUITE_H
#define _CORE_SUITE_H

#include <check.h>
#include "../efl_check.h"

extern int timeout_reached;

void core_test_ecore(TCase *tc);
void core_test_core_idle(TCase *tc);
void core_test_core_poller(TCase *tc);
void core_test_core_x(TCase *tc);
// void core_test_core_imf(TCase *tc);
// void core_test_core_audio(TCase *tc);
void core_test_timer(TCase *tc);
// void core_test_core_evas(TCase *tc);
void core_test_animator(TCase *tc);
void core_test_core_thread_eina_thread_queue(TCase *tc);
void core_test_core_thread_eina_thread_queue2(TCase *tc);
// void core_test_core_fb(TCase *tc);
// void core_test_core_input(TCase *tc);
// void core_test_core_file(TCase *tc);
void core_test_core_job(TCase *tc);
void core_test_core_args(TCase *tc);
void core_test_core_pipe(TCase *tc);
// void core_test_core_evas_selection(TCase *tc);

#endif /* _CORE_SUITE_H */
