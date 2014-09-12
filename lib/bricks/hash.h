#ifndef __bricks_array
#define __bricks_array

#include <stdlib.h>

#include "bricks.h"
#include "error_handling.h"

#include "./string.h"
#include "array.h"


typedef struct Hash {
  int length;
  int limit;
  // Array *values;
} Hash;

#define hash_values(H)    ((H)->values)
#define hash_length(H)    ((H)->length)
#define hash_limit(H)     ((H)->limit)

static uint32_t  Hash_algorithm(String *string);
Hash            *Hash_create(int length);

// static void inline hash_free(Hash *hash) {
//   array_free(hash_values(hash));
//   pfree(hash);
// }

#endif
