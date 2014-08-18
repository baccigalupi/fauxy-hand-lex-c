#include <assert.h>

#include "../../lib/fauxy/token.h"
#include "../../lib/fauxy/bit.h"
#include "../../lib/number.h"
#include "../../lib/dstring.h"

#include "../lib/mu_assert.h"

char *test_create_token() {
  Number *number = Number_create("3.14");
  FauxyBit *bit = FauxyBit_create(FauxyNumberType, number);
  FauxyToken *token = FauxyToken_create(FX_TOKEN_NUMBER, bit);

  mu_assert(fauxy_token_type(token) == FX_TOKEN_NUMBER, "FauxyToken_create did not set the type");
  mu_assert(fauxy_token_value(token) == bit, "FauxyToken_create did not set the value");

  number_destroy(number);
  fauxy_bit_destroy(bit);
  fauxy_token_destroy(token);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create_token);

  return NULL;
}

RUN_TESTS(all_tests);
