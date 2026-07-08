#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/**
 * struct node_s - doubly linked list node
 * @content: pointer to node content
 * @prev: pointer to previous node
 * @next: pointer to next node
 */
typedef struct node_s
{
	void *content;
	struct node_s *prev;
	struct node_s *next;
} node_t;

/**
 * struct list_s - doubly linked list
 * @head: pointer to head node
 * @tail: pointer to tail node
 * @size: number of nodes in list
 */
typedef struct list_s
{
	node_t *head;
	node_t *tail;
	size_t size;
} list_t;

typedef void (*node_func_t)(void *);

list_t *list_init(list_t *list);
void list_add(list_t *list, void *content);
void list_each(list_t const *list, node_func_t func);
void list_destroy(list_t *list, void (*node_free)(void *));

#endif /* LIST_H */
