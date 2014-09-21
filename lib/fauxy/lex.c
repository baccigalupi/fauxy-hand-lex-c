#include "lex.h"

#include "../bricks/error_handling.h"
#include "../bricks/string.h"

SyntaxGeneratorState *SyntaxGeneratorState_create(String *code) {
  SyntaxGeneratorState *state = calloc(1, sizeof(SyntaxGeneratorState));
  check_mem(state);
  state->code = code;
  state->line = 1;
  state->column = 1;

  return state;
error:
  return NULL;
}

Token *token_from_lexeme(Token *lexeme) {
  char *word = string_value(lexeme_value(lexeme));
  void *value = NULL;
  char first_char = word[0];

  if ( char_is_line_end(first_char) ) {
    token_type(lexeme) = FX_TOKEN_LINE_END;
    string_free(token_value(lexeme));
  } else if ( char_is_statement_end(first_char) ) {
    token_type(lexeme) = FX_TOKEN_STATEMENT_END;
    string_free(token_value(lexeme));
  } else if ( char_is_method_selector(first_char) ) {
    token_type(lexeme) = FX_TOKEN_ATTRIBUTE_SELECTOR;
    string_free(token_value(lexeme));
  } else if ( char_opens_group(first_char) ) {
    token_type(lexeme) = FX_TOKEN_GROUP_START;
    string_free(token_value(lexeme));
  } else if ( char_closes_group(first_char) ) {
    token_type(lexeme) = FX_TOKEN_GROUP_END;
    string_free(token_value(lexeme));
  } else if ( char_is_separator(first_char) ) {
    token_type(lexeme) = FX_TOKEN_COMMA;
    string_free(token_value(lexeme));
  } else if ( char_opens_block(first_char) ) {
    token_type(lexeme) = FX_TOKEN_BLOCK_START;
    string_free(token_value(lexeme));
  } else if ( char_closes_block(first_char) ) {
    token_type(lexeme) = FX_TOKEN_BLOCK_END;
    string_free(token_value(lexeme));
  } else if ( char_is_setter(first_char) && lexeme_length(lexeme) == 1 ) {
    token_type(lexeme) = FX_TOKEN_LOCAL_SETTER;
    string_free(token_value(lexeme));
  } else if ( char_is_colon(first_char) && lexeme_length(lexeme) == 1 ) {
    token_type(lexeme) = FX_TOKEN_ATTRIBUTE_SETTER;
    string_free(token_value(lexeme));
  } else if ( char_is_deferred_arg(first_char) && lexeme_length(lexeme) == 1 ) {
    token_type(lexeme) = FX_TOKEN_DEFERRED_ARGUMENT;
    string_free(token_value(lexeme));
  } else if ( word_is_block_declaration(word) ) {
    token_type(lexeme) = FX_TOKEN_BLOCK_DECLARATION;
    string_free(token_value(lexeme));
  } else if ( char_is_regex_bookend(first_char) ) {
    token_type(lexeme) = FX_TOKEN_REGEX;
  } else if ( char_is_string_bookend(first_char) ) {
    token_type(lexeme) = FX_TOKEN_STRING;
    word[lexeme_length(lexeme) - 1] = '\0'; // shortening string contents to remove the quotation marks
    value = String_create((word+1));
    check(value, "token string value is NULL");
    string_free(token_value(lexeme));
    token_value(lexeme) = value;
  } else if ( lexed_word_is_number(word) ) {
    token_type(lexeme) = FX_TOKEN_NUMBER;
    value = Number_create(word);
    check(value, "token number is NULL");
    string_free(token_value(lexeme));
    token_value(lexeme) = value;
  } else if ( char_is_capitalized(first_char) ) {
    token_type(lexeme) = FX_TOKEN_GLOBAL_ID;
  } else if ( char_is_colon(first_char) ) {
    token_type(lexeme) = FX_TOKEN_ATOM;
    value = String_create((word+1));
    check(value, "token string value is NULL");
    string_free(token_value(lexeme));
    token_value(lexeme) = value;
  } else {
    token_type(lexeme) = FX_TOKEN_ID;
  }

  return lexeme;
error:
  return NULL;
}

Token *lex_get_next_lexeme(SyntaxGeneratorState *state) {
  String *word = String_create("");

  int starting_index = 0; // used to track progress against comments
  int column = 0;
  int line =   0;
  char c =     lex_state_current_char(state);
  Boolean should_continue = true;

  while ( lex_state_in_progress(state) && should_continue ) {
    // strings, comments, regex, etc ...
    if ( string_empty(word) && lex_state_opens_at_current(state) ) {
      starting_index = lex_state_current(state);
      lex_state_lexical_bookend(state) = lex_state_closer(state);
    }

    if ( lex_state_current_is_significant(state) ) {
      starting_index = starting_index ? starting_index : (lex_state_current(state));
      column = column ? column : (lex_state_column(state));
      line =   line   ? line   : (lex_state_line(state));
      string_push(word, c);
    }

    // update lex state for new line
    if ( char_is_line_end(c) ) {
      lex_state_start_new_line(state);
    }

    // check for termination of strings and other bookends that may contain spaces
    if ( lex_state_is_open(state) && starting_index < lex_state_current(state) ) {
      // regexes are special, because there can be characters after the ending '/'
      // so we have to switch the state
      lex_state_transition_regex_if_needed(state);

      if ( lex_state_will_close(state) ) {
        lex_state_close(state);
        should_continue = false;
      }
    } else if ( lex_state_current_is_significant(state) && (
        char_is_line_end(c) || char_is_statement_end(c) ||         // line ends usually significant of a statement end
        lex_state_end_of_word(state) ||                        // end of normal word sequence
        word_is_method_selector(word, c)  ||                       // '.'
        char_is_syntax(c) ||                                       // '(' ')' ','
        char_is_colon(lex_state_next_char(state)) ||           // : appearing after first char breaks the word
        lex_state_will_end_word_by_dot(state, word))       ) { // next char is a dot, and word is not a number
      should_continue = false;
    }

    // move to next character
    lex_state_advance(state);
    c = lex_state_current_char(state);
  }

  if ( string_empty(word) ) {
    string_free(word);
    return NULL;
  }

  Token *lexeme = Token_create(word, line, column);
  check_mem(lexeme);

  return lexeme;
error:
  return NULL;
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
