#include <stdio.h>
#include <string.h>
#include "body_parser.h"

#define LINE_SIZE 2048

/**
 * get_path - Extracts the path (without any query string) from the
 * first line of a raw HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 * @path: Buffer to store the extracted path in
 * @size: Size of the path buffer
 */
static void get_path(const char *buffer, char *path, size_t size)
{
	char line[LINE_SIZE];
	char *saveptr, *target, *question_mark;
	size_t i;

	for (i = 0; buffer[i] != '\0' && buffer[i] != '\r' &&
	     buffer[i] != '\n' && i < sizeof(line) - 1; i++)
		line[i] = buffer[i];
	line[i] = '\0';

	path[0] = '\0';
	strtok_r(line, " ", &saveptr);
	target = strtok_r(NULL, " ", &saveptr);
	if (target == NULL)
		return;

	question_mark = strchr(target, '?');
	if (question_mark != NULL)
		*question_mark = '\0';

	strncpy(path, target, size - 1);
}

/**
 * find_body - Finds the start of the body of a raw HTTP request,
 * i.e. the content right after the blank line that ends the headers
 * @buffer: The raw HTTP request (null-terminated)
 *
 * Return: A pointer to the start of the body, or NULL if not found
 */
static const char *find_body(const char *buffer)
{
	const char *separator;

	separator = strstr(buffer, "\r\n\r\n");
	if (separator != NULL)
		return (separator + 4);

	separator = strstr(buffer, "\n\n");
	if (separator != NULL)
		return (separator + 2);

	return (NULL);
}

/**
 * print_body_params - Prints every key/value pair found in a body
 * string, splitting on '&' and '='
 * @body: A mutable copy of the request body
 */
static void print_body_params(char *body)
{
	char *pair, *saveptr, *equal_sign;

	pair = strtok_r(body, "&", &saveptr);
	while (pair != NULL)
	{
		equal_sign = strchr(pair, '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			printf("Body param: \"%s\" -> \"%s\"\n",
			       pair, equal_sign + 1);
		}
		else
		{
			printf("Body param: \"%s\" -> \"\"\n", pair);
		}
		pair = strtok_r(NULL, "&", &saveptr);
	}
}

/**
 * print_path_and_body - Prints the path and all body parameter
 * key/value pairs of a raw HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 */
void print_path_and_body(const char *buffer)
{
	char path[LINE_SIZE];
	char body[LINE_SIZE];
	const char *body_start;

	get_path(buffer, path, sizeof(path));
	printf("Path: %s\n", path);

	body_start = find_body(buffer);
	if (body_start == NULL || *body_start == '\0')
		return;

	strncpy(body, body_start, sizeof(body) - 1);
	body[sizeof(body) - 1] = '\0';

	print_body_params(body);
}
