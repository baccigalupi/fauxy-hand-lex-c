#ifndef __basics__
#define __basics__

#include <stdlib.h>

enum Boolean {
  false,
  true
};

// frees a pointer and sets it to null
#define pfree(P)        if ((P) != NULL) { free(P); (P) = NULL; }

// convenience for clearing the pointer to null after destroyed, second argument is the destroy function
#define pdestroy(P, F)  pfree(P)

#endif
