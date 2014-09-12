#ifndef __bricks_array
#define __bricks_array

#include <stdlib.h>

#include "bricks.h"
#include "error_handling.h"

#define DEFAULT_ARRAY_LIMIT 32

typedef struct Array {
  int length;
  int limit;

  void **values;
} Array;

typedef void (ArrayIterator)(void *);

#define array_length(A)      ((A)->length)
#define array_limit(A)       ((A)->limit)
#define array_values(A)      ((A)->values)
#define array_at_index(A, I) (array_values(A)[I])
#define array_get(A, I)      array_at_index(A, I)
#define array_free(A)        (pfree(A))
#define array_r_free(A)      ((array_each(A, free)), array_free(A))

Array     *Array_create();
void      array_push(Array *array, void *element);
Boolean   array_expand(Array *array, int seed);
void      array_each(Array *array, ArrayIterator f);
void      *array_pop(Array *array);
void      array_set(Array *array, int index, void *value);

#endif
