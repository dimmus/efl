#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "efl_dbus_suite.h"
#include "../efl_check.h"
#include <Efl_Dbus.h>

static const Efl_Test_Case etc[] = {
    {                                 "efl_dbus_init",efl_dbus_test_efl_dbus_init                                                      },
    {                                "efl_dbus_model",            efl_dbus_test_efl_dbus_model},
    {                     "efl_dbus_model_connection", efl_dbus_test_efl_dbus_model_connection},
    {                         "efl_dbus_model_object",     efl_dbus_test_efl_dbus_model_object},
    {                          "efl_dbus_model_proxy",      efl_dbus_test_efl_dbus_model_proxy},
    {"efl_dbus_test_fake_server_efl_dbus_model_proxy",
     efl_dbus_test_fake_server_efl_dbus_model_proxy                                           },
    {                         "efl_dbus_model_method",     efl_dbus_test_efl_dbus_model_method},
    {                         "efl_dbus_model_signal",     efl_dbus_test_efl_dbus_model_signal},
    {                 "efl_dbus_test_efl_dbus_object",           efl_dbus_test_efl_dbus_object},
    {                "efl_dbus_test_efl_dbus_message",          efl_dbus_test_efl_dbus_message},
    {         "efl_dbus_test_efl_dbus_signal_handler",
     efl_dbus_test_efl_dbus_signal_handler                                                    },
    {                  "efl_dbus_test_efl_dbus_proxy",            efl_dbus_test_efl_dbus_proxy},
    {         "efl_dbus_test_efl_dbus_pending_cancel",
     efl_dbus_test_efl_dbus_pending_cancel                                                    },
    {                                            NULL,                                    NULL}
};

SUITE_INIT(efl_dbus)
{
    ck_assert_int_eq(efl_dbus_init(), 1);
}

SUITE_SHUTDOWN(efl_dbus)
{
   //T6814
    ck_assert_int_eq(efl_dbus_shutdown(), 0);
}

int
main(int argc, char **argv)
{
    int failed_count;

    if (!_efl_test_option_disp(argc, argv, etc)) return 0;

#ifdef NEED_RUN_IN_TREE
    putenv("EFL_RUN_IN_TREE=1");
#endif

    failed_count = _efl_suite_build_and_run(argc - 1,
                                            (const char **)argv + 1,
                                            "Efl_Dbus",
                                            etc,
                                            SUITE_INIT_FN(efl_dbus),
                                            SUITE_SHUTDOWN_FN(efl_dbus));

    return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
