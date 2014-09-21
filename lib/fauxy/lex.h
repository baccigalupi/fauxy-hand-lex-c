#ifndef __fauxy_lex
#define __fauxy_lex

#include <ctype.h>
#include <string.h>

#include "../bricks/node.h"
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

typedef List Tokens;

typedef struct SyntaxGeneratorState {
  String *code;
  int current;
  int line;
  int column;
  Bookend        lexical_bookend;

  List          *semantic_bookends;
} SyntaxGeneratorState;

#define char_is_line_end(C)         (C == '\n' || C == '\r')
#define char_is_statement_end(C)    (C == ';')
#define char_is_single_quote(C)     (C == '\'')
#define char_is_double_quote(C)     (C == '"')
#define char_is_string_bookend(C)   (char_is_single_quote(C) || char_is_double_quote(C))
#define char_is_regex_bookend(C)    (C == '/')
#define char_is_capitalized(C)      (isupper(C))
#define char_is_method_selector(C)  (C == '.')
#define char_opens_group(C)         (C == '(')
#define char_closes_group(C)        (C == ')')
#define char_opens_block(C)         (C == '{')
#define char_closes_block(C)        (C == '}')
#define char_is_setter(C)           (C == '=')
#define char_is_separator(C)        (C == ',')
#define char_is_colon(C)            (C == ':')
#define char_is_deferred_arg(C)     (C == '_')
#define char_is_statement_end(C)    (C == ';')
#define char_is_slash(C)            (C == '/')
#define char_is_star(C)             (C == '*')

#define char_is_syntax(C)           (                                         \
                                      char_is_separator(C) ||                 \
                                      char_opens_group(C)  ||                 \
                                      char_closes_group(C) ||                 \
                                      char_opens_block(C)  ||                 \
                                      char_closes_block(C)                    \
                                    )


#define lex_state_current(L)          ((L)->current)
#define lex_state_line(L)             ((L)->line)
#define lex_state_column(L)           ((L)->column)
#define lex_state_length(L)           (((L)->code)->length)

#define lex_state_char_at(L, I)       (                                         \
                                        (I >= 0 && I < lex_state_length(L)) ?   \
                                          (((L)->code)->value[I]) :             \
                                          '\0'                                  \
                                      )

#define lex_state_current_char(L)     (lex_state_char_at(L, lex_state_current(L)))

#define lex_state_bookends(L)         ((L)->bookends)
#define lex_state_bookends_is_open(L) (list_length(lex_state_bookends(L)) > 0)
#define lex_state_bookend_last(L)     (list_last(lex_state_bookends(L)))

#define lex_state_end_of_word(L)      (                                                                           \
                                        (lex_state_next_char(L)) && (                                             \
                                          (isspace(lex_state_next_char(L))) ||                                    \
                                          (char_is_statement_end(lex_state_next_char(L)))   ||                    \
                                          (char_is_syntax(lex_state_next_char(L)))                                \
                                        )                                                                         \
                                      )

#define lex_state_next_char(L)        (lex_state_char_at(L, lex_state_current(L) + 1))
#define lex_state_advance(L)          ((++ lex_state_current(L)), (++ lex_state_column(L)))
#define lex_state_lexical_bookend(L)  ((L)->lexical_bookend)
#define lex_state_is_open(L)          ((L)->lexical_bookend)

#define lex_state_opens_at_current(L) (!lex_state_is_open(L) && lex_state_is_opening(L))

#define lex_state_will_close(L)     (                                                                                                                 \
                                      (L)->lexical_bookend &&                                                                                         \
                                      (                                                                                                               \
                                        ((((L)->lexical_bookend) == FX_CLOSING_SINGLE_QUOTE) && char_is_single_quote(lex_state_current_char(L)))  ||  \
                                        ((((L)->lexical_bookend) == FX_CLOSING_DOUBLE_QUOTE) && char_is_double_quote(lex_state_current_char(L)))   || \
                                        ((((L)->lexical_bookend) == FX_CLOSING_BLOCK_COMMENT) && char_is_star(lex_state_current_char(L)) &&           \
                                            char_is_slash(lex_state_next_char(L)))                                  ||                                        \
                                        ((((L)->lexical_bookend) == FX_CLOSING_LINE_COMMENT) &&                                                       \
                                            char_is_line_end(lex_state_next_char(L)))                                 ||                                        \
                                        ((((L)->lexical_bookend) == FX_CLOSING_REGEX) &&                                                              \
                                            lex_state_end_of_word(L))                                                                                 \
                                      )                                                                                                               \
                                    )

