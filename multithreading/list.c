#include "list.h"
#include <stdlib.h>

/**
 * list_init - initializes a doubly linked list
 * @list: pointer to the list to initialize
 *
 * Return: pointer to the initialized list
 */
list_t *list_init(list_t *list)
{
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

/**
 * list_each - executes a function on each node of the list
 * @list: pointer to the list
 * @func: function to execute
 */
void list_each(list_t const *list, node_func_t func)
{
	node_t *node;

	if (!list || !func)
		return;
	for (node = list->head; node; node = node->next)
		func(node->content);
}

/**
 * list_add - adds a node to the end of a doubly linked list
 * @list: pointer to the list
 * @content: content to add
 */
void list_add(list_t *list, void *content)
{
	node_t *node;

	if (!list)
		return;

	node = malloc(sizeof(*node));
	if (!node)
		return;

	node->content = content;
	node->next = NULL;
	node->prev = list->tail;

	if (list->tail)
		list->tail->next = node;
	else
		list->head = node;
	list->tail = node;
	list->size++;
}

/**
 * list_destroy - destroys a doubly linked list
 * @list: pointer to the list
 * @node_free: function to free node content
 */
void list_destroy(list_t *list, void (*node_free)(void *))
{
	node_t *node, *next;

	if (!list)
		return;

	for (node = list->head; node; node = next)
	{
		next = node->next;
		if (node_free && node->content)
			node_free(node->content);
		free(node);
	}
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}
