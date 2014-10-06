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

char *test_nested_statement() {
  spec_describe("nested statements, memory freeing");

  // block[
  //   list[identifier], statements[
  //     method_call[identifier, identifier]
  //   ]
  // ]]

  Statement *block = Statement_create(FX_ST_BLOCK);
  Statement *list = Statement_create(FX_ST_LIST);
  Statement *statements = Statement_create(FX_ST_STATEMENTS);
  Statement *method_call = Statement_create(FX_ST_METHOD_CALL);
  Token *identifier = Token_create(String_create("my_id"), 1, 1);
  token_type(identifier) = FX_TOKEN_ID;
  Token *literal = Token_create(String_create("0.15"), 1, 1);
  token_type(literal) = FX_TOKEN_NUMBER;
  Token *method_name = Token_create(String_create("to_i"), 1, 1);
  token_type(literal) = FX_TOKEN_ID;

  statement_push(method_call, literal);
  statement_push(method_call, method_name);
  statement_push(statements, method_call);
  statement_push(list, identifier);
  statement_push(block, list);
  statement_push(block, statements);

  statement_free(block);
  // use valgrind to see if it is all free

  return NULL;
}

void specs() {
  spec_setup("Statement");

  run_spec(test_token_statement);
  run_spec(test_list_statement);
  run_spec(test_nested_statement);

  spec_teardown();
}

run_all_specs(specs);
