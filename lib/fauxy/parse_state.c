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

  List *list = List_create();
  check_mem(list);
  state->statement_buffer = list;

  return state;
error:
  if (state) { pfree(state); }
  return NULL;
}

void parse_state_load_buffer(ParseState *state) {
  Token *token = NULL;

  while( lex_state_in_progress(state) && parse_state_seeking_statement_end(state) ) {
    token = lex_get_next_lexeme(state);
    if (token != NULL) {
      token = token_from_lexeme(token);
      parse_state_buffer_push(state, token);
    }
  }
}
