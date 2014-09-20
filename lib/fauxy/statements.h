#ifndef __fauxy_statements
#define __fauxy_statements

#include "../bricks/list.h"
#include "../bricks/array.h"

typedef List  Statement;
typedef Array Statements;

#define STATEMENTS_INIT_SIZE    1024
#define Statements_create()     (Array_create(STATEMENTS_INIT_SIZE))
#define statements_free(S)      (array_free(S))

#endif
