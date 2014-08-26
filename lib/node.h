#ifndef __data_structures_node_h
#define __data_structures_node_h

typedef struct Node {
  void *value;
  struct Node *prev;
  struct Node *next;
  struct Node *parent;
} Node;

// NODE POLYMORPHISM -------------
// Allows same nodes for trees and lists
#define node_left(N)         ((N)->left)
#define node_right(N)        ((N)->right)
#define node_prev(N)         ((N)->left)
#define node_next(N)         ((N)->right)
#define node_parent(N)       ((N)->parent)
#define node_value(N)        ((N)->value)

#endif
