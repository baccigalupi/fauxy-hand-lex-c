#include <stdio.h>
#include <stdlib.h>
#include "dl_list.h"
#include "messaging.h"

// private declarations
void DlList_first_node(DlList *list, DlNode *node);
void DlList_node_print(DlList *list, DlNode *node);

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
    list_tail(list)->next = node;
    node_prev(node) = list_tail(list);
  }

  list_length(list) += 1;
  return node;
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
