#include "parse_state.h"

#include "../bricks/error_handling.h"

#include "token.h"
#include "lex.h"

ParseState *ParseState_create(String *code) {
  ParseState *state = calloc(1, sizeof(ParseState));
  check_mem(state);
  state->code = code;
  state->line = 1;
  state->column = 1;

  return state;
error:
  return NULL;
}
