#include "../lib/spec.h"

#include "../../lib/fauxy/parser.h"
#include "../../lib/fauxy/lex.h"

char *test_parsing_primary_statment_number() {
  spec_describe("Primary Statement: number");

  Stack *stack =   Stack_create(); // pass in name, identifier??
  Tokens *tokens = lex_text(" 1.324 ");

  parse(tokens, stack);

  assert_ints_equal(stack_length(stack), 1, "stack length");
  assert_equal(stack_exception(stack), NULL, "no error");

  stack_free(stack);
  token_list_free(tokens);

  return NULL;
}

void specs() {
  spec_setup("Parsing");

  run_spec(test_parsing_primary_statment_number);

  spec_teardown();
}

run_all_specs(specs);
