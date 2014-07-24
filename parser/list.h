#define list_head(list)    ((list)->head)
#define list_tail(list)    ((list)->tail)
#define list_length(list)  ((list)->length)

#define node_data(node)    ((node)->data)
#define node_next(node)    ((node)->next)
#define node_prev(node)    ((node)->prev)

typedef struct ListNode_ {
  void *data;
  struct ListNode_ *next;
  struct ListNode_ *prev;
} ListNode;

typedef struct List_ {
  ListNode *head;
  ListNode *tail;
  int length;
  char *(*inspect)(void *data);
} List;

List      *List_new(char *(*node_inspect)(void *data));
ListNode  *list_append(List *list, void *data);
void       list_print(List *list);
