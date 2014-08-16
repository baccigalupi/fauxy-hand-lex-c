#ifndef __basics__
#define __basics__

#include <stdlib.h>

enum Boolean {
  false,
  true
};

// frees a pointer and sets it to null
#define pfree(P) if ((P) != NULL) { free(P); (P) = NULL; }

#endif
