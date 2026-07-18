#ifndef TODO_STORE_H
#define TODO_STORE_H

/**
 * struct todo_s - A single todo item
 * @id: Positive integer identifier, starting from 0
 * @title: The todo's title
 * @description: The todo's description
 * @next: Pointer to the next todo in the list
 */
typedef struct todo_s
{
	int id;
	char *title;
	char *description;
	struct todo_s *next;
} todo_t;

todo_t *todo_create(const char *title, const char *description);
char *todo_to_json(const todo_t *todo);
char *todo_list_to_json(void);

#endif /* TODO_STORE_H */
