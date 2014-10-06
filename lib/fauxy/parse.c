#include "parse.h"

#include "../bricks/error_handling.h"

void parse_text(ParseState *state, Stack *stack) {
  check(state, "no syntax state in parser");
  check(stack, "stack not passed into parser");

  Statement *statements = stack_statements(stack);
  Token *token = NULL;
  Statement *statement = NULL;
  Statement *sub_statement = NULL;

  parse_state_load(state);

  while( parse_state_current_token(state) != NULL ) {
    token = parse_state_current_token(state);

    statement == NULL ?
      (statement = parse_unary_statement(token)) :
      (sub_statement = parse_unary_statement(token));

    if (statement && token_may_end_statement(token)) {
      statement_push(statements, statement);
      token_free(token);
      statement = NULL;
    }

    parse_state_load(state);
  }

  if (statement && statement_length(statement) > 0) {
    statement_push(statements, statement);
  }

error:
  if (stack) {
    stack_exception(stack) = Exception_create(FX_EX_MEMORY_ERROR);
  }
  return;
}

Statement *parse_unary_statement(Token *token) {
  Statement *statement;
  StatementType type = 0;

  if ( token_is_literal(token) ) {
    type = FX_ST_LITERAL;
  } else if ( token_is_id(token) ) {
    type = FX_ST_ID;
  } else if ( token_is_class(token) ) {
    type = FX_ST_CLASS;
  }

  if (!type) { return NULL; }

  statement = Statement_create(type);
  check(statement, "Unable to create statement");
  statement_value(statement) = token;

  return statement;
error:
  return NULL;
}
