#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdbool.h>
#include <libgen.h> /* basename() */

#include <Efl_Shared.h>
#include <Efl_Core.h>
#include <Efl_Dbus_Model.h>

#include "test_efl_dbus_fake_server.h"
#include "test_efl_dbus_model.h"
#include "efl_dbus_suite.h"

#define ARGUMENT_A      "arg0"
#define ARGUMENT_B      "arg1"
#define ARGUMENT_RESULT "arg2"

static Eo                         *fake_server_object = NULL;
static Eo                         *fake_server_proxy  = NULL;
static Efl_Dbus_Service_Interface *fake_server        = NULL;
static Fake_Server_Data            fake_server_data   = { 0 };
static Eo                         *method             = NULL;

static void
_setup(void)
{
  char buf[1024];
  snprintf(buf, sizeof(buf), FAKE_SERVER_BUS ".%s", basename(__FILE__));
  fake_server = fake_server_start(&fake_server_data, buf);

  fake_server_object =
    efl_add(EFL_DBUS_MODEL_OBJECT_CLASS,
            efl_main_loop_get(),
            efl_dbus_model_connect(efl_added,
                                   EFL_DBUS_CONNECTION_TYPE_SESSION,
                                   NULL,
                                   EFL_FALSE),
            efl_dbus_model_object_bus_set(efl_added, buf),
            efl_dbus_model_object_path_set(efl_added, FAKE_SERVER_PATH));
  ck_assert_ptr_ne(NULL, fake_server_object);

  fake_server_proxy =
    efl_dbus_model_proxy_from_object_get(fake_server_object,
                                         FAKE_SERVER_INTERFACE);
  ck_assert_ptr_ne(NULL, fake_server_proxy);

  method = efl_dbus_model_method_from_proxy_get(fake_server_proxy,
                                                FAKE_SERVER_SUM_METHOD_NAME);
  ck_assert_ptr_ne(NULL, method);
}

static void
_teardown(void)
{
  efl_del(fake_server_object);

  fake_server_stop(fake_server);
}

EFL_START_TEST(properties_get)
{
  Eina_Iterator *properties = NULL;
  properties                = efl_model_properties_get(method);
  ck_assert_ptr_ne(NULL, properties);

  const unsigned int expected_properties_count =
    3; // a, b and result arguments of 'sum' method
  unsigned int actual_properties_count = 0;
  const char  *prop;

  EINA_ITERATOR_FOREACH(properties, prop)
    actual_properties_count++;
  eina_iterator_free(properties);
  ck_assert_int_eq(expected_properties_count, actual_properties_count);
}

EFL_END_TEST

EFL_START_TEST(property_get)
{
   // Input only property returns error
  Eina_Value *v;
  Eina_Value  i  = EINA_VALUE_EMPTY;
  int         iv = -1;

  v = efl_model_property_get(method, ARGUMENT_A);
  fail_if(eina_value_type_get(v) != EINA_VALUE_TYPE_ERROR);
  eina_value_free(v);

  v = efl_model_property_get(method, ARGUMENT_RESULT);
  eina_value_setup(&i, EINA_VALUE_TYPE_INT);
  fail_if(eina_value_convert(v, &i) != EFL_TRUE);
  fail_if(eina_value_int_get(&i, &iv) != EFL_TRUE);
  fail_if(iv != 0);
  eina_value_flush(&i);
  eina_value_free(v);

   // Nonexistent property returns error
  v = efl_model_property_get(method, "nonexistent");
  fail_if(eina_value_type_get(v) != EINA_VALUE_TYPE_ERROR);
  eina_value_free(v);
}

EFL_END_TEST

static Eina_Value
_expected_error(void                          *data,
                const Eina_Value               v,
                const Eina_Future *dead_future EFL_UNUSED)
{
  fail_if(eina_value_type_get(&v) != EINA_VALUE_TYPE_ERROR);

  if (data)
  {
    Eina_Error *expected = data;
    Eina_Error  error;

    eina_value_error_get(&v, &error);

    fail_if(*expected != error);
  }

  core_main_loop_quit();

  return v;
}

EFL_START_TEST(property_set)
{
   // Output argument returns error
  Eina_Future *future;
  Eina_Value   dummy = EINA_VALUE_EMPTY;

  future = efl_model_property_set(method, ARGUMENT_RESULT, &dummy);
  eina_future_then(future, _expected_error, NULL, NULL);
}

EFL_END_TEST

static void
_test_method_children_count(Eo *efl_model)
{
  check_efl_model_children_count_eq(efl_model, 0);
}

EFL_START_TEST(children_count)
{
  _test_method_children_count(method);
}

EFL_END_TEST

EFL_START_TEST(children_slice_get)
{
  Eina_Future *future;

  future = efl_model_children_slice_get(method, 1, 1);
  eina_future_then(future,
                   _expected_error,
                   &EFL_MODEL_ERROR_NOT_SUPPORTED,
                   NULL);

  core_main_loop_begin();
}

EFL_END_TEST

EFL_START_TEST(child_add)
{
  Eo *child;
  child = efl_model_child_add(method);
  ck_assert_ptr_eq(NULL, child);
}

EFL_END_TEST

EFL_START_TEST(child_del)
{
   // efl_model_child_del always returns ERROR
  Eo *child = NULL;
  efl_model_child_del(method, child);
   //ck_assert_int_eq(EFL_MODEL_LOAD_STATUS_ERROR, status);
}

EFL_END_TEST

EFL_START_TEST(call)
{
  check_efl_model_property_int_set(method, ARGUMENT_A, 12345678);
  check_efl_model_property_int_set(method, ARGUMENT_B, 87654321);

  efl_dbus_model_method_call(method);

  efl_model_wait_for_event(method, EFL_DBUS_MODEL_METHOD_EVENT_SUCCESSFUL_CALL);
  check_efl_model_property_int_eq(method, ARGUMENT_RESULT, 99999999);
}

EFL_END_TEST

void
efl_dbus_test_efl_dbus_model_method(TCase *tc)
{
  tcase_add_checked_fixture(tc, _setup, _teardown);
  tcase_add_test(tc, properties_get);
  tcase_add_test(tc, property_get);
  tcase_add_test(tc, property_set);
  tcase_add_test(tc, children_count);
  tcase_add_test(tc, children_slice_get);
  tcase_add_test(tc, child_add);
  tcase_add_test(tc, child_del);
  tcase_add_test(tc, call);
}
