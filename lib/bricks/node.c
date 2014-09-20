#include <stdarg.h>

#include "node.h"
#include "error_handling.h"

Node *Node_create(void *value) {
  Node *node = calloc(1, sizeof(Node));
  check_mem(node);
  node_value(node) = value;
  return node;
error:
  return NULL;
}
