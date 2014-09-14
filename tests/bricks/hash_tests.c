#include "../../lib/bricks/hash.h"

#include "../lib/spec.h"

char *test_create_hash() {
  spec_describe("Create");

  Hash *hash = Hash_create(100);

  assert_ints_equal(hash_capacity(hash), 100, "capacity");
  assert_ints_equal(hash_length(hash), 0,  "length");
  assert_ints_equal(array_capacity(hash_values(hash)), 100, "values capacity");
  assert_ints_equal(array_length(hash_values(hash)), 0, "values length");

  hash_free(hash);
  return NULL;
}

char *test_get_value_from_empty() {
  spec_describe("Getting values from empty hash");

  Hash *hash = Hash_create(10);
  String *key = String_create("key");

  assert_equal(hash_get(hash, key), NULL, "return NULL");

  string_free(key);
  hash_free(hash);
}

char *all_specs() {
  spec_setup("Brick Hash");

  run_spec(test_create_hash);
  run_spec(test_get_value_from_empty);

  spec_teardown();

  return NULL;
}

run_all_specs(all_specs);
