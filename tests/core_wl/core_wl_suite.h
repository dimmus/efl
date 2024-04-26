#ifndef _ECORE_WL2_SUITE_H
# define _ECORE_WL2_SUITE_H

#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include <check.h>
#include "../efl_check.h"
#include <stdio.h>
#include <unistd.h>
#include <Ecore.h>
#include <Efl_Core_Wl.h>

void ecore_wl_test_init(TCase *tc);
void ecore_wl_test_display(TCase *tc);
void ecore_wl_test_window(TCase *tc);
void ecore_wl_test_input(TCase *tc);
void ecore_wl_test_output(TCase *tc);

#endif
