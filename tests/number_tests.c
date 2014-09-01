#include <assert.h>

#include "../lib/number.h"

#include "lib/mu_assert.h"
#include "../lib/helpers.h"

char *test_number_create_as_float() {
  Number *number = Number_create("1.45");
  mu_assert(number_type(number) == FloatType,    "Number_create with float string did not create the right type");
  mu_assert(number_value(number) == (FLOAT)1.45, "Number_create with float string did not set value correctly");

  number_destroy(number);

  return NULL;
}

char *test_number_create_as_int() {
  Number *number = Number_create("145");
  mu_assert(number_type(number) == IntegerType, "Number_create with int string did not create the right type");
  mu_assert(number_value(number) == (INT)145,   "Number_create with int string did not set value correctly");

  number_destroy(number);

  return NULL;
}

char *test_convert_to_same_type() {
  Number *number = Number_create("145");
  number_convert(number, IntegerType);

  mu_assert(number_type(number) == IntegerType, "number_convert changed type when it shouldn't have");
  mu_assert(number_value(number) == (INT)145,   "number_convert changed value when it shouldn't have");

  number_destroy(number);

  number = Number_create("3.14");
  number_convert(number, FloatType);

  mu_assert(number_type(number) == FloatType,     "number_convert changed type when it shouldn't have");
  mu_assert(number_value(number) == (FLOAT)3.14,  "number_convert changed value when it shouldn't have");

  number_destroy(number);

  return NULL;
}

char *test_convert_to_int() {
  Number *number = Number_create("3.14");
  number_convert(number, IntegerType);

  mu_assert(number_type(number) == (int)IntegerType,  "number_convert did not change type to int");
  mu_assert(number_value(number) == (INT)3,           "number_convert did not change type to int");

  number_destroy(number);
  return NULL;
}

char *test_convert_to_float() {
  Number *number = Number_create("3");
  number_convert(number, FloatType);

  mu_assert(number_type(number) == (int)FloatType,  "number_convert did not change type to int");
  mu_assert(number_value(number) == (FLOAT)3.0,  "number_convert did not change type to int");

  number_destroy(number);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_number_create_as_float);
  mu_run_test(test_number_create_as_int);
  mu_run_test(test_number_create_as_int);
  mu_run_test(test_convert_to_int);
  mu_run_test(test_convert_to_float);

  return NULL;
}

RUN_TESTS(all_tests);
