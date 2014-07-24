/*
  Compiling this test:
    `gcc list_test.c ../list.c ../messaging.c -o list_test`
  Then to run
    `./list_test`

  TODO:
    1) List each instead of inspect method passed to list
    2) String aggregation, with printing happening somewhere else
      - allows string comparison
      - printing to stderr
      ...

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../list.h"
#include "../messaging.h"


typedef struct {
  char *name;
} Data;

Data *Data_new(char *name) {
  Data *data = (Data *) malloc(sizeof(Data));
  data->name = name;
  return data;
}

char *Data_inspect (void *void_data) {
  Data *data = (Data *) void_data;
  int  length = strlen(data->name) + 8;

  // this gets garbage collected by the printer
  char *name = (char *) malloc(sizeof(char) * length);
  name[0] = '\0'; // initializes the string to empty so that it does not use existing data

  strcat(name, "<Data ");
  strcat(name, data->name);
  strcat(name, ">");

  return name;
}

// DSL functions
void add_to_list(List *list, char *message) {
  list_append(list, Data_new(message));
}

int main() {
  printf("Expected:\nList length: 3\nList: <Data Hello> => <Data World!> => <Data It works!>\n\n");

  List *list = List_new(Data_inspect);

  add_to_list(list, "Hello");
  add_to_list(list, "World!");
  add_to_list(list, "It works!");

  printf("List length: %d\n", list_length(list));
  list_print(list);

  return 0;
}
