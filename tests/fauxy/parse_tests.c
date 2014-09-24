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

#define get_statement_at(S, I)        (stack_statements(S)->values[I])

Token *get_statement_token_at(Statement *statement, int index) {
  Node *node;
  int i = 0;
  statement_each(statement, node) {
    if (i == index) {
      return node_value(node);
    }
    i++;
  }

  return NULL;
}

#define get_token_from_stack(I, J) get_token_from_statement(get_statement_at(I), J)

char *spec_number_statement() {
  spec_describe("number no line break");

  parse_test_setup(" 3.14 ");

  assert_ints_equal(stack_length(stack), 1, "statements length");

  Statement *statement = get_statement_at(stack, 0);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  Token *token = get_statement_token_at(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");

  parse_test_free();

  return NULL;
}

char *spec_number_statement_with_statement_end() {
  spec_describe("number semicolon no line break");

  parse_test_setup(" 3.14 ;");

  assert_ints_equal(stack_length(stack), 1, "stack length");

  Statement *statement = get_statement_at(stack, 0);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  Token *token = get_statement_token_at(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");

  parse_test_free();

  return NULL;
}

char *spec_2_numbers_separated_by_statement_end() {
  spec_describe("number semicolon number");

  parse_test_setup(" 3.14 ; 4.15");

  assert_ints_equal(stack_length(stack), 2, "stack length");

  Statement *statement = get_statement_at(stack, 0);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  Token *token = get_statement_token_at(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");

  statement = get_statement_at(stack, 1);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  token = get_statement_token_at(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");

  parse_test_free();

  return NULL;
}

char *spec_number_with_line_end() {
  spec_describe("number line break");

  parse_test_setup(" 3.14 \n");

  assert_ints_equal(stack_length(stack), 1, "stack length");

  Statement *statement = get_statement_at(stack, 0);
  assert_ints_equal(statement_length(statement), 1, "statement 1 length");
  Token *token = get_statement_token_at(statement, 0);
  assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");

  parse_test_free();

  return NULL;
}

void specs() {
  spec_setup("Parsing");

  run_spec(spec_number_statement);
  run_spec(spec_number_statement_with_statement_end);
  run_spec(spec_2_numbers_separated_by_statement_end);
  run_spec(spec_number_with_line_end);

  // GRAMMAR

  // Statement: single item
  // * number
  // * string
  // * regex
  // * atom
  // * id
  // * class name

  // Statement: grouped
  // * open-paren statement closed-paren

  // Statements
  // * statement ; statement
  // * statement line-end statement

  // Statement: Block
  // * block-declaration block-start block-end
  // * block-declaration block-start statements block-end
  // * block-declaration list block-start block-end
  // * block-declaration list block-start statements block-end

  // Statement: method call
  // * identifier dot identifier
  // * identifier identifier

  // Statement: method call with list
  // * identifier dot identifier list
  // * identifier identifier list

  // Statement: method call with block
  // * method-call block
  // * method-call-list block

  // Statement: list
  // * open-paren item close-paren
  // * open-paren item comma item close-paren
  // * open-paren item comma item close-paren


  spec_teardown();
}

run_all_specs(specs);
