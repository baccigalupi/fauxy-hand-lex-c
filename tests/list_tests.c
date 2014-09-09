#include <assert.h>

#include "../lib/bricks/list.h"
#include "lib/mu_assert.h"

char *test_create_list() {
  List *list = List_create();
  mu_assert(list != NULL, "List_create did not create list.");

  list_destroy(list);

  return NULL;
}

char *test_list_first_push() {
  List *list = List_create();
  char *str = "first";

  list_push(list, str);

  mu_assert(list_first(list) == str,   "list_push did not set first node");
  mu_assert(list_last(list)  == str,   "list_push did not set last node");
  mu_assert(list_length(list) == 1,    "list_push did not set list count properly");
  mu_assert(list->first == list->last, "list_push did set first and last to same thing");
  mu_assert(list->first->next == NULL, "list_push did not clear next on new node");
  mu_assert(list->first->prev == NULL, "list_push did not clear prev on new node");

  list_destroy(list);

  return NULL;
}

char *test_list_second_push() {
  List *list = List_create();
  char *str =  "first";
  char *str_2 = "second";

  list_push(list, str);
  list_push(list, str_2);

  mu_assert(list_last(list)  == str_2, "list_push did not set the value of the last node");
  mu_assert(list_length(list) == 2,     "list_push did not set list count properly");

  mu_assert(list->first->next == list->last, "list_push did not set the first node to point to the next");
  mu_assert(list->last->prev == list->first, "list_push did not set the last node to point to the previous");

  list_destroy(list);

  return NULL;
}

char *test_list_additional_pushes() {
  List *list = List_create();
  char *str_1 = "first";
  char *str_2 = "second";
  char *str_3 = "third";

  list_push(list, str_1);
  list_push(list, str_2);
  list_push(list, str_3);

  // basic data
  mu_assert(list_first(list) == str_1, "list_push rearranged first node on additional pushes");
  mu_assert(list_last(list) == str_3,  "list_push did not attach last node correctly after additional pushes");
  mu_assert(list_length(list) == 3,     "list_push did not set count correctly after subsequent pushes");

  // connections between nodes
  Node *first_node =  list->first;
  Node *middle_node = list->first->next;
  Node *last_node =   list->last;

  mu_assert(middle_node->value == str_2,         "list_push did put value on the middle node");

  mu_assert(middle_node->prev == first_node,     "list_push did not connect middle node to previous");
  mu_assert(middle_node->next == last_node,      "list_push did not connect middle node to next");
  mu_assert(last_node->prev == middle_node,      "list_push did not connect last node to middle node");

  list_destroy(list);

  return NULL;
}

char *test_list_pop() {
  List *list = List_create();
  char *str_1 = "first";
  char *str_2 = "second";
  char *str_3 = "third";

  list_push(list, str_1);
  list_push(list, str_2);
  list_push(list, str_3);

  char *value = list_pop(list);
  mu_assert(value == str_3,           "list_pop did not return value");
  mu_assert(list_length(list) == 2,    "list_pop did not decrement count");
  mu_assert(list_last(list) == str_2, "list_pop did not remove last node");
  mu_assert(list->last->next == NULL, "list_pop did not nullify reference to next node on last node");

  list_destroy(list);

  return NULL;
}

char *test_multiple_pops() {
  List *list = List_create();
  char *str_1 = "first";
  char *str_2 = "second";
  char *str_3 = "third";

  list_push(list, str_1);
  list_push(list, str_2);
  list_push(list, str_3);

  char *value = list_pop(list);
  mu_assert(list_length(list) == 2,    "list_pop did not decrement list count");
  value = list_pop(list);
  mu_assert(list_length(list) == 1,    "list_pop did not decrement list count");
  value = list_pop(list);
  mu_assert(list_length(list) == 0,    "list_pop did not decrement list count");

  mu_assert(value == str_1,           "list_pop did not return value");
  mu_assert(list_first(list) == NULL, "list_pop did not nullify first");
  mu_assert(list_last(list) == NULL,  "list_pop did not nullify first");

  value = list_pop(list); // to check that there isn't a memory error

  list_destroy(list);

  return NULL;
}

char *test_list_unshift() {
  List *list = List_create();
  char *str_1 = "first";

  list_unshift(list, str_1);

  mu_assert(list_first(list) == str_1, "list_unshift did not set first node");
  mu_assert(list_last(list)  == str_1, "list_unshift did not set last node");
  mu_assert(list_length(list) == 1,     "list_unshift did not set list count properly");
  mu_assert(list->first == list->last, "list_unshift did set first and last to same thing");
  mu_assert(list->first->next == NULL, "list_unshift did not clear next on new node");
  mu_assert(list->first->prev == NULL, "list_unshift did not clear prev on new node");

  list_destroy(list);

  return NULL;
}

