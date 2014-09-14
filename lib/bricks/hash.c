#include "bricks.h"
#include "error_handling.h"

#include "hash.h"
#include "array.h"

/**
 * Simple Bob Jenkins's hash algorithm taken from the
 * wikipedia description.
 */
static uint32_t Hash_algorithm(String *string) {
  size_t length = string_length(string);
  char *key =     string_value(string);
  uint32_t hash = 0;
  uint32_t i = 0;

  for (hash = i = 0; i < length; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

Hash *Hash_create(int capacity) {
  Hash *hash = calloc(1, sizeof(Hash));
  check_mem(hash);

  hash_capacity(hash) = capacity;

  // TODO: array allocated with that number
  // setup array here to right capacity

  return hash;
error:
  return NULL;
}
