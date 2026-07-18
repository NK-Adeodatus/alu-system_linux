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

/**
 * todo_list_to_json - Builds the JSON array representation of every
 * todo currently in the in-memory list
 *
 * Return: A newly allocated JSON string (must be freed by the
 * caller), or NULL on failure. Returns "[]" if the list is empty
 */
char *todo_list_to_json(void)
{
	todo_t *cur;
	char **parts;
	size_t count, i, total, len;
	char *result, *pos;

	count = 0;
	for (cur = head; cur != NULL; cur = cur->next)
		count++;

	if (count == 0)
	{
		result = malloc(3);
		if (result != NULL)
			strcpy(result, "[]");
		return (result);
	}

	parts = malloc(count * sizeof(*parts));
	if (parts == NULL)
		return (NULL);

	total = 2 + (count - 1);
	i = 0;
	for (cur = head; cur != NULL; cur = cur->next)
	{
		parts[i] = todo_to_json(cur);
		total += strlen(parts[i]);
		i++;
	}

	result = malloc(total + 1);
	if (result == NULL)
	{
		for (i = 0; i < count; i++)
			free(parts[i]);
		free(parts);
		return (NULL);
	}

	pos = result;
	*pos++ = '[';
	for (i = 0; i < count; i++)
	{
		len = strlen(parts[i]);
		memcpy(pos, parts[i], len);
		pos += len;
		if (i < count - 1)
			*pos++ = ',';
		free(parts[i]);
	}
	*pos++ = ']';
	*pos = '\0';

	free(parts);
	return (result);
}
