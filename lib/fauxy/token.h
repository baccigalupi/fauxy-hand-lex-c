#ifndef __fauxy_token
#define __fauxy_token

#include "../helpers.h"
#include "../dstring.h"
#include "../number.h"

/*
  Token is the node for lexing into a list,
  and then for parsing into a syntax tree

  Bit *value is the string or number associated with the token
*/

typedef enum {
  FX_NOT_A_TOKEN,               // NULL state for iffiness
  FX_TOKEN_LINE_END,            // "\n"
  FX_TOKEN_STATEMENT_END,       // ";"

  FX_TOKEN_NUMBER,
  FX_TOKEN_STRING,
  FX_TOKEN_REGEX,

  FX_TOKEN_ATTRIBUTE_SELECTOR,  // "."
  FX_TOKEN_GROUP_START,         // "("
  FX_TOKEN_ARGUMENTS_START,
  FX_TOKEN_GROUP_END,           // ")"
  FX_TOKEN_ARGUMENTS_END,
  FX_TOKEN_COMMA,               // ","
  FX_TOKEN_SETTER,              // "="
  FX_TOKEN_BLOCK_START,         // "{"
  FX_TOKEN_BLOCK_END,           // "}"
  FX_TOKEN_DEFERRED_ARGUMENT,   // "_"

  FX_TOKEN_BLOCK_DECLARATION,   // "->"
  FX_TOKEN_INJECTOR_OPERATOR,   // "<<"
  FX_TOKEN_COMPOSER_OPERATOR,   // ">>"

  FX_TOKEN_GLOBAL_ID,           // starts with uppercase
  FX_TOKEN_ID                   // method calls and variable names
} TokenType;

typedef enum {
  FX_CLOSING_NULL,              // NULL state for iffiness
  FX_CLOSING_SINGLE_QUOTE,
  FX_CLOSING_DOUBLE_QUOTE,
  FX_CLOSING_LINE_COMMENT,
  FX_CLOSING_BLOCK_COMMENT,
  FX_CLOSING_REGEX
} ClosingBookend;

typedef struct Token {
  TokenType  type;
  void       *value;
  int        line;
  int        column;
  //   TODO: file_id
} Token;


#define token_has_value(T)      (                                       \
                                  ((T)->type) == FX_TOKEN_NUMBER ||     \
                                  ((T)->type) == FX_TOKEN_STRING ||     \
                                  ((T)->type) == FX_TOKEN_GLOBAL_ID ||  \
                                  ((T)->type) == FX_TOKEN_ID            \
                                )

#define token_get_value(T)      (                                                               \
                                  ((T)->type) == FX_TOKEN_NUMBER ? (Number *)((T)->value) :     \
                                  ((T)->type) == FX_TOKEN_STRING ||                             \
                                    ((T)->type) == FX_TOKEN_GLOBAL_ID ||                        \
                                    ((T)->type) == FX_TOKEN_ID ? (String *)((T)->value) : NULL  \
                                )

#define token_line(T)           ((T)->line)
#define token_column(T)         ((T)->column)

Token *Token_create(TokenType type);

#define token_destroy(T)        pfree(T)
#define token_number_value(T)   (number_value((Number *)((T)->value)))
#define token_number_type(T)    (object_type((Number *)object_value(token)))
#define token_string_value(T)   (string_value((String *)((T)->value)))

void inline static token_clear_and_destroy(Token *token) {
  free( object_value(token) );
  pfree(token);
}

#endif
