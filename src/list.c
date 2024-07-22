#include <stddef.h>
#include "list.h"
#include "result_code.h"

int list_append_node(struct list *list, struct list_node *node)
{
	if (list == NULL || node == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	if (list->head == NULL) {
		list_prepend_node(list, node);
	}
	node->prev = list->tail;
	node->next = NULL;
	if (list->tail) {
		list->tail->next = node;
	}
	list->tail = node;
	return RESULT_CODE_SUCCESS;
}

int list_prepend_node(struct list *list, struct list_node *node)
{
	if (list == NULL || node == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	node->prev = NULL;
	node->next = list->head;
	if (list->head) {
		list->head->prev = node;
	}
	list->head = node;
	return RESULT_CODE_SUCCESS;
}

int list_remove_node(struct list *list, struct list_node *node)
{
	if (list == NULL || node == NULL) {
		return RESULT_CODE_NULL_PTR;
	}
	return RESULT_CODE_SUCCESS;
}
