#ifndef __bricks_hash
#define __bricks_hash

#include <stdlib.h>

#include "./string.h"
#include "./array.h"

typedef struct Hash {
  int length;
  int capacity;
  Array *values;
} Hash;

#define hash_values(H)    ((H)->values)
#define hash_length(H)    ((H)->length)
#define hash_capacity(H)     ((H)->capacity)
#define hash_free(H)      ((array_free(hash_values(H))), (pfree(hash)))

static uint32_t  Hash_algorithm(String *string);
Hash            *Hash_create(int length);

#endif
