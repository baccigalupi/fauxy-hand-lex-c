#ifndef __fauxy_bit
#define __fauxy_bit


// #include "../string.h"
// #include "../number.h"

#define fauxy_bit_type(B)   ((B)->type)
#define fauxy_bit_value(B)  ((B)->value)

typedef enum {
  FauxyNumberType,
  FauxyStringType
} FauxyValueType;

typedef struct {
  FauxyValueType type;
  void *value;
} FauxyBit;

FauxyBit *FauxyBit_create(FauxyValueType type, void *value);
void fauxy_bit_destroy(FauxyBit *bit);

#endif
