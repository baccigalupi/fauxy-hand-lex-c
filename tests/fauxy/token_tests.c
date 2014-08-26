#include <assert.h>

#include "../../lib/fauxy/token.h"
#include "../../lib/number.h"
#include "../../lib/dstring.h"

#include "../lib/mu_assert.h"

char *test_create_token() {
  Token *token =   Token_create(FX_TOKEN_NUMBER);

  mu_assert(object_type(token) == FX_TOKEN_NUMBER, "Token_create did not set the type");

  token_destroy(token);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create_token);

  return NULL;
}

RUN_TESTS(all_tests);
