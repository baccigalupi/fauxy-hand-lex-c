#include <assert.h>

#include "../../lib/fauxy/lex.h"
#include "../../lib/fauxy/token.h"
#include "../../lib/list.h"
#include "../../lib/node.h"
#include "../../lib/number.h"
#include "../../lib/helpers.h"

#include "../lib/spec.h"

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

  spec_describe("Lexing a float with no padding");

  spec_equal(object_type(token), FX_TOKEN_NUMBER,     "token type");
  spec_equal(token_number_value(token), (FLOAT)1.324, "token value");
  spec_equal(token_line(token), 1,                    "token line");
  spec_equal(token_column(token), 1,                  "token column");

  list_clear_and_destroy(list);

  return NULL;
error:
  return "failed";
}

char *test_float_with_padding() {
  spec_describe("Lexing a float with padding");

  List *list = lex("    1.324   ");
  spec_equal(list_length(list), 1, "lex created wrong number of tokens");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  spec_equal(token_number_value(token), (FLOAT)1.324, "token value");
  spec_equal(token_line(token), 1, "token line");
  spec_equal(token_column(token), 5, "token column");

  list_clear_and_destroy(list);

  return NULL;
error:
  return "failed";
}

char *test_two_floats_with_padding() {
  spec_describe("Lexing two floats with padding");
  List *list = lex("    1.324   4.0  ");
  spec_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  spec_equal(token_number_value(token), (FLOAT)1.324, "token value");
  spec_equal(token_line(token), 1, "token line");
  spec_equal(token_column(token), 5, "token column");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER, "second token type");
  spec_equal(token_number_value(token), (FLOAT)4.0, "second token value");
  spec_equal(token_line(token), 1, "second token line");
  spec_equal(token_column(token), 13, "second token column");

  return NULL;
error:
  return "failed";
}

char *test_line_end() {
  spec_describe("Lexing line end with padding");
  List *list = lex(" \n ");
  spec_equal(list_length(list), 1, "list_length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_LINE_END, "token type");
  spec_equal(object_value(token), NULL, "token value");
  spec_equal(token_line(token), 1, "token line");
  spec_equal(token_column(token), 2, "token column");

  return NULL;
error:
  return "failed";
}

char *test_line_end_with_float() {
  spec_describe("Lexing line end with float and padding");
  List *list = lex(" \n   3.14");
  spec_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_LINE_END, "token type for line end");
  spec_equal(object_value(token), NULL, "token value line end");
  spec_equal(token_line(token), 1, "token line for line end");
  spec_equal(token_column(token), 2, "token column for line end");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER, "token type for number");
  spec_equal(token_number_value(token), (FLOAT)3.14, "token value for number");
  spec_equal(token_line(token), 2, "token line for number");
  spec_equal(token_column(token), 4, "token column for number");

  return NULL;
error:
  return "failed";
}

char *test_integer() {
  spec_describe("Lexing interger");
  List *list = lex("314");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  spec_equal(token_number_value(token), (INT)314, "token value");

  return NULL;
error:
  return "failed";
}

char *test_single_quoted_string_no_space() {
  spec_describe("Lexing single simple quoted strings");

  List *list = lex("'hello'");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "token type");
  spec_strings_equal(token_string_value(token), "hello",  "token value");
  spec_equal(token_line(token), 1, "token line");
  spec_equal(token_column(token), 1, "token column");

  return NULL;
error:
  return "failed";
}

char *test_double_quoted_string_no_space() {
  spec_describe("Lexing double quoted basic string");

  List *list = lex("\"hello\"");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "token type");
  spec_strings_equal(token_string_value(token), "hello",  "token value");
  spec_equal(token_line(token), 1, "token line set incorrectly");
  spec_equal(token_column(token), 1, "token column set incorrectly");

  return NULL;
error:
  return "failed";
}

char *test_single_quoted_string_with_space() {
  spec_describe("Lexing single quoted string containing space, padded");

  List *list = lex(" 'hello world' ");

  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "token type");
  spec_strings_equal(token_string_value(token), "hello world",  "token value");
  spec_equal(token_line(token), 1, "token line");
  spec_equal(token_column(token), 2, "token column");

  return NULL;
error:
  return "failed";
}

