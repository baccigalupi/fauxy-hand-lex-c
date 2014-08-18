#include <stdlib.h>

#include "../error_handling.h"
#include "../helpers.h"
#include "bit.h"

FauxyBit *FauxyBit_create(FauxyValueType type, void *value) {
  FauxyBit *bit = calloc(1, sizeof(FauxyBit));
  check_mem(bit);

  object_type(bit) = type;
  object_value(bit) = value;

  return bit;
error:
  return NULL;
}
