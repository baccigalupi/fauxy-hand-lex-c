#ifndef __fauxy_stack
#define __fauxy_stack

#include "../bricks/array.h"
#include "./statement.h"
#include "./exception.h"
#include "./lex.h"

typedef struct Stack {
  Statement *statements;
  Exception  *exception;
} Stack;

#define stack_statements(S)         ((S)->statements)
#define stack_exception(S)          ((S)->exception)
#define stack_length(S)             (statement_length(stack_statements(S)))
#define stack_exception_code(S)     (exception_code(stack_exception(S)))
#define stack_push(S, V)            (array_push(stack_statements(S), V))
#define stack_pop(S)                (array_pop(stack_statements(S)))

#define stack_free(S)           (                                               \
                                  statement_free(stack_statements(S)),          \
                                  exception_free(stack_exception(S)),           \
                                  pfree(S)                                      \
                                )

Stack *Stack_create();

#endif
