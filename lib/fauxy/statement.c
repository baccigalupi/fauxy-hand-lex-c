#include "statement.h"
#include "token.h"

#include "../bricks/error_handling.h"

Statement *Statement_create(StatementType type) {
  Statement *statement = calloc(1, sizeof(Statement));
  check_mem(statement);
  statement_type(statement) = type;
  statement_complete(statement) = false;

  if (statement_type_is_list(type)) {
    List *list = List_create();
    check_mem(list);
    statement_value(statement) = list;
  }

  return statement;
error:
  if (statement) { pfree(statement); }
  return NULL;
}

void statement_free(Statement *statement) {
  Node *node;

  if (statement_value_is_list(statement)) {
    Statement *child_statement;
    List *list = statement_value(statement);

    list_each(list, node) {
      child_statement = node_value(node);
      statement_free(child_statement);
    }

    list_free(list);
  } else {
    Token *token = statement_value(statement);
    token_free(token);
  }

  pfree(statement);
}
