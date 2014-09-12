#include "error_handling.h"
#include "bricks.h"

#include "./string.h"

String *String_create(CHAR *str) {
  String *string = calloc(1, sizeof(String));
  check_mem(string);

  int length = STRLEN(str);
  int limit = Expandable_limit(length);
  CHAR *value = calloc(limit + 1, sizeof(CHAR));
  check_mem(value);

  string_length(string) = length;
  string_limit(string)  = limit;
  string_value(string)  = value;

  STRCPY(value, str);

  return string;
error:
  if (string) { pfree(string); }
  return NULL;
}

void string_push(String *string, CHAR c) {
  if ( !(string_length(string) < string_limit(string)) ) {
    int limit = Expandable_limit(string_length(string));
    Boolean success = string_expand(string, limit);
    check(success, "Unable to allocate memory to expand string");
  }

  string_value(string)[string_length(string)] = c;
  string_length(string)++;
  string_value(string)[string_length(string)] = '\0'; // just in case
error:
  return;
}

Boolean string_expand(String *string, int limit) {
  CHAR *value = realloc(string_value(string), sizeof(CHAR)*limit);
  check_mem(value);
  string_value(string) = value;
  string_limit(string) = limit;

  return true;
error:
  return false;
}

void string_concat(String *string, CHAR *str) {
  int needed_length = STRLEN(str) + string_length(string);

  if ( !(needed_length < string_limit(string)) ) {
    int limit = Expandable_limit(needed_length);
    Boolean success = string_expand(string, limit);
    check(success, "Unable to allocate memory to expand string");
  }

  STRCAT(string_value(string), str);
  string_length(string) = needed_length;
  string_value(string)[string_length(string)] = '\0'; // just in case
error:
  return;
}