char *test_list_unshift_second_time() {
  List *list = List_create();
  char *str_1 = "first";
  char *str_2 = "second";

  list_unshift(list, str_1);
  list_unshift(list, str_2);

  mu_assert(list_length(list) == 2,     "list_unshift did not set list count properly");
  mu_assert(list_last(list)  == str_1, "list_unshift did not set the value of the last node");
  mu_assert(list_first(list) == str_2, "list_unshift did not set the value of the first node");

  mu_assert(list->first->next == list->last, "list_unshift did not set the first node to point to the next");
  mu_assert(list->last->prev == list->first, "list_unshift did not set the last node to point to the previous");

  list_destroy(list);

  return NULL;
}

char *test_list_unshift_additional_time() {
  List *list = List_create();
  char *str_1 = "first";
  char *str_2 = "second";
  char *str_3 = "third";

  list_unshift(list, str_1);
  list_unshift(list, str_2);
  list_unshift(list, str_3);

  mu_assert(list_length(list) == 3,     "list_unshift did not set list count properly");
  mu_assert(list_last(list)  == str_1, "list_unshift did not set the value of the last node");
  mu_assert(list_first(list) == str_3, "list_unshift did not set the value of the first node");

  // connections between nodes
  Node *first_node =  list->first;
  Node *middle_node = list->first->next;
  Node *last_node =   list->last;

  mu_assert(middle_node->value == str_2,         "list_push did put value on the middle node");

  mu_assert(middle_node->prev == first_node,     "list_push did not connect middle node to previous");
  mu_assert(middle_node->next == last_node,      "list_push did not connect middle node to next");
  mu_assert(last_node->prev == middle_node,      "list_push did not connect last node to middle node");

  list_destroy(list);

  return NULL;
}

char *test_list_shift() {
  List *list = List_create();
  char *str_1 = "first";
  char *str_2 = "second";
  char *str_3 = "third";

  list_push(list, str_1);
  list_push(list, str_2);
  list_push(list, str_3);

  char *value = list_shift(list);
  mu_assert(value == str_1,           "list_shift did not return value");
  mu_assert(list_length(list) == 2,    "list_shift did not decrement count");
  mu_assert(list_first(list) == str_2,"list_shift did not remove first node");

  list_destroy(list);

  return NULL;
}

char *test_multiple_shifts() {
  List *list = List_create();
  char *str_1 = "first";
  char *str_2 = "second";
  char *str_3 = "third";

  list_push(list, str_1);
  list_push(list, str_2);
  list_push(list, str_3);

  char *value = list_shift(list);
  mu_assert(list_length(list) == 2,    "list_shift did not decrement count");
  value = list_shift(list);
  mu_assert(list_length(list) == 1,    "list_shift did not decrement count");
  value = list_shift(list);

  mu_assert(value == str_3,           "list_shift did not return value");
  mu_assert(list_length(list) == 0,    "list_shift did not decrement count");
  mu_assert(list->first == NULL,      "list_shift did not nullify first");
  mu_assert(list->last == NULL,       "list_shift did not nullify last");

  value = list_shift(list); // to make sure there is no memory error

  list_destroy(list);

  return NULL;
}

char *test_clear() {
  List *list = List_create();

  // can't deallocate what you haven't allocated!
  char *str_1 = calloc(10, sizeof(char));
  strcat(str_1, "first");
  char *str_2 = calloc(10, sizeof(char));
  strcat(str_2, "second");
  char *str_3 = calloc(10, sizeof(char));
  strcat(str_3, "third");

  list_push(list, str_1);
  list_push(list, str_2);
  list_push(list, str_3);

  list_clear(list);

  mu_assert(list_first(list) == NULL,         "list_clear did not remove values on first node");
  mu_assert(list->first->next->value == NULL, "list_clear did not remove values on middle node");
  mu_assert(list_last(list) == NULL,          "list_clear did not remove values on last node");

  list_destroy(list);

  return NULL;
}

char *test_clear_and_destroy() {
  List *list = List_create();

  // can't deallocate what you haven't allocated!
  char *str_1 = calloc(10, sizeof(char));
  strcat(str_1, "first");
  char *str_2 = calloc(10, sizeof(char));
  strcat(str_2, "second");
  char *str_3 = calloc(10, sizeof(char));
  strcat(str_3, "third");

  list_push(list, str_1);
  list_push(list, str_2);
  list_push(list, str_3);

  list_clear_and_destroy(list); // just sees that it works without fail

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create_list);

  mu_run_test(test_list_first_push);
  mu_run_test(test_list_second_push);
  mu_run_test(test_list_additional_pushes);

  mu_run_test(test_list_pop);
  mu_run_test(test_multiple_pops);

  mu_run_test(test_list_unshift);
  mu_run_test(test_list_unshift_second_time);
  mu_run_test(test_list_unshift_additional_time);

  mu_run_test(test_list_shift);
  mu_run_test(test_multiple_shifts);

  mu_run_test(test_clear);
  mu_run_test(test_clear_and_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
