#include <tokens.h>

enum TOKEN_TYPES {
  TK_INTEGER, TK_FLOAT, TK_ATOM, TK_STRING,
  TK_L_PAREN, TK_R_PAREN,
  TK_BLOCK_ANNOUNCE, TK_BLOCK_OPEN, TK_BLOCK_CLOSE,
  TK_DOT
};

#define TOKEN_NAMES [
  "Integer", "Float", "Atom", "String",
  "(", ")",
  "Block Announcement", "Block Open", "Block Close",
  "."
]

Token* Token_new(char value, int type) {
  Token *token = malloc(sizeof(Token));
  token_value(token) = value;
  token_type(token) = type;
  return token
}

void   Token_print(void *data) {
  Token *token = (*Token) data; // coerce the pointer
  char *type = TOKEN_NAMES[token_type(token)];

  // Tokens with values
  if (token_type(token) <= TK_STRING) {
    printf("<%s: \"%s\">", type, token_value(token));
  } else {
    printf("<%s>", type);
  }
}
