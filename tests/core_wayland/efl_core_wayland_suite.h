#ifndef _EFL_CORE_WAYLAND_SUITE_H
#define _EFL_CORE_WAYLAND_SUITE_H

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <unistd.h>
#include <check.h>

#include <Efl_Core.h>
#include <Efl_Core_Wl.h>

#include "../efl_check.h"

void test_session(TCase *tc);
void test_display(TCase *tc);
void test_window(TCase *tc);
void test_input(TCase *tc);
void test_output(TCase *tc);

#endif
