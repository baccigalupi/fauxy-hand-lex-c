#include "../lib/spec.h"

#include "../../lib/fauxy/token.h"
#include "../../lib/fauxy/statement.h"

Statement *sub_statement_at_index(Statement *statement, int index) {
  Node *node;
  Statement *sub = NULL;
  int i = 0;

  list_each(statement_list_value(statement), node) {
    if (index == i) {
      sub = node_value(node);
      break;
    }
    i++;
  }

  return sub;
}

char *test_token_statement() {
  spec_describe("token statements");

  Token *token = Token_create(String_create("1.314"), 1, 1);
  token_type(token) = FX_TOKEN_NUMBER;

  Statement *statement = Statement_create(FX_ST_LITERAL);
  statement_push(statement, token);

  assert_ints_equal(statement_value_is_token(statement), 1, "value is token rather than list");
  assert_ints_equal(statement_length(statement),         1, "statement length");
  assert_equal(statement_value(statement),               token, "value");

  statement_free(statement);

  return NULL;
}

char *test_list_statement() {
  spec_describe("list statements");

  Token *literal = Token_create(String_create("1.314"), 1, 1);
  token_type(literal) = FX_TOKEN_NUMBER;
  Token *method = Token_create(String_create("to_s"), 1, 7);
  token_type(method) = FX_TOKEN_ID;

  Statement *statement = Statement_create(FX_ST_METHOD_CALL);
  statement_push(statement, literal);
  statement_push(statement, method);

  assert_ints_equal(statement_value_is_list(statement), 1, "value is list");
  assert_ints_equal(statement_length(statement),        2, "statement length");
  assert_equal(list_first(statement_list_value(statement)),  literal, "first value");
  assert_equal(list_last(statement_list_value(statement)),   method, "last value");

  statement_free(statement);

  return NULL;
}

void specs() {
  spec_setup("Statement");

  run_spec(test_token_statement);
  run_spec(test_list_statement);

  spec_teardown();
}

run_all_specs(specs);
