#ifndef __fauxy_parse_state
#define __fauxy_parse_state

#include "../bricks/list.h"

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
  Bookend        lexical_bookend;

  List *statement_buffer;
} ParseState;

#define parse_state_current(P)          ((P)->current)
#define parse_state_line(P)             ((P)->line)
#define parse_state_column(P)           ((P)->column)
#define parse_state_length(P)           (((P)->code)->length)

#define parse_state_buffer(P)           ((P)->statement_buffer)
#define parse_state_buffer_length(P)    (list_length(parse_state_buffer(P)))
#define parse_state_buffer_push(S, V)   (list_push(parse_state_buffer(S), V))
#define parse_state_buffer_unshift(S)   (list_unshift(parse_state_buffer(S)))

#define parse_state_seeking_statement_end(P)  (                                                                                               \
                                                parse_state_buffer_length(state) > 1 ?                                                        \
                                                  (                                                                                           \
                                                    (list_last(parse_state_buffer(state)) != NULL) &&                                         \
                                                    (                                                                                         \
                                                      (token_type((Token *)list_last(parse_state_buffer(state))) == FX_TOKEN_LINE_END) ||     \
                                                      (token_type((Token *)list_last(parse_state_buffer(state))) == FX_TOKEN_STATEMENT_END)   \
                                                    )                                                                                         \
                                                  )                                                                                           \
                                                  : true                                                                                      \
                                              )

ParseState  *ParseState_create(String *code);
void         parse_state_load_buffer(ParseState *state);

#endif
