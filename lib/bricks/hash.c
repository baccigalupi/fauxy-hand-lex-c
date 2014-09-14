#include "bricks.h"
#include "error_handling.h"

#include "hash.h"
#include "array.h"

Hash *Hash_create(int capacity) {
  Hash *hash = calloc(1, sizeof(Hash));
  check_mem(hash);

  hash_capacity(hash) = capacity;
  hash_values(hash) = Array_create(capacity);
  check(hash_values(hash), "unable to create hash values array");

  return hash;
error:
  if (hash) { pfree(hash); }
  return NULL;
}

void *hash_get(Hash *hash, String *key) {

  return NULL;
}
