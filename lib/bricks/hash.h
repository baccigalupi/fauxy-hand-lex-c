#ifndef __bricks_hash
#define __bricks_hash

#include <stdlib.h>

#include "./string.h"
#include "./array.h"

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
