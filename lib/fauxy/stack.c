#include "stack.h"

#include "../bricks/error_handling.h"

Stack *Stack_create() {
  Stack *stack = calloc(1, sizeof(Stack));
  check_mem(stack);

  Statements *statements = Statements_create();
  check(statements, "memory error allocating stack statements");
  stack_statements(stack) = statements;
  stack_exception(stack) = NULL;

  return stack;
error:
  if (stack) { pfree(stack); }
  return NULL;
}
