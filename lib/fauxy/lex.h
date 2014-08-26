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
} LexState;

#define lex_state_current(L)        ((L)->current)
#define lex_state_line(L)           ((L)->line)
#define lex_state_column(L)         ((L)->column)
#define lex_state_length(L)         (((L)->code)->length)
#define lex_state_current_char(L)   (((L)->code)->value[lex_state_current(L)])
#define lex_state_end_of_word(L)    (lex_state_next_char(L)) && isspace(lex_state_next_char(L))
#define lex_state_next_char(L)      (( lex_state_length(L) > lex_state_current(L) ) ? (((L)->code)->value[lex_state_current(L) + 1]) : '\0')
#define lex_state_advance(L)        ((++ lex_state_current(lex_state)) && (++ lex_state_column(L)))
#define char_is_line_end(C)         (C == '\n' || C == '\r')
#define char_is_significant(C)      (C == '\n' || C == '\r' || !isspace(C))

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
Token     *lexeme_to_token(Lexeme *lexeme);

Lexeme    *Lexeme_create(String *word, int line, int column);

#endif
