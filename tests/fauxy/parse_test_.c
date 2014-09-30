// #include "../lib/spec.h"
//
// #include "../../lib/fauxy/parse.h"
// #include "../../lib/fauxy/stack.h"
// #include "../../lib/fauxy/lex.h"
// #include "../../lib/bricks/string.h"
//
// #define parse_test_setup(C) String *code = String_create(C);              \
//                             ParseState *state = ParseState_create(code);  \
//                             Stack  *stack = Stack_create();               \
//                             parse_text(state, stack);
//
// #define parse_test_free()   ((state && pfree(state)), (stack && stack_free(stack)), (code && string_free(code)))
//
// #define get_statement_at(S, I)        (stack_statements(S)->values[I])
//
// Token *get_statement_token_at(Statement *statement, int index) {
//   Node *node;
//   int i = 0;
//   statement_each(statement, node) {
//     if (i == index) {
//       return node_value(node);
//     }
//     i++;
//   }
//
//   return NULL;
// }
//
// #define get_token_from_stack(I, J) get_token_from_statement(get_statement_at(I), J)
//
// char *spec_2_numbers_separated_by_statement_end() {
//   spec_describe("number semicolon number");
//
//   parse_test_setup(" 3.14 ; 4.15");
//   /*
//     stack->statements = statement(type=statements, values=list of statements)
//     each statement is type literal, with a values containing only the token
//   */
//
//   assert_ints_equal(stack_length(stack), 2, "stack length");
//
//   Statement *statement = get_statement_at(stack, 0);
//   assert_ints_equal(statement_length(statement), 1, "statement 1 length");
//   assert_equal(statement_type(statement), FX_OBJECT_STATEMENT, "statement type");
//   Token *token = get_statement_token_at(statement, 0);
//   assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");
//
//   statement = get_statement_at(stack, 1);
//   assert_ints_equal(statement_length(statement), 1, "statement 1 length");
//   assert_equal(statement_type(statement), FX_OBJECT_STATEMENT, "statement type");
//   token = get_statement_token_at(statement, 0);
//   assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");
//
//   parse_test_free();
//
//   return NULL;
// }
//
// char *spec_2_numbers_separated_by_line_end() {
//   spec_describe("number line break");
//
//   parse_test_setup(" 3.14 \n 4.15 ");
//
//   assert_ints_equal(stack_length(stack), 1, "stack length");
//
//   Statement *statement = get_statement_at(stack, 0);
//   assert_ints_equal(statement_length(statement), 1, "statement 1 length");
//   assert_equal(statement_type(statement), FX_OBJECT_STATEMENT, "statement type");
//   Token *token = get_statement_token_at(statement, 0);
//   assert_equal(token_type(token), FX_TOKEN_NUMBER, "token type");
//
//   parse_test_free();
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
//   Statement *statement = get_statement_at(stack, 0);
//   assert_ints_equal(statement_length(statement), 2, "statement 1 length");
//   assert_equal(statement_type(statement), FX_METHOD_STATEMENT, "statement type");
//   Token *token = get_statement_token_at(statement, 0);
//   assert_equal(token_type(token), FX_TOKEN_ID, "token type");
//   token = get_statement_token_at(statement, 1);
//   assert_equal(token_type(token), FX_TOKEN_ID, "token type");
//
//   return NULL;
// }
//
// void specs() {
//   spec_setup("Parsing");
//
//   run_spec(spec_2_numbers_separated_by_statement_end);
//   run_spec(spec_2_numbers_separated_by_line_end);
//   run_spec(spec_dot_method_call);
//
//   spec_teardown();
// }
//
// run_all_specs(specs);
