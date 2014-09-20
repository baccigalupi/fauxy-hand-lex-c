#include <stdlib.h>

#include "error_handling.h"
#include "bricks.h"

#include "list.h"
#include "node.h"

List *List_create() {
  List *list = calloc(1, sizeof(List));
  check_mem(list);
  return list;
error:
  return NULL;
}

void list_push(List *list, void *value) {
  check(list, "list in list_push is NULL");

  Node *new_node = Node_create(value);

  Node *current_tail = list->last;
  if (current_tail) {
    node_next(current_tail) = new_node;
    node_prev(new_node)     = current_tail;
  } else {
    list_node_first(list)   = new_node;
  }

  list_node_last(list) = new_node;
  list_length(list) ++;

  return;
error:
  return;
}

void list_unshift(List *list, void *value) {
  check(list,  "list in list_unshift is NULL");

  Node *new_node = Node_create(value);

  Node *current_head = list->first;
  if (current_head) {
    node_prev(current_head) = new_node;
    node_next(new_node) = current_head;
  } else {
    list_node_last(list) = new_node;
  }

  list_node_first(list) = new_node;
  list_length(list) ++;

  return;
error:
  return;
}

void *list_pop(List *list) {
  check(list,       "list in list_pop is NULL");
  if (!list_node_last(list)) { return NULL; }

  void *value = list_last(list);
  Node *old_tail = list_node_last(list);
  if (!old_tail) { return NULL; }

  Node *new_tail = node_prev(old_tail);
  if (new_tail) {
    node_next(new_tail) = NULL;   // set penultimate next to null
    list_node_last(list) = new_tail;   // set last on list to penultimate
  } else {
    // if there is no new tail, it is empty yo!
    list_node_last(list) = NULL;
    list_node_first(list) = NULL;
  }

  list_length(list) --;          // decrement list count
  pfree(old_tail);          // pfree last node

  return value;
error:
  return NULL;
}

void *list_shift(List *list) {
  check(list,         "list in list_shift is NULL");
  if (!list_node_first(list)) { return NULL; }

  void *value = list_first(list);
  Node *old_head = list_node_first(list);
  if (!old_head) { return NULL; }

  Node *new_head = node_next(old_head);
  list_node_first(list) = new_head;
  if (!new_head) {
    list_node_last(list) = NULL;
  }

  list_length(list) --;
  pfree(old_head);

  return value;
error:
  return NULL;
}

void list_free(List *list) {
  check(list, "list in list_free is NULL");

  Node *node = list_node_first(list);

  if (node != NULL) {
    Node *next;

    while (node != NULL) {
      next = node_next(node);
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

  Node *node = list_node_first(list);
  if (node != NULL) {
    Node *next;

    while (node != NULL) {
      next = node_next(node);
      pfree(node_value(node));
      pfree(node);
      node = next;
    }
  }

  pfree(list);
error:
  return;
}
