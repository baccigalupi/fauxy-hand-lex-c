#ifndef __fauxy_bit
#define __fauxy_bit


#include "../string.h"
#include "../number.h"

enum FauxyValueType {
  NumberType,
  StringType
}

typedef struct {
  FauxyValueType type;
  void *value;
} FauxyBit;

#endif
