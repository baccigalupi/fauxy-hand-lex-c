#ifndef __fauxy_parser
#define __fauxy_parser

#include "../bricks/array.h"
#include "./lex.h"

typedef Array Statements;

typedef enum {
  FX_EX_MEMORY_ERROR,
  FX_EX_UNEXPECTED_TOKEN
} ExceptionCode;

typedef struct Exception {
  ExceptionCode code;
} Exception;

typedef struct Stack {
  Statements *statements;
  Exception  *exception;
} Stack;


#define exception_code(E)       ((E)->code)
#define stack_statements(S)     ((S)->statements)
#define stack_length(S)         (array_length(stack_statements(S)))
#define stack_exception(S)      ((S)->exception)
#define stack_exception_code(S) (exception_code(stack_exception(S)))
#define stack_free(S)           (array_free(stack_statements(S)), pfree(stack_exception(S)), pfree(S))

#define STACK_START_SIZE        1024
#define Statements_create()     (Array_create(STACK_START_SIZE))

Stack *Stack_create();
void  parse(Tokens *tokens, Stack *stack);

#endif
