#ifndef __fauxy_parse
#define __fauxy_parse

#include "stack.h"
#include "token.h"

// object for carrying switching current statement
// and identifying success of parse
typedef struct ParseStatement {
  Statement   *statement;
  Boolean     success;
  // complete?
} ParseStatement;

#define parse_statement_statement(P)    ((P)->statement)
#define parse_statement_success(P)      ((P)->success)

#define token_is_literal(T)     (                                               \
                                  (token_type(T) >= FX_TOKEN_NUMBER) &&         \
                                  (token_type(T) <= FX_TOKEN_ATOM)              \
                                )

#define token_is_id(T)          ( token_type(T) == FX_TOKEN_ID )

#define token_is_class(T)       ( token_type(T) == FX_TOKEN_CLASS_ID )

#define token_may_end_statement(T)  (                                                 \
                                      (token_type(T)) &&                              \
                                      (                                               \
                                        (token_type(T) == FX_TOKEN_STATEMENT_END) ||  \
                                        (token_type(T) == FX_TOKEN_LINE_END)          \
                                      )                                               \
                                    )

void         parse_text(ParseState *state, Stack *stack);
Statement   *get_next_statement(ParseState *state, Statement *parent_statement);
Statement   *create_unary_statement(Token *token);
void         wrap_unary_statement(Token *token, ParseStatement *p_statement);

ParseStatement *ParseStatement_create(Statement *statement);

#endif
