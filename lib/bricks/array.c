#include "array.h"

Array *Array_create() {
  Array *array = calloc(1, sizeof(Array));
  check_mem(array);
  array_length(array) = 0;
  array_limit(array)  = DEFAULT_ARRAY_LIMIT;

  void **values = calloc(DEFAULT_ARRAY_LIMIT, sizeof(void *));
  check_mem(values);
  array->values = values;

  return array;
error:
  if (array) { pfree(array); }
  return NULL;
}

void array_push(Array *array, void *element) {
  if (array_limit(array) == array_length(array)) {
    int limit = Expandable_limit(array_limit(array));
    Boolean success = array_expand(array, limit);
    check(success, "unable to reallocate memory for expanding array");
  }

  array_values(array)[array_length(array)] = element;
  array_length(array) ++;
error:
  return;
}

Boolean array_expand(Array *array, int limit) {
  void **values = realloc(array_values(array), sizeof(void *) * limit);
  check_mem(values);
  array_values(array) = values;
  array_limit(array)  = limit;

  return true;
error:
  return false;
}

void array_each(Array *array, ArrayIterator f) {
  int i;
  void *element;
  for (i = 0; i < array_length(array); i++) {
    element = array_at_index(array, i);
    f(element);
  }
}

void *array_pop(Array *array) {
  void *value = NULL;

  if (array_length(array) > 0) {
    int index = array_length(array) - 1;
    value = array_at_index(array, index);
    array_at_index(array, index) = NULL;
    array_length(array)--;
  }

  return value;
}

void array_set(Array *array, int index, void *value) {
  if (array_limit(array) <= index) {
    int limit = Expandable_limit(index);
    Boolean success = array_expand(array, limit);
    check(success, "unable to reallocate memory for expanding array");
  }

  array_at_index(array, index) = value;
  array_length(array) = index + 1;
error:
  return;
}