char *test_double_quoted_string_with_space() {
  spec_describe("Lexing double quoted string containing space, padded");

  List *list = lex(" \"hello world\" ");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "token type");
  spec_strings_equal(token_string_value(token), "hello world",  "token value");
  spec_equal(token_line(token), 1, "token line");
  spec_equal(token_column(token), 2, "token column");

  return NULL;
error:
  return "failed";
}

char *test_strings_with_line_break() {
  spec_describe("Multiple string types, one containing line break");

  List *list = lex(" \"hello\nworld\" 'wha?'");
  spec_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "token type");
  spec_strings_equal(token_string_value(token), "hello\nworld",  "token value");
  spec_equal(token_line(token), 1, "token line set incorrectly");
  spec_equal(token_column(token), 2, "token column set incorrectly");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "token type");
  spec_strings_equal(token_string_value(token), "wha?",  "token value");
  spec_equal(token_line(token), 2, "token line");
  spec_equal(token_column(token), 8, "token column");

  return NULL;
error:
  return "failed";
}

char *test_block_comment() {
  spec_describe("Lexing block comments");
  List *list = lex(" /* hello\ncomment */ ");

  spec_equal(list_length(list), 0, "list length");

  return NULL;
}

char *test_block_comment_affect_line() {
  spec_describe("Lexing block comment followed by float");

  List *list = lex("/* hello\ncomment */\n 3.14");
  spec_equal(list_length(list), 2, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_LINE_END, "line end token type");
  spec_equal(object_value(token), NULL, "token value");
  spec_equal(token_line(token), 2, "token line");
  spec_equal(token_column(token), 11, "token column");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER,    "number token type");
  spec_equal(token_number_value(token), (FLOAT)3.14, "token value");
  spec_equal(token_line(token), 3,                   "token line");
  spec_equal(token_column(token), 2,                 "token column");

  return NULL;
error:
  return "failed";
}

char *test_line_comment() {
  spec_describe("Lexing line comment preceded by float");

  List *list = lex("3.14 // hello comment");

  spec_equal(list_length(list), 1, "list length");

  return NULL;
}

char *test_line_comment_affect_line() {
  spec_describe("Lexing int line comment and float on next line");

  List *list = lex("1 // hello comment\n 3.14");
  spec_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER,    "int token type");
  spec_equal(token_number_value(token), (INT)1,      "token value");
  spec_equal(token_line(token), 1,                   "token line");
  spec_equal(token_column(token), 1,                 "token column");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_LINE_END, "line end token type");
  spec_equal(object_value(token), NULL,             "token value");
  spec_equal(token_line(token), 1,                  "token line");
  spec_equal(token_column(token), 19,               "token column");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER,    "float number type");
  spec_equal(token_number_value(token), (FLOAT)3.14, "token value");
  spec_equal(token_line(token), 2,                   "token line");
  spec_equal(token_column(token), 2,                 "token column");

  return NULL;
error:
  return "failed";
}

char *test_statement_end() {
  spec_describe("Lexing multiple statements seperated by semicolons");

  List *list = lex("'hello'; 1234; 3.14");
  spec_equal(list_length(list), 5, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING,           "single string token type");
  spec_strings_equal(token_string_value(token), "hello", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STATEMENT_END,    "semicolon token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER,    "int token type");
  spec_equal(token_number_value(token), (INT)1234,   "int token value");

  token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STATEMENT_END,    "semicolon token type");
  spec_equal(object_value(token), NULL, "semicolon token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER,    "float number type");
  spec_equal(token_number_value(token), (FLOAT)3.14,   "float number value");

  return NULL;
error:
  return "failed";
}

char *test_regex() {
  spec_describe("Lexing regex with trailing modifier, padded");

  List *list = lex(" /[a-z]/i ");

  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_REGEX, "token type");
  spec_equal(strcmp(token_string_value(token),"/[a-z]/i"), 0, "token value");

  return NULL;
error:
  return "failed";
}

char *test_regex_with_space() {
  spec_describe("Lexing regex containing white space and with trailing modifier, padded");

  List *list = lex(" /[a-z] [0-9]/i ");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_REGEX, "token type");
  spec_equal(strcmp(token_string_value(token),"/[a-z] [0-9]/i"), 0, "token value");

  return NULL;
