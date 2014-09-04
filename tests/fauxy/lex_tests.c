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

  mu_assert(object_type(token) == FX_TOKEN_LINE_END, "lex did not build correct token type for line end");
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

  mu_assert(object_type(token) == FX_TOKEN_LINE_END, "lex did not build correct token type for line end");
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

char *test_single_quoted_string_no_space() {
  List *list = lex("'hello'");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STRING, "lex did not build right token type for string");
  mu_assert(strcmp(token_string_value(token), "hello") == 0,  "lex did not assign right value for string");

  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 1, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_double_quoted_string_no_space() {
  List *list = lex("\"hello\"");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STRING, "lex did not build right token type for string");
  mu_assert(strcmp(token_string_value(token), "hello") == 0,  "lex did not assign right value for string");

  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 1, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_single_quoted_string_with_space() {
  List *list = lex(" 'hello world' ");

  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STRING, "lex did not build right token type for string");
  mu_assert(strcmp(token_string_value(token), "hello world") == 0,  "lex did not assign right value for string");

  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 2, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_double_quoted_string_with_space() {
  List *list = lex(" \"hello world\" ");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STRING, "lex did not build right token type for string");
  mu_assert(strcmp(token_string_value(token), "hello world") == 0,  "lex did not assign right value for string");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 2, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_strings_with_line_break() {
  List *list = lex(" \"hello\nworld\" 'wha?'");
  mu_assert(list_length(list) == 2, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STRING, "lex did not build right token type for string");
  mu_assert(strcmp(token_string_value(token), "hello\nworld") == 0,  "lex did not assign right value for string");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 2, "token column set incorrectly");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STRING, "lex did not build right token type for string");
  mu_assert(strcmp(token_string_value(token), "wha?") == 0,  "lex did not assign right value for string");
  mu_assert(token_line(token) == 2, "token line set incorrectly");
  mu_assert(token_column(token) == 8, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_block_comment() {
  List *list = lex(" /* hello\ncomment */ ");

  mu_assert(list_length(list) == 0, "lex created wrong number of tokens");

  return NULL;
}

char *test_block_comment_affect_line() {
  List *list = lex("/* hello\ncomment */\n 3.14");
  mu_assert(list_length(list) == 2, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_LINE_END, "lex did not build correct token type for line end");
  mu_assert(object_value(token) == NULL, "lex incorrectly assigned value to line end token");
  mu_assert(token_line(token) == 2, "token line set incorrectly");
  mu_assert(token_column(token) == 11, "token column set incorrectly");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER,    "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)3.14, "lex did not assign number token value");
  mu_assert(token_line(token) == 3,                   "token line set incorrectly");
  mu_assert(token_column(token) == 2,                 "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_line_comment() {
  List *list = lex("3.14 // hello comment");

  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  return NULL;
}

char *test_line_comment_affect_line() {
  List *list = lex("1 // hello comment\n 3.14");
  mu_assert(list_length(list) == 3, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER,    "lex did not build token for number");
  mu_assert(token_number_value(token) == (INT)1, "lex did not assign number token value");
  mu_assert(token_line(token) == 1,                   "token line set incorrectly");
  mu_assert(token_column(token) == 1,                 "token column set incorrectly");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_LINE_END, "lex did not build correct token type for line end");
  mu_assert(object_value(token) == NULL, "lex incorrectly assigned value to line end token");
  mu_assert(token_line(token) == 1, "token line set incorrectly");
  mu_assert(token_column(token) == 19, "token column set incorrectly");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER,    "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)3.14, "lex did not assign number token value");
  mu_assert(token_line(token) == 2,                   "token line set incorrectly");
  mu_assert(token_column(token) == 2,                 "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_statement_end() {
  List *list = lex("'hello'; 1234; 3.14");
  mu_assert(list_length(list) == 5, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STRING,           "lex did not build token for string");
  mu_assert(strcmp(token_string_value(token), "hello") == 0, "lex did not assign number token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STATEMENT_END,    "lex did not build token for semicolon");
  mu_assert(object_value(token) == NULL, "lex incorrectly assigned value to line end token");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER,    "lex did not build token for number");
  mu_assert(token_number_value(token) == (INT)1234,   "lex did no assign value to number");

  token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_STATEMENT_END,    "lex did not build token for semicolon");
  mu_assert(object_value(token) == NULL, "lex incorrectly assigned value to line end token");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_NUMBER,    "lex did not build token for number");
  mu_assert(token_number_value(token) == (FLOAT)3.14,   "lex did no assign value to number");

  return NULL;
error:
  return "failed";
}

char *test_regex() {
  List *list = lex(" /[a-z]/i ");

  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_REGEX, "lex did not build right token type for regex");
  mu_assert(strcmp(token_string_value(token),"/[a-z]/i") == 0, "lex did not build right value for type regex");

  return NULL;
error:
  return "failed";
}

char *test_regex_with_space() {
  List *list = lex(" /[a-z] [0-9]/i ");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_REGEX, "lex did not build right token type for regex");
  mu_assert(strcmp(token_string_value(token),"/[a-z] [0-9]/i") == 0, "lex did not build right value for type regex");

  return NULL;
error:
  return "failed";
}

char *test_basic_identifier() {
  List *list = lex(" gerbil ");
  mu_assert(list_length(list) == 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  mu_assert(object_type(token) == FX_TOKEN_ID, "lex did not build right token type for id");
  mu_assert(strcmp(token_string_value(token),"gerbil") == 0, "lex did not build right value for type id");

  return NULL;
error:
  return "failed";
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_float);
  mu_run_test(test_float_with_padding);
  mu_run_test(test_two_floats_with_padding);
  mu_run_test(test_integer);

  mu_run_test(test_line_end);
  mu_run_test(test_line_end_with_float);

  mu_run_test(test_single_quoted_string_no_space);
  mu_run_test(test_double_quoted_string_no_space);
  mu_run_test(test_single_quoted_string_with_space);
  mu_run_test(test_double_quoted_string_with_space);
  mu_run_test(test_strings_with_line_break);

  mu_run_test(test_block_comment);
  mu_run_test(test_block_comment_affect_line);
  mu_run_test(test_line_comment);
  mu_run_test(test_line_comment_affect_line);

  mu_run_test(test_statement_end);

  mu_run_test(test_regex);
  mu_run_test(test_regex_with_space);

  // mu_run_test(test_basic_identifier);

  return NULL;
}

RUN_TESTS(all_tests);
