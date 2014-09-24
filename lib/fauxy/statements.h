#ifndef __fauxy_statements
#define __fauxy_statements

#include "../bricks/list.h"
#include "../bricks/array.h"

typedef enum {
  FX_OBJECT_STATEMENT,        // number, string, regex, atom, identifier, class name, list, block
  FX_LIST_STATEMENT,          // object_statements, (paren grouped, comman separated objects)
  FX_BLOCK_STATEMENT,         // list (optional), statements
  FX_METHOD_STATEMENT,        // object, method
  FX_ATTR_ASSIGN_STATEMENT,   // identifier attribute_assignment_op statement
  FX_LOCAL_ASSIGN_STATEMENT,  // identifier local_assign_op statement
  FX_STATEMENTS               // list of any of the above statements
} StatementType;

typedef struct Statement {
  StatementType type;
  List *values;
} Statement;

typedef Array Statements;

#define STATEMENTS_INIT_SIZE    1024
#define statements_free(S)      (array_each(S, statement_free), array_free(S))
#define Statements_create()     (Array_create(STATEMENTS_INIT_SIZE))

#define statement_values(S)     ((S)->values)
#define statement_length(S)     (list_length(statement_values(S)))
#define statement_push(S, V)    (list_push(statement_values(S), V))
#define statement_each(S, N)    list_each(statement_values(S), N)

Statement *Statement_create();
void       statement_free(void *statement);

#endif
