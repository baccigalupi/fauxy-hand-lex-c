#include <string.h>
#include <ctype.h>

#include "lex.h"
#include "token.h"
#include "../error_handling.h"
#include "../dstring.h"
#include "../list.h"

LexState *LexState_create(String *code) {
  LexState *lex_state = calloc(1, sizeof(LexState));
  check_mem(lex_state);
  lex_state->code = code;
  lex_state->line = 1;
  lex_state->column = 1;

  return lex_state;
error:
  return NULL;
}

List *lex(char *str) {
  List *list = List_create();
  check_mem(list);

  String   *code =      String_create(str);
  LexState *lex_state = LexState_create(code);

  Lexeme *lexeme = NULL;

  while( lex_state_in_progress(lex_state) ) {
    pfree(lexeme);
    lexeme = lex_get_next_lexeme(lex_state);
    if (lexeme) {
      list_push(list, token_from_lexeme(lexeme));
    }
  }

  pfree(code);
  pfree(lex_state);
  // free some shit!

  return list;
error:
  return NULL;
}

Token *token_from_lexeme(Lexeme *lexeme) {
  TokenType type;
  char *word = string_value(lexeme_word(lexeme));
  void *value = NULL;
  char first_char = word[0];

  if ( char_is_line_end(first_char) ) {
    type = FX_TOKEN_LINE_END;
  } else if ( char_is_statement_end(first_char) ) {
    type = FX_TOKEN_STATEMENT_END;
  } else if ( char_is_method_selector(first_char) ) {
    type = FX_TOKEN_ATTRIBUTE_SELECTOR;
  } else if ( char_opens_group(first_char) ) {
    type = FX_TOKEN_GROUP_START;
  } else if ( char_closes_group(first_char) ) {
    type = FX_TOKEN_GROUP_END;
  } else if ( char_is_separator(first_char) ) {
    type = FX_TOKEN_COMMA;
  } else if ( char_opens_block(first_char) ) {
    type = FX_TOKEN_BLOCK_START;
  } else if ( char_closes_block(first_char) ) {
    type = FX_TOKEN_BLOCK_END;
  } else if ( char_is_setter(first_char) && string_length(lexeme_word(lexeme)) == 1 ) {
    type = FX_TOKEN_LOCAL_SETTER;
  } else if ( char_is_colon(first_char) && string_length(lexeme_word(lexeme)) == 1 ) {
    type = FX_TOKEN_ATTRIBUTE_SETTER;
  } else if ( char_is_deferred_arg(first_char) && string_length(lexeme_word(lexeme)) == 1 ) {
    type = FX_TOKEN_DEFERRED_ARGUMENT;
  } else if ( word_is_block_declaration(word) ) {
    type = FX_TOKEN_BLOCK_DECLARATION;
  } else if ( char_is_regex_bookend(first_char) ) {
    type = FX_TOKEN_REGEX;
    value = String_create(word);
    check(value, "token string value is NULL");
  } else if ( char_is_string_bookend(first_char) ) {
    type = FX_TOKEN_STRING;
    word[string_length(lexeme_word(lexeme)) - 1] = '\0'; // shortening string contents to remove the quotation marks
    value = String_create((word+1));
    check(value, "token string value is NULL");
  } else if ( lexed_word_is_number(word) ) {
    type = FX_TOKEN_NUMBER;
    value = Number_create(word);
    check(value, "token number is NULL");
  } else if ( char_is_capitalized(first_char) ) {
    type = FX_TOKEN_GLOBAL_ID;
    value = String_create(word);
    check(value, "token string value is NULL");
  } else if ( char_is_colon(first_char) ) {
    type = FX_TOKEN_ATOM;
    value = String_create((word+1));
    check(value, "token string value is NULL");
  } else {
    type = FX_TOKEN_ID;
    value = String_create(word);
    check(value, "token string value is NULL");
  }

  Token  *token =  Token_create(type);
  check(token, "token token is NULL");

  object_value(token) = value;
  token_line(token) =   lexeme_line(lexeme);
  token_column(token) = lexeme_column(lexeme);

  pfree(lexeme_word(lexeme)); // because we created all new strings

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
    // strings, comments, regex, etc ...
    if ( string_empty(word) && lex_state_opens_at_current(lex_state) ) {
      starting_index = lex_state_current(lex_state);
      lex_state_expects_closing(lex_state) = lex_state_closer(lex_state, c);
    }

    if ( lex_state_current_is_significant(lex_state, c) ) {
      starting_index = starting_index ? starting_index : (lex_state_current(lex_state));
      column = column ? column : (lex_state_column(lex_state));
      line =   line   ? line   : (lex_state_line(lex_state));
      string_push(word, c);
    }

    // update lex state for new line
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
    } else if ( lex_state_current_is_significant(lex_state, c) && (
        char_is_line_end(c) || char_is_statement_end(c) ||         // line ends usually significant of a statement end
        lex_state_end_of_word(lex_state) ||                        // end of normal word sequence
        word_is_method_selector(word, c)  ||                       // '.'
        char_is_syntax(c) ||                                       // '(' ')' ','
        char_is_colon(lex_state_next_char(lex_state)) ||           // : appearing after first char breaks the word
        lex_state_will_end_word_by_dot(lex_state, word))       ) { // next char is a dot, and word is not a number
      should_continue = false;
    }

    // move to next character
    lex_state_advance(lex_state);
    c = lex_state_current_char(lex_state);
  }

  if ( string_empty(word) ) {
    pfree(word);
    return NULL;
  }

  Lexeme *lexeme = Lexeme_create(word, line, column);

  return lexeme;
}

Boolean lexed_word_is_number(char *word) {
  Boolean is_valid = isdigit(word[0]) || word[0] == '-';
  if (!is_valid) { return is_valid; }

  int i;
  int length = strlen(word);
  int dot_count = 0;
  int exp_count = 0;
  for(i = 1; i < length; i++) {
    if ( !is_valid ) { break; }
    if ( isdigit(word[i]) ) { continue; }

    // test for decimal ness
    if ( word[i] == '.' ) {
      dot_count ++;
      if ( dot_count > 1 ) {
        is_valid = false;
      }
      continue;
    }

    if ( word[i] == 'E' ) {
      if ( i+1 < length && word[i+1] == '-' ) {
        exp_count ++;
        if ( exp_count > 1 ) {
          is_valid = false;
        }
      } else {
        is_valid = false;
      }
      i++; // advance beyond -
      continue;
    }

    if ( !isdigit(word[i]) ) {
      is_valid = false;
    }
  }

  return is_valid;
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
