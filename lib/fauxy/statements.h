#ifndef __fauxy_statements
#define __fauxy_statements

#include "../bricks/list.h"
#include "../bricks/array.h"

typedef List  Statement;
typedef Array Statements;

#define STATEMENTS_INIT_SIZE    1024
#define Statements_create()     (Array_create(STATEMENTS_INIT_SIZE))
#define statements_free(S)      (array_each(S, statement_free), array_free(S))

#define statement_length(S)     list_length(S)
#define Statement_create()      List_create()
#define statement_push(S, V)    list_push(S, V)

void statement_free(void *statement);

#endif
