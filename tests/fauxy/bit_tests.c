#include <assert.h>

#include "../../lib/fauxy/bit.h"
#include "../../lib/number.h"

#include "../lib/mu_assert.h"

char *test_fauxy_bit_create() {
  Number *number = Number_create("1.223");
  FauxyBit *bit = FauxyBit_create(FauxyNumberType, number);

  mu_assert(fauxy_bit_type(bit) == FauxyNumberType, "FauxyBit_create did not set the type");
  mu_assert(fauxy_bit_value(bit) == number,         "FauxyBit_create did not set the type");

  fauxy_bit_destroy(bit);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_fauxy_bit_create);

  return NULL;
}

RUN_TESTS(all_tests);
