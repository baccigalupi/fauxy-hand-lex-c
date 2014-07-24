#define token_value(token)    (token->value)
#define token_type(token)     (token->type)

typedef struct {
  char *value;
  int type;
} Token;

Token* Token_new(char value, int type);
void   Token_print(Token *token);
