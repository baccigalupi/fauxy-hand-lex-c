#include "parser.h"

#include "../bricks/error_handling.h"
#include "statement.h"

Stack *Stack_create() {
  Stack *stack = calloc(1, sizeof(Stack));
  check_mem(stack);

  Statements *statements = Statements_create();
  check(statements, "unable to allocate array for stack statements");
  stack_statements(stack) = statements;

  stack_exception(stack) = NULL;

  return stack;
error:
  if (stack) { pfree(stack); }
  return NULL;
}

void parse(Tokens *tokens, Stack *stack) {
  // ParserState *parser_state = ParserState_create();
  // check(parser_state, "unable to allocate parser state");
  check(tokens, "tokens in parse is null");
  check(stack,  "stack in parse is null");

  Node *node;
  list_each(tokens, node) {

  }

  // parser_state_free(parse_state);
  return;
error:
  // if (parser_state) { parser_state_free(parser_state); }
  if (stack) { stack_exception_code(stack) = FX_EX_MEMORY_ERROR; }
  return;
}
