#include <stdlib.h>

#include "error_handling.h"
#include "helpers.h"

#include "list.h"

List *List_create() {
  List *list = calloc(1, sizeof(List));
  check_mem(list);
  return list;
error:
  return NULL;
}

Node *Node_create(void *value) {
  Node *node = calloc(1, sizeof(Node));
  check_mem(node);
  node->value = value;
  return node;
error:
  return NULL;
}

void  list_push(List *list, void *value) {
  check(list, "list in list_push is NULL");

  Node *new_node = Node_create(value);

  Node *current_tail = list->last;
  if (current_tail) {
    current_tail->next = new_node;
    new_node->prev = current_tail;
  } else {
    list->first = new_node;
  }

  list->last = new_node;
  list->length ++;

  return;
error:
  return;
}

void list_unshift(List *list, void *value) {
  check(list,  "list in list_unshift is NULL");

  Node *new_node = Node_create(value);

  Node *current_head = list->first;
  if (current_head) {
    current_head->prev = new_node;
    new_node->next = current_head;
  } else {
    list->last = new_node;
  }

  list->first = new_node;
  list->length ++;

  return;
error:
  return;
}

void *list_pop(List *list) {
  check(list,       "list in list_pop is NULL");
  check(list->last, "list in list_pop in empty");

  void *value = list_last(list);
  Node *old_tail = list->last;
  check(old_tail, "list in list_pop has nothing to pop");

  Node *new_tail = old_tail->prev;
  if (new_tail) {
    new_tail->next = NULL;   // set penultimate next to null
    list->last = new_tail;   // set last on list to penultimate
  } else {
    // if there is no new tail, it is empty yo!
    list->last = NULL;
    list->first = NULL;
  }

  list->length --;          // decrement list count
  pfree(old_tail);          // pfree last node

  return value;
error:
  return NULL;
}

void *list_shift(List *list) {
  check(list,         "list in list_shift is NULL");
  check(list->first,  "list in list_shift in empty");

  void *value = list_first(list);
  Node *old_head = list->first;
  check(old_head, "list in list_shift is ");

  Node *new_head = old_head->next;
  list->first = new_head;
  if (!new_head) {
    list->last = NULL;
  }

  list->length --;
  pfree(old_head);

  return value;
error:
  return NULL;
}

void list_clear(List *list) {
  check(list, "list is NULL");

  Node *node;
  list_each(list, node) {
    if (node->value) {
      pfree(node->value);
    }
  }
error:
  return;
}

void list_destroy(List *list) {
  check(list, "list in list_destroy is NULL");

  Node *node = list->first;

  if (node != NULL) {
    Node *next;

    while (node != NULL) {
      next = node->next;
      pfree(node);
      node = next;
    }
  }

  pfree(list);
error:
  return;
}

void  list_clear_and_destroy(List *list) {
  check(list, "list in list_clear_and_destroy is NULL");

  Node *node = list->first;
  if (node != NULL) {
    Node *next;

    while (node != NULL) {
      next = node->next;
      pfree(node->value);
      pfree(node);
      node = next;
    }
  }

  pfree(list);
error:
  return;
}
