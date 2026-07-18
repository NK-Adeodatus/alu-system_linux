#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo_store.h"

static todo_t *head;
static todo_t *tail;
static int next_id;

/**
 * todo_create - Creates a new todo and appends it to the in-memory
 * list, assigning it the next available id
 * @title: The todo's title
 * @description: The todo's description
 *
 * Return: A pointer to the newly created todo, or NULL on failure
 */
todo_t *todo_create(const char *title, const char *description)
{
	todo_t *todo;

	todo = malloc(sizeof(*todo));
	if (todo == NULL)
		return (NULL);

	todo->id = next_id++;
	todo->title = strdup(title);
	todo->description = strdup(description);
	todo->next = NULL;

	if (tail == NULL)
		head = todo;
	else
		tail->next = todo;
	tail = todo;

	return (todo);
}

/**
 * todo_to_json - Builds the JSON representation of a todo
 * @todo: The todo to serialize
 *
 * Return: A newly allocated JSON string (must be freed by the
 * caller), or NULL on failure
 */
char *todo_to_json(const todo_t *todo)
{
	char *json;
	size_t size;

	size = strlen(todo->title) + strlen(todo->description) + 64;
	json = malloc(size);
	if (json == NULL)
		return (NULL);

	snprintf(json, size, "{\"id\":%d,\"title\":\"%s\",\"description\":\"%s\"}",
		 todo->id, todo->title, todo->description);

	return (json);
}
