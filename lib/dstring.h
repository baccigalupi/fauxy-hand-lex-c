#ifndef __data_structures_string_h
#define __data_structures_string_h

#include <string.h>

#define STRING_LENGTH_DEFAULT 30

typedef struct String {
  int length;
  int limit;
  char *value;
} String;

#define string_value(S)         ((S)->value)
#define string_limit(S)         ((S)->limit)
#define string_allocation(S)    (((S)->limit) + 1)
#define string_count(S)         ((S)->length)
#define string_length(S)        ((S)->length)
#define string_compare(S, T)    (strcmp(string_value(S), string_value(T)))
#define string_empty(S)         (string_length(S) == 0)

String *String_create(const char *value);
int     string_set(String *string, const char *value);
int     string_expand(String *string, int needed_length);
int     string_concat(String *string, const char *addition);
void    string_destroy(String *string);

static void inline string_push(String *string, char c) {
  char tmp[2];
  tmp[0] = c;
  tmp[1] = '\0';
  strcat(string_value(string), tmp);
  string_length(string) ++;
}

#endif
