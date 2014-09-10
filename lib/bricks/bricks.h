#ifndef __helpers__
#define __helpers__

#include <stdlib.h>

#define CONSOLE_RED      "\e[31m"
#define CONSOLE_GREEN    "\e[38;5;40m"
#define CONSOLE_ORANGE   "\e[40;38;5;214m"
#define CONSOLE_RESET    "\e[0m"

typedef enum {
  false,
  true
} Boolean;

// MEMORY -----------
// frees a pointer and sets it to null
#define pfree(P)        ((P) != NULL) ? (free(P), ((P) = NULL)) : (NULL)
// convenience for clearing the pointer to null after destroyed, second argument is the destroy function
#define pdestroy(P, F)  pfree(P)


// VARIANT ACCESSORS -------------
#define object_type(O)       ((O)->type)
#define object_value(O)      ((O)->value)

#endif
