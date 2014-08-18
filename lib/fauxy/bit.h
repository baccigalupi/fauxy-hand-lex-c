#ifndef __fauxy_bit
#define __fauxy_bit

#include "../basics.h"

#define fauxy_bit_type(B)     ((B)->type)
#define fauxy_bit_value(B)    ((B)->value)
#define fauxy_bit_destroy(B)  pfree(B)

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
