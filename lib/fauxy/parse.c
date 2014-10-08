#include "parse.h"

#include "../bricks/error_handling.h"

ParseStatement *ParseStatement_create(Statement *statement) {
  ParseStatement *parse_statement = calloc(1, sizeof(ParseStatement));
  check_mem(parse_statement);

  parse_statement_statement(parse_statement) = statement;
  parse_statement_success(parse_statement) = false;

  return parse_statement;
error:
  return NULL;
}

void parse_text(ParseState *state, Stack *stack) {
  check(state, "no syntax state in parser");
  check(stack, "stack not passed into parser");

  Statement *statements = stack_statements(stack);
  Statement *statement = NULL;

  parse_state_load(state);
  while ( parse_state_current_token(state) ) {
    statement = get_next_statement(state, NULL);
    if (statement) {
      statement_push(statements, statement);
    }
  }

error:
  if (stack) {
    stack_exception(stack) = Exception_create(FX_EX_MEMORY_ERROR);
  }
  return;
}

Statement *get_next_statement(ParseState *state, Statement *parent_statement) {
  Token *token = parse_state_current_token(state);
  Statement *return_statement = NULL;

  // keep going until statement is complete // parse_state_current_token(state) &&
  while ( statement_is_incomplete(parent_statement) ) {
    ParseStatement *p_statement = ParseStatement_create(parent_statement);

    wrap_unary_statement(token, p_statement);
    parse_state_load(state);

    return_statement = parse_statement_statement(p_statement);
    pfree(p_statement);
    return return_statement;
  }

  return NULL;
}

void wrap_unary_statement(Token *token, ParseStatement *p_statement) {
  parse_statement_success(p_statement) = false;

  Statement *unary = create_unary_statement(token);
  if (unary) {
    parse_statement_statement(p_statement) = unary;
    parse_statement_success(p_statement) = true;
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
