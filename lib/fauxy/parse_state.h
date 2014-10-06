#ifndef __fauxy_parse_state
#define __fauxy_parse_state

#include "../bricks/list.h"
#include "token.h"

typedef enum {
  FX_CLOSING_NULL,              // NULL state for iffiness
  FX_CLOSING_SINGLE_QUOTE,
  FX_CLOSING_DOUBLE_QUOTE,
  FX_CLOSING_LINE_COMMENT,
  FX_CLOSING_BLOCK_COMMENT,
  FX_OPENING_REGEX,
  FX_CLOSING_REGEX
} Bookend;

typedef struct ParseState {
  String *code;
  int current;
  int line;
  int column;
  Bookend  lexical_bookend;
  Token    *current_token;
  Token    *next_token;
} ParseState;

#define parse_state_current(P)          ((P)->current)
#define parse_state_line(P)             ((P)->line)
#define parse_state_column(P)           ((P)->column)
#define parse_state_length(P)           (((P)->code)->length)
#define parse_state_current_token(P)    ((P)->current_token)
#define parse_state_next_token(P)       ((P)->next_token)

#define parse_state_is_new(P)           (                                               \
                                          (lex_state_in_progress(state)) &&             \
                                          (parse_state_current_token(state) == NULL) && \
                                          (parse_state_next_token(state) == NULL)       \
                                        )

#define parse_state_load(P)             (                                                                       \
                                          (parse_state_is_new(P)) ? (                                       \
                                            (parse_state_current_token(P) = lex_get_next_token(P)),    \
                                            (parse_state_next_token(P) = lex_get_next_token(P))        \
                                          ) : (                                                                 \
                                            (parse_state_current_token(P) = parse_state_next_token(P)), \
                                            (parse_state_next_token(P) = lex_get_next_token(P))         \
                                          )                                                                     \
                                        )

ParseState *ParseState_create(String *code);

#endif
