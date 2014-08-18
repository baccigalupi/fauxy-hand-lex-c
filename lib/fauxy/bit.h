#ifndef __fauxy_bit
#define __fauxy_bit

#include "../helpers.h"

#define bit_destroy(B)  pfree(B)

typedef enum {
  FauxyNumberType,
  FauxyStringType
} FauxyValueType;

typedef struct {
  FauxyValueType type;
  void *value;
} FauxyBit;

FauxyBit *FauxyBit_create(FauxyValueType type, void *value);

#endif