error:
  return "failed";
}

char *test_basic_identifier() {
  spec_describe("Lexing padded identifier");

  List *list = lex(" gerbil ");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "token type");
  spec_strings_equal(token_string_value(token),"gerbil", "token value");

  return NULL;
error:
  return "failed";
}

char *test_global_identifier() {
  spec_describe("Lexing global identifier with padding");

  List *list = lex(" Gerbil ");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_GLOBAL_ID, "token type");
  spec_strings_equal(token_string_value(token),"Gerbil", "lex did not build right value for type id");

  return NULL;
error:
  return "failed";
}

char *test_number_starting_with_minus_sign() {
  spec_describe("Lexing number starting with minus sign");

  List *list = lex(" -1.23");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  spec_equal(token_number_value(token), (FLOAT)(-1.23), "token value");

  return NULL;
error:
  return "failed";
}

char *test_exponential_numbers() {
  spec_describe("Lexing exponential numbers");

  List *list = lex(" 1E-8");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_NUMBER, "token type");
  spec_equal(token_number_value(token), (FLOAT)(1E-8), "token value");

  return NULL;
error:
  return "failed";
}

char *test_ids_starting_as_numbers() {
  spec_describe("Lexing identifiers starting with numbers");

  List *list = lex(" 123foo");

  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID,  "token type");
  spec_strings_equal(token_string_value(token), "123foo", "token value");

  return NULL;
error:
  return "failed";
}

char *test_ids_with_hyphens_and_underscores() {
  spec_describe("Lexing ids with numbers hyphens and underscores");

  List *list = lex(" 123-foo_bar- ");

  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID,  "token type");
  spec_strings_equal(token_string_value(token), "123-foo_bar-", "token value");

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call() {
  spec_describe("Lexing dot attribute selection");

  List *list = lex(" file.open ");
  spec_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "id token type");
  spec_strings_equal(token_string_value(token), "file", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ATTRIBUTE_SELECTOR, "dot attribute selector token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "id token type");
  spec_strings_equal(token_string_value(token), "open", "token value");

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call_and_argument() {
  spec_describe("Lexing dot method calls with argument");

  List *list = lex(" file.open('w') ");
  spec_equal(list_length(list), 6, "list length");

  Token *token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_GROUP_START, "group start token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  spec_strings_equal(token_string_value(token), "w", "token value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_GROUP_END, "group end token type");
  spec_equal(object_value(token), NULL, "token value");

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call_and_arguments() {
  spec_describe("Lexing identifier with dot method call and arguments");

  List *list = lex(" gerbil.talk('squeak','bark') ");
  spec_equal(list_length(list), 8, "list length");

  Token *token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_COMMA, "comma token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 6);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  spec_strings_equal(token_string_value(token), "bark", "token value");

  token = get_token_from_list(list, 7);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_GROUP_END, "group end lex type");
  spec_equal(object_value(token), NULL, "lex value");

  return NULL;
error:
  return "failed";
}

char *test_identifier_with_dot_method_call_and_deferred_arg() {
  spec_describe("Lexing method call with deferred argument");

  List *list = lex(" gerbil.talk(_,'bark') ");
  spec_equal(list_length(list), 8, "list length");

  Token *token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_DEFERRED_ARGUMENT, "deferred argument token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_COMMA, "comma token type");
  spec_equal(object_value(token), NULL, "token value");

  return NULL;
error:
  return "failed";
}

char *test_setting_local_variables() {
  spec_describe("Lexing setting of local variables");

  List *list = lex(" foo = 'bar' ");
  spec_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "id token type");
  spec_strings_equal(token_string_value(token), "foo", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_LOCAL_SETTER, "local setter token type");
  spec_equal(object_value(token), NULL, "token value");


  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  spec_strings_equal(token_string_value(token), "bar", "string token value");

  return NULL;
error:
  return "failed";
}


