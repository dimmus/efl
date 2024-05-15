/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2013 Sam Spilsbury <smspillaz@gmail.com>
 */

#ifndef _EFL_TEST_RUNNER_H_
#define _EFL_TEST_RUNNER_H_

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <semaphore.h>
#include <stdlib.h>

#include "helpers.h"
#include "string-helpers.h"
#include "zalloc.h"

#include "Efl_Shared.h"

#include "efl-assert.h"
#include "efl-testsuite-data.h"

#ifdef NDEBUG
#error "Tests must not be built with NDEBUG defined, they rely on assert()."
#endif

#define EFL_TEST_SKIP_IS_FAILURE 0

/* Uncomment this line to print a warning line to stdout for every expected error received.
   Otherwise, expected errors are just silently ignored, greatly reducing the log size.
*/
//#define PRINT_EXPECTED_ERROR

#define DISABLE_ABORT_ON_CRITICAL_START \
   do { \
      int ___val = eina_log_abort_on_critical_get(); \
      eina_log_abort_on_critical_set(0)

#define DISABLE_ABORT_ON_CRITICAL_END \
      eina_log_abort_on_critical_set(___val); \
   } while (0)

#define EXPECT_ERROR_START \
  do { \
      DISABLE_ABORT_ON_CRITICAL_START; \
      Efl_Bool expect_error_start = EFL_FALSE; \
      do { \
        eina_log_print_cb_set(_efl_test_expect_error, &expect_error_start); \
      } while(0)


#define EXPECT_ERROR_END \
    eina_log_print_cb_set(NULL, NULL); \
    efl_assert_int_eq(expect_error_start, EFL_TRUE); \
    DISABLE_ABORT_ON_CRITICAL_END; \
  } while(0)

EFL_UNUSED static void
_efl_test_expect_error(const Eina_Log_Domain *d EFL_UNUSED, Eina_Log_Level level, const char *file EFL_UNUSED, const char *fnc, int line EFL_UNUSED, const char *fmt EFL_UNUSED, void *data, va_list args EFL_UNUSED)
{
   Efl_Bool *error = (Efl_Bool*) data;
   if (level == EINA_LOG_LEVEL_ERR) *error = EFL_TRUE;
#ifdef PRINT_EXPECTED_ERROR
   printf("EXPECTED ERROR %s\n", fnc);
#else
   (void)fnc;
#endif
}

/**
 * Printf-style argument attribute
 *
 * \param x Ordinality of the format string argument
 * \param y Ordinality of the argument to check against the format string
 *
 * \sa https://gcc.gnu.org/onlinedocs/gcc-3.2.1/gcc/Function-Attributes.html
 */
#if defined(__GNUC__) && __GNUC__ >= 4
#define EFL_PRINTF(x, y) __attribute__((__format__(__printf__, x, y)))
#else
#define EFL_PRINTF(x, y)
#endif

/** Test program entry
 *
 * Each invocation of TEST(), TEST_P(), or PLUGIN_TEST() will create one
 * more efl_test_entry in a custom named section in the final binary.
 * Iterating through the section then allows to iterate through all
 * the defined tests.
 *
 * \ingroup testharness_private
 */
struct efl_test_entry {
	const char *name;
	void (*run)(struct efl_testsuite_data *, void *);
	const void *table_data;
	size_t element_size;
	int n_elements;
} __attribute__ ((aligned (64)));

#define TEST_BEGIN(name, arg)						\
	static void name(struct efl_testsuite_data *_efl_suite_data, arg)

#define TEST_BEGIN_NO_ARG(name)						\
	static void name(struct efl_testsuite_data *_efl_suite_data EFL_UNUSED)

#define TEST_COMMON(func, name, data, size, n_elem)			\
	static void func(struct efl_testsuite_data *, void *);		\
									\
	const struct efl_test_entry test##name			\
		__attribute__ ((used, section ("test_section"))) =	\
	{								\
		#name, func, data, size, n_elem				\
	};

#define NO_ARG_TEST(name)						\
	TEST_COMMON(wrap##name, name, NULL, 0, 1)			\
	static void name(struct efl_testsuite_data *);			\
	static void wrap##name(struct efl_testsuite_data *_efl_suite_data,\
			       void *data)				\
	{								\
		(void) data;						\
		name(_efl_suite_data);					\
	}								\
									\
	TEST_BEGIN_NO_ARG(name)

#define ARG_TEST(name, test_data)					\
	TEST_COMMON(name, name, test_data,				\
		    sizeof(test_data[0]),				\
		    ARRAY_LENGTH(test_data))				\
	TEST_BEGIN(name, void *data)					\

/** Add a test with no parameters
 *
 * This defines one test as a new function. Use this macro in place of the
 * function signature and put the function body after this.
 *
 * \param name Name for the test, must be a valid function name.
 *
 * \ingroup testharness
 */
#define TEST(name) NO_ARG_TEST(name)

/** Add an array driven test with a parameter
 *
 * This defines an array of tests as a new function. Use this macro in place
 * of the function signature and put the function body after this. The function
 * will be executed once for each element in \c data_array, passing the
 * element as the argument <tt>void *data</tt> to the function.
 *
 * This macro is not usable if fixture setup is using
 * efl_test_harness_execute_as_plugin().
 *
 * \param name Name for the test, must be a valid function name.
 * \param data_array A static const array of any type. The length will be
 * recorded automatically.
 *
 * \ingroup testharness
 */
#define TEST_P(name, data_array) ARG_TEST(name, data_array)

