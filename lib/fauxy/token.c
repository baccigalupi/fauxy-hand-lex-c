#include <stdlib.h>
#include <string.h>

#include "../error_handling.h"

#include "token.h"

Token *Token_create(TokenType type) {
  Token *token = calloc(1, sizeof(Token));
  check_mem(token);

  object_type(token) = type;

  return token;
error:
  return NULL;
}
