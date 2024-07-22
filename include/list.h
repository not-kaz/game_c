#ifndef LIST_H
#define LIST_H

#define LIST_NODE_CONTAINER(_ptr_, _type_, _member_) \
	((_type_ *)((char *)(_ptr_) - offsetof(_type_, _member_)))
#define LIST_FOR_EACH(_list_, _node_) \
	for (_node_ = (_list_)->head; _node_; _node_ = (_node_)->next)

struct list_node {
	struct list_node *next;
	struct list_node *prev;
};

struct list {
	struct list_node *head;
	struct list_node *tail;
};

int list_append_node(struct list *list, struct list_node *node);
int list_prepend_node(struct list *list, struct list_node *node);
int list_remove_node(struct list *list, struct list_node *node);

#endif
