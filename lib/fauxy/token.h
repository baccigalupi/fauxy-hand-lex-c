#ifndef __fauxy_token
#define __fauxy_token

#include "bit.h"
#include "../basics.h"

/*
  Token is the node for lexing into a list,
  and then for parsing into a syntax tree

  Bit *value is the string or number associated with the token
*/

typedef enum {
  FX_TOKEN_LINE_END,            // "\n"
  FX_TOKEN_STATEMENT_END,

  FX_TOKEN_NUMBER,
  FX_TOKEN_STRING,

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
  FX_TOKEN_COMMENT_LINE_START,  // "//"
  FX_TOKEN_COMMENT_BLOCK_START, // "/*"
  FX_TOKEN_COMMENT_BLOCK_END,   // "*/"
  FX_TOKEN_INJECTOR_OPERATOR,   // "<<"
  FX_TOKEN_COMPOSER_OPERATOR,   // ">>"

  FX_TOKEN_GLOBAL_ID,           // starts with uppercase
  FX_TOKEN_ID                   // method calls and variable names
} FauxyTokenType;

static char * FauxyTokenTypeDescriptions[] = {
  "Line End",
  "Statement End",

  "Number",
  "String",

  "Attribute Accessor",
  "Group Start",
  "Arguments Start",
  "Group End",
  "Arguments End",
  "Comma",
  "Setter",
  "Block Start",
  "Block End",
  "Deferred Argument",

  "Block Declaration",
  "Comment Line Start",
  "Comment Block Start",
  "Commend Block End",
  "Injector Operator",
  "Composer Operator",

  "Global Id",
  "Id"
};

typedef struct FauxyToken {
  FauxyTokenType  type;
  FauxyBit        *value;

  struct FauxyToken *parent;
  struct FauxyToken *left;
  struct FauxyToken *right;
} FauxyToken;

#define fauxy_token_type(T)       ((T)->type)
#define fauxy_token_value(T)      ((T)->value)
#define fauxy_token_parent(T)     ((T)->parent)
#define fauxy_token_left(T)       ((T)->left)
#define fauxy_token_prev(T)       ((T)->left)
#define fauxy_token_right(T)      ((T)->right)
#define fauxy_token_next(T)       ((T)->right)
#define fauxy_token_destroy(T)    pfree(T)
#define fauxy_token_has_value(T)  (fauxy_token_type(T) == FX_TOKEN_NUMBER || fauxy_token_type(T) == FX_TOKEN_STRING)

FauxyToken *FauxyToken_create(FauxyTokenType type, FauxyBit *bit);

#endif
