#include <string.h>
#include <ctype.h>

#include "lex.h"
#include "token.h"
#include "../error_handling.h"
#include "../dstring.h"
#include "../list.h"

LexState *LexState_create(String *code) {
  LexState *state = calloc(1, sizeof(LexState));
  check_mem(state);
  state->code = code;
  state->line = 1;
  state->column = 1;

  return state;
error:
  return NULL;
}

List *lex(char *str) {
  List *list = List_create();
  check_mem(list);

  String   *code =      String_create(str);
  LexState *lex_state = LexState_create(code);

  Lexeme *lexeme;
  while( lex_state_in_progress(lex_state) ) {
    lexeme = lex_get_next_lexeme(lex_state);
    if (lexeme) {
      list_push(list, lexeme_to_token(lexeme));
    }
  }

  // free some shit!
  // lex_state, etc

  return list;
error:
  return NULL;
}

Token *lexeme_to_token(Lexeme *lexeme) {
  TokenType type;
  char *word = string_value(lexeme_word(lexeme));
  void *value = NULL;
  char first_char = word[0];

  if ( char_is_line_end(first_char) ) {
    type = FX_TOKEN_LINE_END;
  } else if ( char_is_statement_end(first_char) ) {
    type = FX_TOKEN_STATEMENT_END;
  } else if ( char_is_regex_bookend(first_char) ) {
    type = FX_TOKEN_REGEX;
    value = String_create(word);
    check(value, "string is NULL");
  } else if ( char_is_string_bookend(first_char) ) {
    type = FX_TOKEN_STRING;
    word[string_length(lexeme_word(lexeme)) - 1] = '\0';
    value = String_create((word+1));
    check(value, "string is NULL");
  } else {
    type = FX_TOKEN_NUMBER;
    value = Number_create(word);
    check(value, "number is NULL");
  }

  Token  *token =  Token_create(type);
  check(token, "token is NULL");

  object_value(token) = value;
  token_line(token) =   lexeme_line(lexeme);
  token_column(token) = lexeme_column(lexeme);

  // clean up some stuff, lexeme ???

  return token;
error:
  return NULL;
}

Lexeme *lex_get_next_lexeme(LexState *lex_state) {
  String *word = String_create("");

  int starting_index = 0; // used to track progress against comments
  int column = 0;
  int line =   0;
  char c =     lex_state_current_char(lex_state);
  Boolean should_continue = true;

  while ( lex_state_in_progress(lex_state) && should_continue ) {
    // strings, comments ...
    if ( string_length(word) == 0 && !lex_state_is_open(lex_state) && lex_state_is_opening(lex_state, c) ) {
      starting_index = lex_state_current(lex_state);
      lex_state_expects_closing(lex_state) = lex_state_closer(lex_state, c);
    }

    // anything inside a string, line ends, non-space chars
    if ( lex_state_current_is_significant(lex_state, c) ) {
      starting_index = starting_index ? starting_index : (lex_state_current(lex_state));
      column = column ? column : (lex_state_column(lex_state));
      line =   line   ? line   : (lex_state_line(lex_state));
      string_push(word, c);
    }

    if ( char_is_line_end(c) ) {
      lex_state_start_new_line(lex_state);
    }

    // check for termination of strings and other bookends that may contain spaces
    if ( lex_state_is_open(lex_state) && starting_index < lex_state_current(lex_state) ) {
      // regexes are special, because there can be characters after the ending '/'
      // so we have to switch the state
      lex_state_transition_regex_if_needed(lex_state, c);

      if ( lex_state_will_close(lex_state, c) ) {
        lex_state_close(lex_state);
        should_continue = false;
      }
    } else if ( lex_state_current_is_significant(lex_state, c) ) {
      // line ends usually significant of a statement end
      if ( char_is_line_end(c) || char_is_statement_end(c) ) {
        should_continue = false;
      // end of normal word sequence
      } else if ( lex_state_end_of_word(lex_state) ) {
        should_continue = false;
      }
    }

    // move to next character
    lex_state_advance(lex_state);
    c = lex_state_current_char(lex_state);
  }

  if (string_length(word) == 0) {
    pfree(word);
    return NULL;
  }

  Lexeme *lexeme = Lexeme_create(word, line, column);

  return lexeme;
}

Lexeme *Lexeme_create(String *word, int line, int column) {
  Lexeme *lexeme = calloc(1, sizeof(Lexeme));
  check_mem(lexeme);

  lexeme_word(lexeme) = word;
  lexeme_line(lexeme) = line;
  lexeme_column(lexeme) = column;

  return lexeme;
error:
  return NULL;
}
