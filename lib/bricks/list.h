#ifndef __data_structures_list_h
#define __data_structures_list_h

#include "node.h"

typedef struct List {
  int length;
  struct Node *first;
  struct Node *last;
} List;

#define list_length(L)        ((L)->length)
#define list_node_first(L)    ((L)->first)
#define list_first(L)         ((L)->first ? (L)->first->value : NULL)
#define list_node_last(L)     ((L)->last)
#define list_last(L)          ((L)->last == NULL  ? NULL : (L)->last->value)
#define list_each(L, N)       for (N = L->first; N != NULL; N = N->next)

List *List_create();
void  list_push(   List *list, void *value);
void *list_pop(    List *list);
void  list_unshift(List *list, void *value);
void *list_shift(  List *list);

void  list_print(  List *list);
void  list_destroy(List *list);
void  list_clear(  List *list);
void  list_clear_and_destroy(List *list);

#endif
