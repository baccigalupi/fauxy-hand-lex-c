#include <assert.h>

#include "../lib/dstring.h"
#include "../lib/basics.h"

#include "lib/mu_assert.h"

char *test_create_string_from_literal() {
  String *string = String_create("foo");

  mu_assert(strcmp(string_value(string), "foo") == 0, "String create did not set value");
  mu_assert(string_length(string) == 3,               "String create did not set count");

  string_destroy(string);

  return NULL;
}

char *test_create_string_from_alloced() {
  char *foo = calloc(4, sizeof(char));
  strcpy(foo, "foo");

  String *string = String_create(foo);

  pfree(foo);
  mu_assert(strcmp(string_value(string), "foo") == 0, "String create did not work with a freed allocation");
  mu_assert(string_length(string) == 3,               "String create did not set count with freed allocation");

  string_destroy(string);

  return NULL;
}

char *test_string_concat() {
  String *string = String_create("foo");
  string_concat(string, " bar");

  mu_assert(strcmp(string_value(string), "foo bar") == 0, "string_concat did not concat new string");
  mu_assert(string_length(string) == 7,                   "string_concat did not concat new string");

  string_destroy(string);

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create_string_from_literal);
  mu_run_test(test_create_string_from_alloced);
  mu_run_test(test_string_concat);

  return NULL;
}

RUN_TESTS(all_tests);
