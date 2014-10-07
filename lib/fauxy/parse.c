#include "parse.h"

#include "../bricks/error_handling.h"

void parse_text(ParseState *state, Stack *stack) {
  check(state, "no syntax state in parser");
  check(stack, "stack not passed into parser");

  Statement *statements = stack_statements(stack);

  parse_state_load(state);
  while ( parse_state_current_token(state) ) {
    statments_add(statements, state);
    parse_state_load(state);
  }

error:
  if (stack) {
    stack_exception(stack) = Exception_create(FX_EX_MEMORY_ERROR);
  }
  return;
}

void statments_add(Statement *statements, ParseState *state) {
  Token *token = parse_state_current_token(state);
  Statement *unary = create_unary_statement(token);
  if (unary) {
    statement_push(statements, unary);
  }
}

Statement *create_unary_statement(Token *token) {
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
