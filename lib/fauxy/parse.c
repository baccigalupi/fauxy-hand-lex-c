#include "parse.h"

#include "../bricks/error_handling.h"

void parse_text(ParseState *state, Stack *stack) {
  check(state, "no syntax state in parser");
  check(stack, "stack not passed into parser");

  Token *lexeme = NULL;
  Token *token = NULL;
  Statement *statement;

  stack_add_statement(stack, statement);

  while( lex_state_in_progress(state) ) {
    lexeme = lex_get_next_lexeme(state);
    if (lexeme) {
      token = token_from_lexeme(lexeme);
      if ( token_type(token) == FX_TOKEN_STATEMENT_END || token_type(token) == FX_TOKEN_LINE_END ) {
        token_free(token);
        stack_add_statement(stack, statement);
      } else {
        statement_push(statement, token);
      }
    }
  }

  if (statement_length(statement) == 0) {
    statement_free(stack_pop(stack));
  }
error:
  if (stack) {
    stack_exception(stack) = Exception_create(FX_EX_MEMORY_ERROR);
  }
  return;
}
