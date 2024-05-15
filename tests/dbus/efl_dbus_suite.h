#ifndef _EFL_DBUS_SUITE_H
#define _EFL_DBUS_SUITE_H

#include <check.h>
#include "../efl_check.h"
void efl_dbus_test_efl_dbus_init(TCase *tc);
void efl_dbus_test_efl_dbus_model(TCase *tc);
void efl_dbus_test_efl_dbus_model_connection(TCase *tc);
void efl_dbus_test_efl_dbus_model_object(TCase *tc);
void efl_dbus_test_efl_dbus_model_proxy(TCase *tc);
void efl_dbus_test_fake_server_efl_dbus_model_proxy(TCase *tc);
void efl_dbus_test_efl_dbus_model_method(TCase *tc);
void efl_dbus_test_efl_dbus_model_signal(TCase *tc);
void efl_dbus_test_efl_dbus_object(TCase *tc);
void efl_dbus_test_efl_dbus_message(TCase *tc);
void efl_dbus_test_efl_dbus_signal_handler(TCase *tc);
void efl_dbus_test_efl_dbus_proxy(TCase *tc);
void efl_dbus_test_efl_dbus_pending_cancel(TCase *tc);
#endif
