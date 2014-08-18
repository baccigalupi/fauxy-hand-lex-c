#include <stdlib.h>

#include "../error_handling.h"
#include "../basics.h"
#include "bit.h"

FauxyBit *FauxyBit_create(FauxyValueType type, void *value) {
  FauxyBit *bit = calloc(1, sizeof(FauxyBit));
  check_mem(bit);

  fauxy_bit_type(bit) = type;
  fauxy_bit_value(bit) = value;

  return bit;
error:
  return NULL;
}
