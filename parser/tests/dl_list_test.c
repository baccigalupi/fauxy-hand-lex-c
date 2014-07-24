/*
  Compiling this test:
    `gcc dl_list_test.c ../dl_list.c ../messaging.c -o dl_list_test`
  Then to run
    `./dl_list_test`

  Expected output:
   List length: 3
   <Data Hello> => <Data World> => <Data It works!>

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../dl_list.h"
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
  int  length = strlen(data->name) + 3;

  // this gets garbage collected by the printer
  char *name = (char *) malloc(length);
  strcat(name, "<Data ");
  strcat(name, data->name);
  strcat(name, ">");

  return name;
}

// DSL functions
void add_to_list(DlList *list, char *message) {
  DlList_append(list, Data_new(message));
}

int main() {
  DlList *list = DlList_new(Data_inspect);


  add_to_list(list, "Hello");
  add_to_list(list, "World!");
  add_to_list(list, "It works!");

  printf("List length: %d\n", list_length(list));
  DlList_print(list);

  return 0;
}
