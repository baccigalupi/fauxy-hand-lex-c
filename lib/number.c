#include <stdlib.h>
#include <string.h>

#include "error_handling.h"
#include "basics.h"

#include "number.h"


Number *Number_create(const char *str) {
  Number *number = calloc(1, sizeof(Number));
  check_mem(number);

  unsigned int is_float = 0;
  unsigned int i;
  for (i = 0; i < strlen(str); i++) {
    if (str[i] == '.') { is_float = 1; }
  }

  number_type(number) = is_float ? FloatType : IntegerType;
  number->value.as_float = A_TO_F(str);

  return number;
error:
  return NULL;
}

void number_convert(Number *number, int type) {
  if (number_type(number) == type) { return; }

  number_type(number) = (type == IntegerType) ? IntegerType : FloatType;
}

void number_destroy(Number *number) {
  pfree(number);
}
