#include "statements.h"

#include "token.h"

void statement_free(void *value) {
  Statement *statement = value;
  Node *node;
  Token *token;
  list_each(statement, node) {
    token = node_value(node);
    token_free(token);
  }
  list_free(statement);
}
