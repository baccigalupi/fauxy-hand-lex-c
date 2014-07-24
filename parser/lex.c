#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "messaging.h"

#define DEFAULT_SOURCE_PATH "./inputs/first.fxy"

FILE * get_source(char *path);
void exception(char *message, char *thing);
void lex(FILE *file);

int main(int argc, char **argv) {
  char *path = argc > 1 ? argv[1] : DEFAULT_SOURCE_PATH;
  FILE *file = get_source(path);

  lex(file);

  return 0;
}

void lex(*file) {

}

FILE *get_source(char *file_path) {
  FILE *file = fopen(file_path,"r");

  if (file == NULL) {
    exception("Unable to open file", file_path);
  }

  return file;
}
