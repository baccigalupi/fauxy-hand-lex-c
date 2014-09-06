#ifndef __fauxy_lex_h
#define __fauxy_lex_h

#include <ctype.h>

#include "../list.h"
#include "../dstring.h"
#include "token.h"

typedef struct LexState {
  String *code;
  int current;
  int line;
  int column;
  ClosingBookend expects_closing;
} LexState;

#define char_is_line_end(C)         (C == '\n' || C == '\r')
#define char_is_statement_end(C)    (C == ';')
#define char_is_string_bookend(C)   (C == '\'' || C == '"')
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

#define char_is_syntax(C)           (                                         \
                                      char_is_separator(C) ||                 \
                                      char_opens_group(C) ||                  \
                                      char_closes_group(C) ||                 \
                                      char_opens_block(C)                     \
                                    )


#define lex_state_current(L)          ((L)->current)
#define lex_state_line(L)             ((L)->line)
#define lex_state_column(L)           ((L)->column)
#define lex_state_length(L)           (((L)->code)->length)
#define lex_state_current_char(L)     (((L)->code)->value[lex_state_current(L)])

#define lex_state_end_of_word(L)      (                                                                           \
                                        (lex_state_next_char(L)) && (                                             \
                                          (isspace(lex_state_next_char(L))) ||                                    \
                                          (lex_state_next_char(L) == ';')   ||                                    \
                                          (char_is_syntax(lex_state_next_char(L)))                                \
                                        )                                                                         \
                                      )

#define lex_state_next_char(L)        (( lex_state_length(L) > lex_state_current(L) ) ? (((L)->code)->value[lex_state_current(L) + 1]) : '\0')
#define lex_state_advance(L)          ((++ lex_state_current(lex_state)) && (++ lex_state_column(L)))
#define lex_state_expects_closing(L)  ((L)->expects_closing)
#define lex_state_is_open(L)          ((L)->expects_closing)

#define lex_state_will_close(L, C)    (                                                                           \
                                        (L)->expects_closing &&                                                   \
                                        (                                                                         \
                                          ((((L)->expects_closing) == FX_CLOSING_SINGLE_QUOTE) && C == '\'')  ||  \
                                          ((((L)->expects_closing) == FX_CLOSING_DOUBLE_QUOTE) && C == '"')   ||  \
                                          ((((L)->expects_closing) == FX_CLOSING_BLOCK_COMMENT) && C == '*' &&    \
                                              lex_state_next_char(L) == '/')                                  ||  \
                                          ((((L)->expects_closing) == FX_CLOSING_LINE_COMMENT) &&                 \
                                              lex_state_next_char(L) == '\n')                                 ||  \
                                          ((((L)->expects_closing) == FX_CLOSING_REGEX) &&                        \
                                              lex_state_end_of_word(L))                                           \
                                        )                                                                         \
                                      )
#define lex_state_close(L)                      (                                                                         \
                                                  ((L)->expects_closing) == FX_CLOSING_BLOCK_COMMENT ?                    \
                                                    (lex_state_advance(L)) && ((L)->expects_closing = FX_CLOSING_NULL) :  \
                                                    ((L)->expects_closing = FX_CLOSING_NULL)                              \
                                                )

#define lex_state_start_new_line(L)             ((++ lex_state_line(lex_state)) && (lex_state_column(lex_state) = 0))
#define lex_state_opening_block_comment(L, C)   (C == '/' && lex_state_next_char(L) == '*')
#define lex_state_opening_line_comment(L, C)    (C == '/' && lex_state_next_char(L) == '/')

#define lex_state_is_opening(L, C)              (                                                                         \
                                                  char_is_string_bookend(C) ||                                            \
                                                  lex_state_opening_block_comment(L, C) ||                                \
                                                  lex_state_opening_line_comment(L, C) ||                                 \
                                                  char_is_regex_bookend(C)                                                \
                                                )

#define lex_state_closer(L, C)                  (C == '\'' ? FX_CLOSING_SINGLE_QUOTE :                                                        \
                                                  (C == '"'  ? FX_CLOSING_DOUBLE_QUOTE :                                                      \
                                                    (lex_state_opening_block_comment(L, C) ? FX_CLOSING_BLOCK_COMMENT :                       \
                                                      (lex_state_opening_line_comment(L, C) ? FX_CLOSING_LINE_COMMENT: FX_OPENING_REGEX)      \
                                                    )                                                                                         \
                                                  )                                                                                           \
                                                )

#define lex_state_opened_comment(L)             (                                                                 \
                                                  (lex_state_expects_closing(L) == FX_CLOSING_BLOCK_COMMENT) ||   \
                                                  (lex_state_expects_closing(L) == FX_CLOSING_LINE_COMMENT)       \
                                                )

#define lex_state_terminating_regex(L, C)       (                                                                 \
                                                  (lex_state_expects_closing(L) == FX_OPENING_REGEX) &&           \
                                                  (C == '/')                                                      \
                                                )

#define lex_state_current_is_significant(L, C)  (                                                                 \
                                                  (!lex_state_opened_comment(L)) && (                             \
                                                    (C == '\n' || C == '\r' || !isspace(C)) ||                    \
                                                    (lex_state_is_open(L))                                        \
                                                  )                                                               \
                                                )

#define lex_state_transition_regex_if_needed(L, C)  (lex_state_terminating_regex(L, C) && (lex_state_expects_closing(L) = FX_CLOSING_REGEX))

#define lex_state_in_progress(L)                (lex_state_current(lex_state) < lex_state_length(lex_state))

#define word_is_method_selected_by_char(S, C)   (                                                     \
                                                    char_is_method_selector(C) &&                     \
                                                    string_length(S) > 0 &&                           \
                                                    !lexed_word_is_number(string_value(S))            \
                                                )

#define word_is_method_selector(S, C)           (                                                     \
                                                    char_is_method_selector(C) &&                     \
                                                    string_length(S) == 1                             \
                                                )

#define word_is_block_declaration(W)            (strcmp(W, "->") == 0)

typedef struct Lexeme {
  String *word;
  int line;
  int column;
} Lexeme;

#define lexeme_word(L)             ((L)->word)
#define lexeme_line(L)             ((L)->line)
#define lexeme_column(L)           ((L)->column)


List      *lex(char *code);

LexState  *LexState_create(String *code);
Lexeme    *lex_get_next_lexeme(LexState *lex_state);
void       list_push_tokens_from_lexeme(List *list, Lexeme *lexeme);
Boolean    lexed_word_is_number(char *word);

Lexeme    *Lexeme_create(String *word, int line, int column);

#endif
