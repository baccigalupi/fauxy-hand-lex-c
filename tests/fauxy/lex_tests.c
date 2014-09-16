#include <assert.h>

#include "../../lib/fauxy/lex.h"
#include "../../lib/fauxy/token.h"
#include "../../lib/bricks/list.h"
#include "../../lib/bricks/node.h"
#include "../../lib/bricks/number.h"
#include "../../lib/bricks/bricks.h"

#include "../lib/spec.h"

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

  spec_describe("Lexing a float with no padding");

  assert_equal(object_type(token), FX_TOKEN_NUMBER,     "token type");
  assert_equal(token_number_value(token), (FLOAT)1.324, "token value");
  assert_equal(token_line(token), 1,                    "token line");
  assert_equal(token_column(token), 1,                  "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_float_with_padding() {
  spec_describe("Lexing a float with padding");

  List *list = lex("    1.324   ");
  assert_equal(list_length(list), 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  assert_equal(token_number_value(token), (FLOAT)1.324, "token value");
  assert_equal(token_line(token), 1, "token line");
  assert_equal(token_column(token), 5, "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_two_floats_with_padding() {
  spec_describe("Lexing two floats with padding");
  List *list = lex("    1.324   4.0  ");
  assert_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  assert_equal(token_number_value(token), (FLOAT)1.324, "token value");
  assert_equal(token_line(token), 1, "token line");
  assert_equal(token_column(token), 5, "token column");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER, "second token type");
  assert_equal(token_number_value(token), (FLOAT)4.0, "second token value");
  assert_equal(token_line(token), 1, "second token line");
  assert_equal(token_column(token), 13, "second token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_line_end() {
  spec_describe("Lexing line end with padding");
  List *list = lex(" \n ");
  assert_equal(list_length(list), 1, "list_length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_LINE_END, "token type");
  assert_equal(object_value(token), NULL, "token value");
  assert_equal(token_line(token), 1, "token line");
  assert_equal(token_column(token), 2, "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_line_end_with_float() {
  spec_describe("Lexing line end with float and padding");
  List *list = lex(" \n   3.14");
  assert_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_LINE_END, "token type for line end");
  assert_equal(object_value(token), NULL, "token value line end");
  assert_equal(token_line(token), 1, "token line for line end");
  assert_equal(token_column(token), 2, "token column for line end");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER, "token type for number");
  assert_equal(token_number_value(token), (FLOAT)3.14, "token value for number");
  assert_equal(token_line(token), 2, "token line for number");
  assert_equal(token_column(token), 4, "token column for number");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_integer() {
  spec_describe("Lexing interger");
  List *list = lex("314");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  assert_equal(token_number_value(token), (INT)314, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_single_quoted_string_no_space() {
  spec_describe("Lexing single simple quoted strings");

  List *list = lex("'hello'");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "token type");
  assert_strings_equal(token_string_value(token), "hello",  "token value");
  assert_equal(token_line(token), 1, "token line");
  assert_equal(token_column(token), 1, "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_double_quoted_string_no_space() {
  spec_describe("Lexing double quoted basic string");

  List *list = lex("\"hello\"");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "token type");
  assert_strings_equal(token_string_value(token), "hello",  "token value");
  assert_equal(token_line(token), 1, "token line set incorrectly");
  assert_equal(token_column(token), 1, "token column set incorrectly");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_single_quoted_string_with_space() {
  spec_describe("Lexing single quoted string containing space, padded");

  List *list = lex(" 'hello world' ");

  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "token type");
  assert_strings_equal(token_string_value(token), "hello world",  "token value");
  assert_equal(token_line(token), 1, "token line");
  assert_equal(token_column(token), 2, "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_double_quoted_string_with_space() {
  spec_describe("Lexing double quoted string containing space, padded");

  List *list = lex(" \"hello world\" ");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "token type");
  assert_strings_equal(token_string_value(token), "hello world",  "token value");
  assert_equal(token_line(token), 1, "token line");
  assert_equal(token_column(token), 2, "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_strings_with_line_break() {
  spec_describe("Multiple string types, one containing line break");

  List *list = lex(" \"hello\nworld\" 'wha?'");
  assert_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "token type");
  assert_strings_equal(token_string_value(token), "hello\nworld",  "token value");
  assert_equal(token_line(token), 1, "token line set incorrectly");
  assert_equal(token_column(token), 2, "token column set incorrectly");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "token type");
  assert_strings_equal(token_string_value(token), "wha?",  "token value");
  assert_equal(token_line(token), 2, "token line");
  assert_equal(token_column(token), 8, "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_block_comment() {
  spec_describe("Lexing block comments");
  List *list = lex(" /* hello\ncomment */ ");

  assert_equal(list_length(list), 0, "list length");

  token_list_free(list);

  return NULL;
}

char *test_block_comment_affect_line() {
  spec_describe("Lexing block comment followed by float");

  List *list = lex("/* hello\ncomment */\n 3.14");
  assert_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_LINE_END, "line end token type");
  assert_equal(object_value(token), NULL, "token value");
  assert_equal(token_line(token), 2, "token line");
  assert_equal(token_column(token), 11, "token column");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER,    "number token type");
  assert_equal(token_number_value(token), (FLOAT)3.14, "token value");
  assert_equal(token_line(token), 3,                   "token line");
  assert_equal(token_column(token), 2,                 "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_line_comment() {
  spec_describe("Lexing line comment preceded by float");

  List *list = lex("3.14 // hello comment");

  assert_equal(list_length(list), 1, "list length");

  token_list_free(list);

  return NULL;
}

char *test_line_comment_affect_line() {
  spec_describe("Lexing int line comment and float on next line");

  List *list = lex("1 // hello comment\n 3.14");
  assert_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER,    "int token type");
  assert_equal(token_number_value(token), (INT)1,      "token value");
  assert_equal(token_line(token), 1,                   "token line");
  assert_equal(token_column(token), 1,                 "token column");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_LINE_END, "line end token type");
  assert_equal(object_value(token), NULL,             "token value");
  assert_equal(token_line(token), 1,                  "token line");
  assert_equal(token_column(token), 19,               "token column");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER,    "float number type");
  assert_equal(token_number_value(token), (FLOAT)3.14, "token value");
  assert_equal(token_line(token), 2,                   "token line");
  assert_equal(token_column(token), 2,                 "token column");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_statement_end() {
  spec_describe("Lexing multiple statements seperated by semicolons");

  List *list = lex("'hello'; 1234; 3.14");
  assert_equal(list_length(list), 5, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING,           "single string token type");
  assert_strings_equal(token_string_value(token), "hello", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STATEMENT_END,    "semicolon token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER,    "int token type");
  assert_equal(token_number_value(token), (INT)1234,   "int token value");

  token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STATEMENT_END,    "semicolon token type");
  assert_equal(object_value(token), NULL, "semicolon token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER,    "float number type");
  assert_equal(token_number_value(token), (FLOAT)3.14,   "float number value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_regex() {
  spec_describe("Lexing regex with trailing modifier, padded");

  List *list = lex(" /[a-z]/i ");

  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_REGEX, "token type");
  assert_equal(strcmp(token_string_value(token),"/[a-z]/i"), 0, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_regex_with_space() {
  spec_describe("Lexing regex containing white space and with trailing modifier, padded");

  List *list = lex(" /[a-z] [0-9]/i ");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_REGEX, "token type");
  assert_equal(strcmp(token_string_value(token),"/[a-z] [0-9]/i"), 0, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_basic_identifier() {
  spec_describe("Lexing padded identifier");

  List *list = lex(" gerbil ");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "token type");
  assert_strings_equal(token_string_value(token),"gerbil", "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_global_identifier() {
  spec_describe("Lexing global identifier with padding");

  List *list = lex(" Gerbil ");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_GLOBAL_ID, "token type");
  assert_strings_equal(token_string_value(token),"Gerbil", "lex did not build right value for type id");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_number_starting_with_minus_sign() {
  spec_describe("Lexing number starting with minus sign");

  List *list = lex(" -1.23");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  assert_equal(token_number_value(token), (FLOAT)(-1.23), "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_exponential_numbers() {
  spec_describe("Lexing exponential numbers");

  List *list = lex(" 1E-8");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  assert_equal(token_number_value(token), (FLOAT)(1E-8), "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_ids_starting_as_numbers() {
  spec_describe("Lexing identifiers starting with numbers");

  List *list = lex(" 123foo");

  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID,  "token type");
  assert_strings_equal(token_string_value(token), "123foo", "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_ids_with_hyphens_and_underscores() {
  spec_describe("Lexing ids with numbers hyphens and underscores");

  List *list = lex(" 123-foo_bar- ");

  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID,  "token type");
  assert_strings_equal(token_string_value(token), "123-foo_bar-", "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call() {
  spec_describe("Lexing dot attribute selection");

  List *list = lex(" file.open ");
  assert_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "id token type");
  assert_strings_equal(token_string_value(token), "file", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ATTRIBUTE_SELECTOR, "dot attribute selector token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "id token type");
  assert_strings_equal(token_string_value(token), "open", "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call_and_argument() {
  spec_describe("Lexing dot method calls with argument");

  List *list = lex(" file.open('w') ");
  assert_equal(list_length(list), 6, "list length");

  Token *token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_GROUP_START, "group start token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  assert_strings_equal(token_string_value(token), "w", "token value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_GROUP_END, "group end token type");
  assert_equal(object_value(token), NULL, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call_and_arguments() {
  spec_describe("Lexing identifier with dot method call and arguments");

  List *list = lex(" gerbil.talk('squeak','bark') ");
  assert_equal(list_length(list), 8, "list length");

  Token *token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_COMMA, "comma token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 6);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  assert_strings_equal(token_string_value(token), "bark", "token value");

  token = get_token_from_list(list, 7);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_GROUP_END, "group end lex type");
  assert_equal(object_value(token), NULL, "lex value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call_and_deferred_arg() {
  spec_describe("Lexing method call with deferred argument");

  List *list = lex(" gerbil.talk(_,'bark') ");
  assert_equal(list_length(list), 8, "list length");

  Token *token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_DEFERRED_ARGUMENT, "deferred argument token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_COMMA, "comma token type");
  assert_equal(object_value(token), NULL, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_setting_local_variables() {
  spec_describe("Lexing setting of local variables");

  List *list = lex(" foo = 'bar' ");
  assert_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "id token type");
  assert_strings_equal(token_string_value(token), "foo", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_LOCAL_SETTER, "local setter token type");
  assert_equal(object_value(token), NULL, "token value");


  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  assert_strings_equal(token_string_value(token), "bar", "string token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}


char *test_ids_can_start_with_setter() {
  spec_describe("Lexing identifiers starts with identifier");

  List *list = lex(" foo =bar ");
  assert_equal(list_length(list), 2, "list_length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "id token type");
  assert_strings_equal(token_string_value(token), "foo", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "id token type starting with =");
  assert_strings_equal(token_string_value(token), "=bar", "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_atom() {
  spec_describe("Lexing atoms");

  List *list = lex(" :bar ");
  assert_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ATOM, "token type");
  assert_strings_equal(token_string_value(token), "bar", "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_attribute_assignment() {
  spec_describe("Lexing attribute assignment");

  List *list = lex(" foo: bar ");
  assert_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "id token type");
  assert_strings_equal(token_string_value(token), "foo", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ATTRIBUTE_SETTER, "attribute setter token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "id token identifier");
  assert_strings_equal(token_string_value(token), "bar", "token vaule");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_block_start_no_arguments() {
  spec_describe("Lexing block start with no arguments");

  List *list = lex(" list.each ->{ \n");

  assert_equal(list_length(list), 6, "list length");

  Token *token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "identifier token type");
  assert_strings_equal(token_string_value(token), "each", "token value");

  token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_DECLARATION, "block declaration token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_START, "block start token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_LINE_END, "line end token type");
  assert_equal(object_value(token), NULL, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_block_start_statement_and_end() {
  spec_describe("Lexing block start, statement and end, one line");

  List *list = lex(" list.each -> {puts 'gerbil'}");

  assert_equal(list_length(list), 8, "list length");

  Token *token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_DECLARATION, "block declaration token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_START, "block start token type");
  assert_equal(object_value(token), NULL, "block value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_ID, "identifier token type");
  assert_strings_equal(token_string_value(token), "puts", "token vauel");

  token = get_token_from_list(list, 6);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  assert_strings_equal(token_string_value(token), "gerbil", "token value");

  token = get_token_from_list(list, 7);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_END, "block end token type");
  assert_equal(object_value(token), NULL, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

char *test_block_with_arguments() {
  spec_describe("Lexing block with arguments and multiple statements");
  List *list = lex(" object.each ->(key, value){puts key; puts value}");

  assert_equal(list_length(list), 16, "list length");

  Token *token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_DECLARATION, "block declaration token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_GROUP_START, "group start token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 6);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_COMMA, "comma token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 8);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_GROUP_END, "group end token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 9);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_START, "block start token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 12);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_STATEMENT_END, "statement end token type");
  assert_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 15);
  check(token, "token was not attached to node");

  assert_equal(object_type(token), FX_TOKEN_BLOCK_END, "block end token type");
  assert_equal(object_value(token), NULL, "token value");

  token_list_free(list);

  return NULL;
error:
  return "failed";
}

void specs() {
  spec_setup("Lexing");

  run_spec(test_float);

  run_spec(test_float_with_padding);
  run_spec(test_two_floats_with_padding);
  run_spec(test_integer);

  run_spec(test_line_end);
  run_spec(test_line_end_with_float);

  run_spec(test_single_quoted_string_no_space);
  run_spec(test_double_quoted_string_no_space);
  run_spec(test_single_quoted_string_with_space);
  run_spec(test_double_quoted_string_with_space);
  run_spec(test_strings_with_line_break);

  run_spec(test_block_comment);
  run_spec(test_block_comment_affect_line);
  run_spec(test_line_comment);
  run_spec(test_line_comment_affect_line);

  run_spec(test_statement_end);

  run_spec(test_regex);
  run_spec(test_regex_with_space);

  run_spec(test_basic_identifier);
  run_spec(test_global_identifier);
  run_spec(test_number_starting_with_minus_sign);
  run_spec(test_exponential_numbers);
  run_spec(test_ids_starting_as_numbers);
  run_spec(test_ids_with_hyphens_and_underscores);

  run_spec(test_identifier_with_dot_method_call);
  run_spec(test_identifier_with_dot_method_call_and_argument);
  run_spec(test_identifier_with_dot_method_call_and_arguments);
  run_spec(test_identifier_with_dot_method_call_and_deferred_arg);

  run_spec(test_setting_local_variables);
  run_spec(test_ids_can_start_with_setter);

  run_spec(test_atom);
  run_spec(test_attribute_assignment);

  run_spec(test_block_start_no_arguments);
  run_spec(test_block_start_statement_and_end);
  run_spec(test_block_with_arguments);

  spec_teardown();
}

run_all_specs(specs);
