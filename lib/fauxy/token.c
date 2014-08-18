#include <stdlib.h>
#include <string.h>

#include "../error_handling.h"

#include "token.h"

FauxyToken *FauxyToken_create(FauxyTokenType type, FauxyBit *bit) {
  FauxyToken *token = calloc(1, sizeof(FauxyToken));
  check_mem(token);

  object_type(token) = type;
  object_value(token) = bit;
  
  return token;
error:
  return NULL;
}
