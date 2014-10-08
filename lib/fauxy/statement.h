#ifndef __fauxy_statement
#define __fauxy_statement

#include "../bricks/list.h"
#include "../bricks/array.h"

#define STATEMENT_TYPE_LIMIT        64
#define NESTED_STATEMENT_TYPE_LIMIT 128

typedef enum {
  FX_ST_LITERAL = STATEMENT_TYPE_LIMIT,
  FX_ST_ID,
  FX_ST_CLASS,
  FX_ST_BLOCK   = NESTED_STATEMENT_TYPE_LIMIT,
  FX_ST_LIST,
  FX_ST_ATTR_ASSIGN,
  FX_ST_LOCAL_ASSIGN,
  FX_ST_METHOD_CALL,
  FX_ST_GROUP,
  FX_ST_STATEMENTS
} StatementType;

typedef struct Statement {
  StatementType type;
  void *value;
  Boolean complete;
} Statement;

#define STATEMENTS_INIT_SIZE    1024

#define statement_type(S)       ((S)->type)
#define statement_value(S)      ((S)->value)
#define statement_complete(S)   ((S)->complete)

#define statement_is_incomplete(S)  (!(S) || !statement_complete(S))

#define statement_type_is_list(T)   ((T) >= NESTED_STATEMENT_TYPE_LIMIT)
#define statement_value_is_list(S)  (statement_type_is_list(statement_type(S)))
#define statement_value_is_token(S) !(statement_value_is_list(S))

#define statement_list_value(S) ((List *)statement_value(S))
#define statement_length(S)     (statement_value_is_list(S) ? list_length(statement_list_value(S)) : 1)

#define statement_push(S, V)    (                                               \
                                  statement_value_is_list(S) ?                  \
                                  (list_push(statement_value(S), V)) :          \
                                  (statement_value(S)=V)                        \
                                )

#define statement_is_token(S)   (statement_type(S) < STATEMENT_TYPE_LIMIT)

Statement *Statement_create(StatementType type);
void       statement_free(Statement *statement);

#endif
