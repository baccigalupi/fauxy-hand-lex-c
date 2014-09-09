#include <stdlib.h>
#include <string.h>

#include "bricks/error_handling.h"
#include "bricks/bricks.h"

#include "dstring.h"

String *String_create(const char *value) {
  String *string = calloc(1, sizeof(String));
  check_mem(string);

  string_length(string) = 0;
  check(string_expand(string, STRING_LENGTH_DEFAULT), "String_create unable to allocate string value");
  string_set(string, value); // no check because previous expansion did real check

  return string;
error:
  if (string) {
    pfree(string);
  }

  return NULL;
}

int string_set(String *string, const char *val) {
  int needed_length = strlen(val);

  // reallocate if too small
  if (string_limit(string) < needed_length) {
    check(string_expand(string, needed_length) > 0, "string_expand unable to allocate");
  }

  // reset string content
  memset(string_value(string), '\0', string_allocation(string));

  // copy data over and reset length
  strcpy(string_value(string), val);
  return string_length(string) = needed_length;
error:
  return -1;
}

int string_expand(String *string, int limit) {
  int original_limit = string_limit(string);
  char *original_value = string_value(string);

  string_limit(string) = limit;
  char *value = calloc(string_allocation(string), sizeof(char));
  check_mem(value);
  string_value(string) = value;

  pfree(original_value);

  return string_limit(string);
error:
  string_limit(string) = original_limit;

  return -1;
}

int string_concat(String *string, const char *addition) {
  int needed_length = string_length(string) + strlen(addition);

  if (needed_length > string_limit(string)) {
    check(string_expand(string, needed_length), "string_concat unable to allocate additional memory");
  }

  strcat(string_value(string), addition);
  return string_length(string) = needed_length;
error:
  return -1;
}

void string_destroy(String *string) {
  pfree(string_value(string));
  pfree(string);
}
