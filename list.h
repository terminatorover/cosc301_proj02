#ifndef __LIST_H__
#define __LIST_H__

/* your list data structure declarations */
struct node {
  pid_t name;
  struct node * next;
};

//void list(void);
/* your function declarations associated with the list */
struct node* add_node(struct node* head, int val);
char* remove_newline(char *s);
int isint (char * str);int isint (char * str);

void list_insert( pid_t *name, struct node **head);
  void list_clear(struct node *list);
#endif // __LIST_H__
