#define list_head(list)    ((list)->head)
#define list_tail(list)    ((list)->tail)
#define list_length(list)  ((list)->length)
#define list_inspect(list) ((list)->inspect)
#define node_data(node)    ((node)->data)
#define node_next(node)    ((node)->next)
#define node_prev(node)    ((node)->prev)

typedef struct DlNode_ {
  void *data;
  struct DlNode_ *next;
  struct DlNode_ *prev;
} DlNode;

typedef struct DlList_ {
  DlNode *head;
  DlNode *tail;
  int length;
  char *(*inspect)(void *data);
} DlList;

DlList *DlList_new(char *(*node_inspect)(void *data));
DlNode *DlList_append(DlList *list, void *data);
void    DlList_print(DlList *list);

DlNode *DlNode_new(void *data);
