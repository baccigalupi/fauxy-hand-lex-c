#ifndef __helpers__
#define __helpers__

#include <stdlib.h>

typedef enum {
  false,
  true
} Boolean;

// MEMORY -----------
// frees a pointer and sets it to null
#define pfree(P)        if ((P) != NULL) { free(P); (P) = NULL; }
// convenience for clearing the pointer to null after destroyed, second argument is the destroy function
#define pdestroy(P, F)  pfree(P)


// VARIANT ACCESSORS -------------
#define object_type(O)       ((O)->type)
#define object_value(O)      ((O)->value)

// NODE POLYMORPHISM -------------
// Allows same nodes for trees and lists
#define node_left(N)         ((N)->left)
#define node_right(N)        ((N)->right)
#define node_prev(N)         ((N)->left)
#define node_next(N)         ((N)->right)
#define node_parent(N)       ((N)->parent)

#endif
