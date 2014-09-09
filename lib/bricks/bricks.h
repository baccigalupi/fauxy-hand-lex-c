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

#endif