char *test_ids_can_start_with_setter() {
  spec_describe("Lexing identifiers starts with identifier");

  List *list = lex(" foo =bar ");
  spec_equal(list_length(list), 2, "list_length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "id token type");
  spec_strings_equal(token_string_value(token), "foo", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "id token type starting with =");
  spec_strings_equal(token_string_value(token), "=bar", "token value");

  return NULL;
error:
  return "failed";
}

char *test_atom() {
  spec_describe("Lexing atoms");

  List *list = lex(" :bar ");
  spec_equal(list_length(list), 1, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ATOM, "token type");
  spec_strings_equal(token_string_value(token), "bar", "token value");

  return NULL;
error:
  return "failed";
}

char *test_attribute_assignment() {
  spec_describe("Lexing attribute assignment");

  List *list = lex(" foo: bar ");
  spec_equal(list_length(list), 3, "list length");

  Token *token = get_token_from_list(list, 0);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "id token type");
  spec_strings_equal(token_string_value(token), "foo", "token value");

  token = get_token_from_list(list, 1);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ATTRIBUTE_SETTER, "attribute setter token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "id token identifier");
  spec_strings_equal(token_string_value(token), "bar", "token vaule");

  return NULL;
error:
  return "failed";
}

char *test_block_start_no_arguments() {
  spec_describe("Lexing block start with no arguments");

  List *list = lex(" list.each ->{ \n");

  spec_equal(list_length(list), 6, "list length");

  Token *token = get_token_from_list(list, 2);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "identifier token type");
  spec_strings_equal(token_string_value(token), "each", "token value");

  token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_DECLARATION, "block declaration token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_START, "block start token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_LINE_END, "line end token type");
  spec_equal(object_value(token), NULL, "token value");

  return NULL;
error:
  return "failed";
}

char *test_block_start_statement_and_end() {
  spec_describe("Lexing block start, statement and end, one line");

  List *list = lex(" list.each -> {puts 'gerbil'}");

  spec_equal(list_length(list), 8, "list length");

  Token *token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_DECLARATION, "block declaration token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_START, "block start token type");
  spec_equal(object_value(token), NULL, "block value");

  token = get_token_from_list(list, 5);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_ID, "identifier token type");
  spec_strings_equal(token_string_value(token), "puts", "token vauel");

  token = get_token_from_list(list, 6);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STRING, "string token type");
  spec_strings_equal(token_string_value(token), "gerbil", "token value");

  token = get_token_from_list(list, 7);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_END, "block end token type");
  spec_equal(object_value(token), NULL, "token value");

  return NULL;
error:
  return "failed";
}

char *test_block_with_arguments() {
  spec_describe("Lexing block with arguments and multiple statements");
  List *list = lex(" object.each ->(key, value){puts key; puts value}");

  spec_equal(list_length(list), 16, "list length");

  Token *token = get_token_from_list(list, 3);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_DECLARATION, "block declaration token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 4);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_GROUP_START, "group start token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 6);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_COMMA, "comma token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 8);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_GROUP_END, "group end token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 9);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_START, "block start token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 12);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_STATEMENT_END, "statement end token type");
  spec_equal(object_value(token), NULL, "token value");

  token = get_token_from_list(list, 15);
  check(token, "token was not attached to node");

  spec_equal(object_type(token), FX_TOKEN_BLOCK_END, "block end token type");
  spec_equal(object_value(token), NULL, "token value");

  return NULL;
error:
  return "failed";
}

char *all_tests() {
  mu_suite_start();
  setup("Lexing");

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

  mu_run_test(test_basic_identifier);
  mu_run_test(test_global_identifier);
  mu_run_test(test_number_starting_with_minus_sign);
  mu_run_test(test_exponential_numbers);
  mu_run_test(test_ids_starting_as_numbers);
  mu_run_test(test_ids_with_hyphens_and_underscores);

  mu_run_test(test_identifier_with_dot_method_call);
  mu_run_test(test_identifier_with_dot_method_call_and_argument);
  mu_run_test(test_identifier_with_dot_method_call_and_arguments);
  mu_run_test(test_identifier_with_dot_method_call_and_deferred_arg);

  mu_run_test(test_setting_local_variables);
  mu_run_test(test_ids_can_start_with_setter);

  mu_run_test(test_atom);
  mu_run_test(test_attribute_assignment);

  mu_run_test(test_block_start_no_arguments);
  mu_run_test(test_block_start_statement_and_end);
  mu_run_test(test_block_with_arguments);

  teardown();

  return NULL;
}

RUN_TESTS(all_tests);
