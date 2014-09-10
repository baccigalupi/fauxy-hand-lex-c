#ifndef __bricks_string
#define __bricks_string

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bricks.h"

#define CHAR      char
#define STRLEN    strlen
#define STRCPY    strcpy
#define STRCAT    strcat

typedef struct String {
  int length;
  int limit;

  CHAR *value;
} String;

#define string_length(S)      ((S)->length)
#define string_limit(S)       ((S)->limit)
#define string_value(S)       ((S)->value)
#define string_free(S)        ((pfree(string_value((String *)S))), (pfree(S)))
#define string_char_at(S, I)  ((string_length(S) > I) ? (string_value(S)[I]) : '\0')
#define string_empty(S)       (string_length(S) == 0)

String *String_create(CHAR *value);
void    string_push(String *string, CHAR c);
void    string_concat(String *string, CHAR *str);
Boolean string_expand(String *string, int length);

static inline int String_limit(int len) {
  int exponent = 1;
  while ( len > 1 ) {
    exponent ++;
    len = len >> 1;
  }
  return pow(2, exponent+1);
}

#endif
