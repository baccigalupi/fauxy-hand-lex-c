#include "parse.h"

#include "../bricks/error_handling.h"

void parse_text(char *text, Stack *stack) {
  check(text, "no code pased in for parsing");
  check(stack, "stack not passed into parser");

  String   *code =              String_create(text);
  SyntaxGeneratorState *state = SyntaxGeneratorState_create(code);

  Token *lexeme = NULL;

  while( lex_state_in_progress(state) ) {
    lexeme = lex_get_next_lexeme(state);
    if (lexeme) {
      // do something with this: token_from_lexeme(lexeme);
    }
  }

  string_free(code);
  pfree(state);
error:
  if (stack) {
    stack_exception_code(stack) = FX_EX_MEMORY_ERROR;
  }
  return;
}
