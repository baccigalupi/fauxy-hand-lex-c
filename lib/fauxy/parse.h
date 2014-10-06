#ifndef __fauxy_parse
#define __fauxy_parse

#include "stack.h"
#include "token.h"

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

void       parse_text(ParseState *state, Stack *stack);
Statement *parse_unary_statement(Token *token);

#endif
