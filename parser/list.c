#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "messaging.h"

// private declarations
ListNode *ListNode_new(void *data);
void    List_first_node(List *list, ListNode *node);
void    List_node_print(List *list, ListNode *node);

List *List_new(char *(*node_inspect)(void *data)) {
  List *list = malloc(sizeof(List));

  list_head(list) = NULL;
  list_tail(list) = NULL;
  list->inspect = node_inspect;
  list_length(list) = 0;

  return list;
}

ListNode *ListNode_new(void *data) {
  ListNode *node = malloc(sizeof(ListNode));
  node_data(node) = (void *) data;

  return node;
}

ListNode *list_append(List *list, void *data) {
  ListNode *node = ListNode_new(data);

  if (list_head(list) == NULL) {
    List_first_node(list, node);
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
  * Usage: strings = List_map(list, inspect_node);
  *   or with the macros macro(list).map(inspect_node);
***/
void ** List_map(List *list, void *(*f)(void *data)) {
  if (list_length(list) == 0) {
    return NULL;
  }

  int i;
  void   **returned_values = malloc(sizeof(void *) * list_length(list));
  ListNode *node = list_head(list);

  for (i = 0; i <= list_length(list); i++) {
    returned_values[i] = f(node_data(node));
    node = node_next(node);
  }

  return returned_values;
}

void list_print(List *list) {
  ListNode *node = list_head(list);
  if (node == NULL) {
    printf("no list elements\n");
    return;
  }

  printf("List: ");
  List_node_print(list, node);

  while ( (node = node_next(node)) != NULL ) {
    printf(" => ");
    List_node_print(list, node);
  }

  printf("\n");
}

void List_node_print(List *list, ListNode *node) {
  char *inspected = list->inspect(node_data(node));
  printf("%s", inspected);
  free(inspected);
}


void List_first_node(List *list, ListNode *node) {
  list_head(list) = node;
  list_tail(list) = node;
}
