#include <stddef.h>
#include "list.h"

void list_append_node(struct list *list, struct list_node *node)
{
	if (!list || !node) {
		return;
	}
	if (!list->head) {
		list_prepend_node(list, node);
	}
	node->prev = list->tail;
	node->next = NULL;
	if (list->tail) {
		list->tail->next = node;
	}
	list->tail = node;
}

void list_prepend_node(struct list *list, struct list_node *node)
{
	if (!list || !node) {
		return;
	}
	node->prev = NULL;
	node->next = list->head;
	if (list->head) {
		list->head->prev = node;
	}
	list->head = node;
}
