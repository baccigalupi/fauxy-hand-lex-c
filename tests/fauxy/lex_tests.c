#include <assert.h>

#include "../../lib/fauxy/lex.h"
#include "../../lib/fauxy/token.h"
#include "../../lib/list.h"
#include "../../lib/node.h"
#include "../../lib/number.h"
#include "../../lib/helpers.h"

#include "../lib/mu_assert.h"

Token *get_token_from_list(List *list, int position) {
  check(list, "token not built by lex");

  Node *node;
  int i = 0;
  list_each(list, node) {
    if (i == position) {
      break;
    }
    i++;
  }

  check(node, "node not found");
  return node_value(node);
error:
  return NULL;
}

char *test_float() {
  List *list = lex("1.324");
  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER, "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)1.324, "lex did not assign number token value");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 1, "token column set incorrectly");

  list_clear_and_destroy(list);

  return NULL;
error:
  return "failed";
}

char *test_float_with_padding() {
  List *list = lex("    1.324   ");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER, "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)1.324, "lex did not assign number token value");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 5, "token column set incorrectly");

  list_clear_and_destroy(list);

  return NULL;
error:
  return "failed";
}

char *test_two_floats_with_padding() {
  List *list = lex("    1.324   4.0  ");
  mu_assert(list_length(list) == 2, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER, "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)1.324, "lex did not assign number token value");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 5, "token column set incorrectly");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER, "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)4.0, "lex did not assign number token value");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 13, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_line_end() {
  List *list = lex(" \n ");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STATEMENT_END, "lex did not build correct token type for line end");
  mu_assert(object_value(token) == NULL, "lex incorrectly assigned value to line end token");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 2, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_line_end_with_float() {
  List *list = lex(" \n   3.14");
  mu_assert(list_length(list) == 2, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STATEMENT_END, "lex did not build correct token type for line end");
  mu_assert(object_value(token) == NULL, "lex incorrectly assigned value to line end token");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 2, "token column set incorrectly");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER, "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)3.14, "lex did not assign number token value");
  mu_assert(token_line(token) == 2, "token line set incorrectly");

  mu_assert(token_column(token) == 4, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_integer() {
  List *list = lex("314");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER, "lex did not build token for number");
  mu_assert(token_number_value(token) == (INT)314, "lex did not assign number token value");

  return NULL;
error:
  return "failed";
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_float);
  mu_run_test(test_float_with_padding);
  mu_run_test(test_two_floats_with_padding);
  mu_run_test(test_line_end);
  mu_run_test(test_line_end_with_float);
  mu_run_test(test_integer);

  return NULL;
}

RUN_TESTS(all_tests);