/** Add a test with weston_compositor argument
 *
 * This defines one test as a new function. Use this macro in place of the
 * function signature and put the function body after this. The function
 * will have one argument <tt>struct weston_compositor *compositor</tt>.
 *
 * This macro is only usable if fixture setup is using
 * efl_test_harness_execute_as_plugin().
 *
 * \param name Name for the test, must be a valid function name.
 *
 * \ingroup testharness
 */
#define PLUGIN_TEST(name)						\
	TEST_COMMON(wrap##name, name, NULL, 0, 1)			\
	static void name(struct efl_testsuite_data *,			\
			 struct weston_compositor *);			\
	static void wrap##name(struct efl_testsuite_data *_efl_suite_data,\
			       void *compositor)			\
	{								\
		name(_efl_suite_data, compositor);			\
	}								\
	TEST_BEGIN(name, struct weston_compositor *compositor)

/** Get test suite data structure
 *
 * This returns the shared test suite data structure, to be used in
 * any test which is declared with TEST(), TEST_P(), or PLUGIN_TEST().
 *
 * \return Test suite data structure
 * \ingroup testharness
 */
#define TEST_GET_SUITE_DATA() _efl_suite_data

void
testlog(const char *fmt, ...) EFL_PRINTF(1, 2);

const char *
get_test_name(void);

int
get_test_fixture_index(void);

/** Metadata for fixture setup array elements
 *
 * Every type used as a fixture setup array's elements needs one member of
 * this type, initialized.
 *
 * \sa DECLARE_FIXTURE_SETUP_WITH_ARG()
 *
 * \ingroup testharness
 */
struct fixture_metadata {
	/** Human friendly name of the fixture setup */
	const char *name;
};

/** Fixture setup array record
 *
 * Helper to store the attributes of the data array passed in to
 * DECLARE_FIXTURE_SETUP_WITH_ARG().
 *
 * \ingroup testharness_private
 */
struct fixture_setup_array {
	const void *array;
	size_t element_size;
	int n_elements;
	size_t meta_offset;
};

const struct fixture_setup_array *
fixture_setup_array_get_(void);

/** Test harness context
 *
 * \ingroup testharness
 */
struct efl_test_harness;

enum test_result_code
fixture_setup_run_(struct efl_test_harness *harness, const void *arg_);

/** Register a fixture setup function
 *
 * This registers the given (preferably static) function to be used for setting
 * up any fixtures you might need. The function must have the signature:
 *
 * \code
 * enum test_result_code func_(struct efl_test_harness *harness)
 * \endcode
 *
 * The function must call one of efl_test_harness_execute_standalone(),
 * efl_test_harness_execute_as_plugin() or
 * efl_test_harness_execute_as_client() passing in the \c harness argument,
 * and return the return value from that call. The function can also return a
 * test_result_code on its own if it does not want to run the tests,
 * e.g. RESULT_SKIP or RESULT_HARD_ERROR.
 *
 * The function will be called once to run all tests.
 *
 * \param func_ The function to be used as fixture setup.
 *
 * \ingroup testharness
 */
#define DECLARE_FIXTURE_SETUP(func_)					\
	enum test_result_code						\
	fixture_setup_run_(struct efl_test_harness *harness,		\
			   const void *arg_ EFL_UNUSED)				\
	{								\
		return func_(harness);					\
	}

/** Register a fixture setup function with a data array
 *
 * This registers the given (preferably static) function to be used for setting
 * up any fixtures you might need. The function must have the signature:
 *
 * \code
 * enum test_result_code func_(struct efl_test_harness *harness, typeof(array_[0]) *arg)
 * \endcode
 *
 * The function must call one of efl_test_harness_execute_standalone(),
 * efl_test_harness_execute_as_plugin() or
 * efl_test_harness_execute_as_client() passing in the \c harness argument,
 * and return the return value from that call. The function can also return a
 * test_result_code on its own if it does not want to run the tests,
 * e.g. RESULT_SKIP or RESULT_HARD_ERROR.
 *
 * The function will be called once with each element of the array pointed to
 * by \c arg, so that all tests would be repeated for each element in turn.
 *
 * \param func_ The function to be used as fixture setup.
 * \param array_ A static const array of arbitrary type.
 * \param meta_ Name of the field with type struct fixture_metadata.
 *
 * \ingroup testharness
 */
#define DECLARE_FIXTURE_SETUP_WITH_ARG(func_, array_, meta_)		\
	const struct fixture_setup_array *				\
	fixture_setup_array_get_(void)					\
	{								\
		static const struct fixture_setup_array arr = {		\
			.array = array_,				\
			.element_size = sizeof(array_[0]),		\
			.n_elements = ARRAY_LENGTH(array_),		\
			.meta_offset = offsetof(typeof(array_[0]), meta_),	\
		};								\
		TYPEVERIFY(const struct fixture_metadata *, &array_[0].meta_);	\
		return &arr;						\
	}								\
									\
	enum test_result_code						\
	fixture_setup_run_(struct efl_test_harness *harness,		\
			   const void *arg_)				\
	{								\
		typeof(array_[0]) *arg = arg_;				\
		return func_(harness, arg);				\
	}

// enum test_result_code
// efl_test_harness_execute_as_client(struct efl_test_harness *harness,
// 				      const struct compositor_setup *setup);

// enum test_result_code
// efl_test_harness_execute_as_plugin(struct efl_test_harness *harness,
// 				      const struct compositor_setup *setup);

enum test_result_code
efl_test_harness_execute_standalone(struct efl_test_harness *harness);

#endif
