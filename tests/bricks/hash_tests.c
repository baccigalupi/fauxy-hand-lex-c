#include "../../lib/bricks/hash.h"

#include "../lib/spec.h"

char *test_create_hash() {
  spec_describe("Create");
  //
  // Hash *hash = Hash_create(100);
  //
  // assert_ints_equal(hash_limit(hash), 100, "limit");
  // assert_ints_equal(hash_length(hash), 0,  "length");
  //
  // hash_free(hash);
  return NULL;
}

char *all_specs() {
  spec_setup("Brick Hash");

  // run_spec(test_create_hash);

  spec_teardown();

  return NULL;
}

run_all_specs(all_specs);
