#include "statements.h"

#include "../bricks/error_handling.h"
#include "token.h"

Statement *Statement_create() {
  Statement *statement = calloc(1, sizeof(Statement));
  check_mem(statement);

  List *list = List_create();
  check_mem(list);
  statement_values(statement) = list;

  return statement;
error:
  if (statement) { pfree(statement); }
  return NULL;
}

void statement_free(void *value) {
  Statement *statement = value;
  Node *node;
  Token *token;

  list_each(statement_values(statement), node) {
    token = node_value(node);
    token_free(token);
  }
  pfree(statement);
}
