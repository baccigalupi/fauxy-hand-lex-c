#include <stdio.h>
#include <stdlib.h>
#include "dl_list.h"
#include "messaging.h"

// private declarations
DlNode *DlNode_new(void *data);
void    DlList_first_node(DlList *list, DlNode *node);
void    DlList_node_print(DlList *list, DlNode *node);

DlList *DlList_new(char *(*node_inspect)(void *data)) {
  DlList *list = malloc(sizeof(DlList));

  list_head(list) = NULL;
  list_tail(list) = NULL;
  list_inspect(list) = node_inspect;
  list_length(list) = 0;

  return list;
}

DlNode *DlNode_new(void *data) {
  DlNode *node = malloc(sizeof(DlNode));
  node_data(node) = (void *) data;

  return node;
}

DlNode *DlList_append(DlList *list, void *data) {
  DlNode *node = DlNode_new(data);

  if (list_head(list) == NULL) {
    DlList_first_node(list, node);
  } else {
    // add node to the end of tail
    list_tail(list)->next = node;
    // connect new tail to the old tail
    node_prev(node) = list_tail(list);
    // tell the list about the new tail
    list_tail(list) = node;
  }

  list_length(list) += 1;
  return node;
}

/**
  * Map returning an array of pointers to anything.
  * That way the function can take a data element, do whatever,
  * and then return whatever.
  *
  * Usage: strings = DlList_map(list, inspect_node);
  *   or with the macros macro(list).map(inspect_node);
***/
void ** DlList_map(DlList *list, void *(*f)(void *data)) {
  if (list_length(list) == 0) {
    return NULL;
  }

  int i;
  void   **returned_values = malloc(sizeof(void *) * list_length(list));
  DlNode *node = list_head(list);

  for (i = 0; i <= list_length(list); i++) {
    returned_values[i] = f(node_data(node));
    node = node_next(node);
  }

  return returned_values;
}

void DlList_print(DlList *list) {
  DlNode *node = list_head(list);
  if (node == NULL) {
    printf("no list elements\n");
    return;
  }

  printf("List: ");
  DlList_node_print(list, node);

  while ( (node = node_next(node)) != NULL ) {
    printf(" => ");
    DlList_node_print(list, node);
  }

  printf("\n");
}

void DlList_node_print(DlList *list, DlNode *node) {
  char *inspected = list_inspect(list)(node_data(node));
  printf("%s", inspected);
  free(inspected);
}


void DlList_first_node(DlList *list, DlNode *node) {
  list_head(list) = node;
  list_tail(list) = node;
}