#define lex_state_close(L)          (                                                                         \
                                      ((L)->lexical_bookend) == FX_CLOSING_BLOCK_COMMENT ?                    \
                                        (lex_state_advance(L)) && ((L)->lexical_bookend = FX_CLOSING_NULL) :  \
                                        ((L)->lexical_bookend = FX_CLOSING_NULL)                              \
                                    )

#define lex_state_start_new_line(L)             ((++ lex_state_line(L)), (lex_state_column(L) = 0))
#define lex_state_opening_block_comment(L)      (char_is_slash(lex_state_current_char(L)) && char_is_star(lex_state_next_char(L)))
#define lex_state_opening_line_comment(L)       (char_is_slash(lex_state_current_char(L)) && char_is_slash(lex_state_next_char(L)))

#define lex_state_is_opening(L)                 (                                                                         \
                                                  char_is_string_bookend(lex_state_current_char(L)) ||                    \
                                                  lex_state_opening_block_comment(L) ||                                   \
                                                  lex_state_opening_line_comment(L) ||                                    \
                                                  char_is_regex_bookend(lex_state_current_char(L))                        \
                                                )

#define lex_state_closer(L)                     (char_is_single_quote(lex_state_current_char(L)) ? FX_CLOSING_SINGLE_QUOTE :                                                        \
                                                  (char_is_double_quote(lex_state_current_char(L))  ? FX_CLOSING_DOUBLE_QUOTE :                                                      \
                                                    (lex_state_opening_block_comment(L) ? FX_CLOSING_BLOCK_COMMENT :                          \
                                                      (lex_state_opening_line_comment(L) ? FX_CLOSING_LINE_COMMENT: FX_OPENING_REGEX)         \
                                                    )                                                                                         \
                                                  )                                                                                           \
                                                )

#define lex_state_opened_comment(L)             (                                                                 \
                                                  (lex_state_lexical_bookend(L) == FX_CLOSING_BLOCK_COMMENT) ||   \
                                                  (lex_state_lexical_bookend(L) == FX_CLOSING_LINE_COMMENT)       \
                                                )

#define lex_state_terminating_regex(L)          (                                                                 \
                                                  (lex_state_lexical_bookend(L) == FX_OPENING_REGEX) &&           \
                                                  (char_is_slash(lex_state_current_char(L)))                      \
                                                )

#define lex_state_current_is_significant(L)     (                                                         \
                                                  (!lex_state_opened_comment(L)) && (                     \
                                                    (                                                     \
                                                      char_is_line_end(lex_state_current_char(L)) ||      \
                                                      !isspace(lex_state_current_char(L))                 \
                                                    ) || (lex_state_is_open(L))                           \
                                                  )                                                       \
                                                )

#define lex_state_transition_regex_if_needed(L) (lex_state_terminating_regex(L) && (lex_state_lexical_bookend(L) = FX_CLOSING_REGEX))

#define lex_state_in_progress(L)                (lex_state_current(L) < lex_state_length(L))

#define lex_state_will_end_word_by_dot(L, S)    (                                                       \
                                                  char_is_method_selector(lex_state_next_char(L)) &&    \
                                                  string_length(S) > 0 &&                               \
                                                  !lexed_word_is_number(string_value(S))                \
                                                )

#define word_is_method_selector(S, C)           (                                                   \
                                                  char_is_method_selector(C) &&                     \
                                                  string_length(S) == 1                             \
                                                )

#define word_is_block_declaration(W)            (strcmp(W, "->") == 0)

#define lexeme_value(L)        ((String *)object_value(L))
#define lexeme_length(L)       (string_length(lexeme_value(L)))


SyntaxGeneratorState  *SyntaxGeneratorState_create(String *code);

Token     *lex_get_next_lexeme(SyntaxGeneratorState *lex_state);
Token     *token_from_lexeme(Token *lexeme);
Boolean    lexed_word_is_number(char *word);

#endif
