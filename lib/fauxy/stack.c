#include "stack.h"

#include "../bricks/error_handling.h"

Stack *Stack_create() {
  Stack *stack = calloc(1, sizeof(Stack));
  check_mem(stack);

  Statement *statements = Statement_create(FX_ST_STATEMENTS);
  check(statements, "memory error allocating stack statements");
  stack_statements(stack) = statements;
  stack_exception(stack) = NULL;

  return stack;
error:
  if (stack) { pfree(stack); }
  return NULL;
}
