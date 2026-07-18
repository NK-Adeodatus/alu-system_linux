#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "request_parser.h"

#define LINE_SIZE 2048

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
 * get_method_and_path - Extracts the method and the path (without
 * any query string) from the first line of a raw HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 * @method: Buffer to store the extracted method in
 * @method_size: Size of the method buffer
 * @path: Buffer to store the extracted path in
 * @path_size: Size of the path buffer
 */
void get_method_and_path(const char *buffer, char *method,
			  size_t method_size, char *path, size_t path_size)
{
	char line[LINE_SIZE];
	char *saveptr, *method_tok, *target, *question_mark;
	size_t i;

	for (i = 0; buffer[i] != '\0' && buffer[i] != '\r' &&
	     buffer[i] != '\n' && i < sizeof(line) - 1; i++)
		line[i] = buffer[i];
	line[i] = '\0';

	method[0] = '\0';
	path[0] = '\0';

	method_tok = strtok_r(line, " ", &saveptr);
	if (method_tok != NULL)
		strncpy(method, method_tok, method_size - 1);

	target = strtok_r(NULL, " ", &saveptr);
	if (target == NULL)
		return;

	question_mark = strchr(target, '?');
	if (question_mark != NULL)
		*question_mark = '\0';

	strncpy(path, target, path_size - 1);
}

/**
 * get_content_length - Extracts the value of the Content-Length
 * header from a raw HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 *
 * Return: The Content-Length value, or -1 if the header is missing
 */
long get_content_length(const char *buffer)
{
	const char *line_start, *line_end;
	char line[LINE_SIZE];
	size_t len;
	const char *prefix = "content-length:";

	line_start = buffer;
	while ((line_end = strchr(line_start, '\n')) != NULL)
	{
		len = (size_t)(line_end - line_start);
		if (len > 0 && line_start[len - 1] == '\r')
			len--;
		if (len == 0)
			break;
		if (len >= sizeof(line))
			len = sizeof(line) - 1;

		memcpy(line, line_start, len);
		line[len] = '\0';

		if (len > strlen(prefix) &&
		    strncasecmp(line, prefix, strlen(prefix)) == 0)
			return (strtol(line + strlen(prefix), NULL, 10));

		line_start = line_end + 1;
	}

	return (-1);
}

/**
 * get_body_param - Extracts the value of a named parameter from the
 * body of a raw HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 * @name: The parameter name to look for
 * @value: Buffer to store the extracted value in
 * @size: Size of the value buffer
 *
 * Return: 1 if the parameter was found, 0 otherwise
 */
int get_body_param(const char *buffer, const char *name,
		    char *value, size_t size)
{
	char body[LINE_SIZE];
	char *pair, *saveptr, *equal_sign;
	const char *body_start;

	body_start = find_body(buffer);
	if (body_start == NULL)
		return (0);

	strncpy(body, body_start, sizeof(body) - 1);
	body[sizeof(body) - 1] = '\0';

	pair = strtok_r(body, "&", &saveptr);
	while (pair != NULL)
	{
		equal_sign = strchr(pair, '=');
		if (equal_sign != NULL && strncmp(pair, name,
			(size_t)(equal_sign - pair)) == 0 &&
		    strlen(name) == (size_t)(equal_sign - pair))
		{
			strncpy(value, equal_sign + 1, size - 1);
			value[size - 1] = '\0';
			return (1);
		}
		pair = strtok_r(NULL, "&", &saveptr);
	}

	return (0);
}
