#include <stdio.h>
#include <string.h>
#include "query_parser.h"

#define TARGET_SIZE 2048

/**
 * get_request_target - Extracts the request-target (the second token
 * of the first line) from a raw HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 * @target: Buffer to store the extracted request-target in
 * @size: Size of the target buffer
 */
static void get_request_target(const char *buffer, char *target, size_t size)
{
	char line[TARGET_SIZE];
	char *method, *saveptr;
	size_t i;

	for (i = 0; buffer[i] != '\0' && buffer[i] != '\r' &&
	     buffer[i] != '\n' && i < sizeof(line) - 1; i++)
		line[i] = buffer[i];
	line[i] = '\0';

	method = strtok_r(line, " ", &saveptr);
	(void)method;

	target[0] = '\0';
	if (saveptr != NULL)
	{
		char *token = strtok_r(NULL, " ", &saveptr);

		if (token != NULL)
			strncpy(target, token, size - 1);
	}
}

/**
 * print_query_string - Prints every key/value pair found in a query
 * string, splitting on '&' and '='
 * @query: The query string (without the leading '?')
 */
static void print_query_string(char *query)
{
	char *pair, *saveptr, *equal_sign;

	pair = strtok_r(query, "&", &saveptr);
	while (pair != NULL)
	{
		equal_sign = strchr(pair, '=');
		if (equal_sign != NULL)
		{
			*equal_sign = '\0';
			printf("Query: \"%s\" -> \"%s\"\n", pair, equal_sign + 1);
		}
		else
		{
			printf("Query: \"%s\" -> \"\"\n", pair);
		}
		pair = strtok_r(NULL, "&", &saveptr);
	}
}

/**
 * print_path_and_queries - Prints the path and all query key/value
 * pairs of the request-target found in a raw HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 */
void print_path_and_queries(const char *buffer)
{
	char target[TARGET_SIZE];
	char *question_mark;

	get_request_target(buffer, target, sizeof(target));

	question_mark = strchr(target, '?');
	if (question_mark != NULL)
		*question_mark = '\0';

	printf("Path: %s\n", target);

	if (question_mark != NULL)
		print_query_string(question_mark + 1);
}
