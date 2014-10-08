#include "../lib/spec.h"

#include "../../lib/fauxy/parse.h"
#include "../../lib/fauxy/stack.h"
#include "../../lib/fauxy/lex.h"
#include "../../lib/bricks/string.h"

#define parse_test_setup(C) String *code = String_create(C);              \
                            ParseState *state = ParseState_create(code);  \
                            Stack  *stack = Stack_create();               \
                            parse_text(state, stack);

#define parse_test_free()   ((state && pfree(state)), (stack && stack_free(stack)), (code && string_free(code)))

void *get_statement_at_index(Statement *parent_statement, int index) {
  Statement *statement = NULL;

  if ( statement_value_is_list(parent_statement) ) {
    Node *node;
    int i = 0;

    list_each(statement_list_value(parent_statement), node) {
      if (i == index) {
        statement = node_value(node);
        break;
      }
      i++;
    }
  } else {
    statement = statement_value(parent_statement);
  }

  return statement;
}

void *get_stack_statement_at_index(Stack *stack, int index) {
  return get_statement_at_index(stack_statements(stack), index);
}

char *test_single_literal_statement() {
  spec_describe("single literal statement");

  parse_test_setup("1.324");

  Statement *statement;
  Token *token;

  assert_ints_equal(stack_length(stack), 1, "stack length");

  statement = get_stack_statement_at_index(stack, 0);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  assert_equal(statement_type(statement), FX_ST_LITERAL, "statement type");
  token = get_statement_at_index(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");

  parse_test_free();

  return NULL;
}

char *test_single_class_name() {
  spec_describe("single class name statement");

  parse_test_setup("MyClass");

  Statement *statement;
  Token *token;

  assert_ints_equal(stack_length(stack), 1, "stack length");

  statement = get_stack_statement_at_index(stack, 0);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  assert_equal(statement_type(statement), FX_ST_CLASS, "statement type");
  token = get_statement_at_index(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_CLASS_ID, "token type");

  parse_test_free();

  return NULL;
}

char *test_single_identifier() {
  spec_describe("single identifier statement");

  parse_test_setup("my_id");

  Statement *statement;
  Token *token;

  assert_ints_equal(stack_length(stack), 1, "stack length");

  statement = get_stack_statement_at_index(stack, 0);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  assert_equal(statement_type(statement), FX_ST_ID, "statement type");
  token = get_statement_at_index(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_ID, "token type");

  parse_test_free();

  return NULL;
}

char *test_literal_space_identifier() {
  spec_describe("literal space identifier method call");

  parse_test_setup("0 ++");

  Statement *statement;
  Token *token;

  assert_ints_equal(stack_length(stack), 1, "stack length");

  return NULL;
}

void specs() {
  spec_setup("Parsing");

  // identification of single statements
  run_spec(test_single_literal_statement);
  run_spec(test_single_class_name);
  run_spec(test_single_identifier);

  // method calls
  // run_spec(test_literal_space_identifier);

  // run_spec(spec_2_numbers_separated_by_statement_end);
  // run_spec(spec_2_numbers_separated_by_line_end);
  // run_spec(spec_literal_with_dot_method_call);
  // run_spec(spec_dot_method_call);

  spec_teardown();
}

run_all_specs(specs);

// char *spec_2_numbers_separated_by_statement_end() {
//   spec_describe("number semicolon number");
//
//   parse_test_setup(" 3.14 ; 4.15");
//
//   assert_ints_equal(stack_length(stack), 2, "stack length");
//
//   Statement *statement;
//   Token *token;
//
//   statement = get_stack_statement_at_index(stack, 0);
//   assert_ints_equal(statement_length(statement), 1, "statement 1 length");
//   assert_equal(statement_type(statement), FX_ST_LITERAL, "statement type");
//   token = get_statement_at_index(statement, 0);
//   assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");
//
//   statement = get_stack_statement_at_index(stack, 1);
//   assert_ints_equal(statement_length(statement), 1, "statement 2 length");
//   assert_equal(statement_type(statement), FX_ST_LITERAL, "statement type");
//   token = get_statement_at_index(statement, 0);
//   assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");
//
//   parse_test_free();
//
//   return NULL;
// }

// char *spec_2_numbers_separated_by_line_end() {
//   spec_describe("number line break");
//
//   parse_test_setup(" 3.14 \n 4.15 ");
//
//   assert_ints_equal(stack_length(stack), 2, "stack length");
//
//   Statement *statement;
//   Token *token;
//
//   statement = get_stack_statement_at_index(stack, 0);
//   assert_ints_equal(statement_length(statement), 1, "statement 1 length");
//   assert_ints_equal(statement_type(statement), FX_ST_LITERAL, "statement type");
//   token = get_statement_at_index(statement, 0);
//   assert_ints_equal(token_type(token), FX_TOKEN_NUMBER, "token type");
//
//   statement = get_stack_statement_at_index(stack, 1);
//   assert_ints_equal(statement_length(statement), 1, "statement 2 length");
//   assert_ints_equal(statement_type(statement), FX_ST_LITERAL, "statement type");
//   token = get_statement_at_index(statement, 0);
//   assert_ints_equal(token_type(token), FX_TOKEN_NUMBER, "token type");
//
//   parse_test_free();
//
//   return NULL;
// }
//
// char *spec_literal_with_dot_method_call() {
//   spec_describe("literal with dot method-call");
//
//   parse_test_setup(" '0'.to_i \n");
//
//   assert_ints_equal(stack_length(stack), 1, "stack length");
//
//   Statement *parent_statement;
//   Statement *sub_statement;
//   Token *token;
//
//   parent_statement = get_stack_statement_at_index(stack, 0);
//   assert_ints_equal(statement_length(parent_statement), 1, "statement 1 length");
//   assert_ints_equal(statement_type(parent_statement), FX_ST_METHOD_CALL, "statement type");
//
//   sub_statement = get_statement_at_index(parent_statement, 0);
//   assert_ints_equal(statement_length(sub_statement), 1, "method call receiver statement length");
//   assert_ints_equal(statement_type(sub_statement), FX_ST_LITERAL, "literal statement receiver");
//   token = get_statement_at_index(sub_statement, 0);
//   assert_ints_equal(token_type(token), FX_TOKEN_STRING, "token type string");
//
//   sub_statement = get_statement_at_index(parent_statement, 1);
//   assert_ints_equal(statement_length(sub_statement), 1, "method call method name statement length");
//   assert_ints_equal(statement_type(sub_statement), FX_ST_ID, "id statement method name");
//
//   return NULL;
// }
//
// char *spec_dot_method_call() {
//   spec_describe("dot method-call");
//
//   parse_test_setup(" foo.to_s \n");
//
//   assert_ints_equal(stack_length(stack), 1, "stack length");
//
//   Statement *parent_statement;
//   Statement *sub_statement;
//   // Token *token;
//
//   parent_statement = get_stack_statement_at_index(stack, 0);
//   assert_ints_equal(statement_length(parent_statement), 1, "statement 1 length");
//   assert_ints_equal(statement_type(parent_statement), FX_ST_METHOD_CALL, "statement type");
//
//   sub_statement = get_statement_at_index(parent_statement, 0);
//   assert_ints_equal(statement_length(sub_statement), 1, "method call receiver statement length");
//   assert_ints_equal(statement_type(sub_statement), FX_ST_ID, "id type statement receiver");
//
//   sub_statement = get_statement_at_index(parent_statement, 1);
//   assert_ints_equal(statement_length(sub_statement), 1, "method call method name statement length");
//   assert_ints_equal(statement_type(sub_statement), FX_ST_ID, "id statement method name");
//
//   parse_test_free();
//
//   return NULL;
// }
//
