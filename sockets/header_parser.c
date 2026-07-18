#include <stdio.h>
#include <string.h>
#include "header_parser.h"

#define LINE_SIZE 1024

/**
 * print_header_line - Splits a single header line on the first ':'
 * and prints the resulting key/value pair
 * @line: A single header line (no line terminator)
 */
static void print_header_line(char *line)
{
	char *colon, *value;

	colon = strchr(line, ':');
	if (colon == NULL)
		return;

	*colon = '\0';
	value = colon + 1;
	while (*value == ' ')
		value++;

	printf("Header: \"%s\" -> \"%s\"\n", line, value);
}

/**
 * print_headers - Prints every header key/value pair found in a raw
 * HTTP request, stopping at the blank line that ends the headers
 * @buffer: The raw HTTP request (null-terminated)
 */
void print_headers(const char *buffer)
{
	const char *line_start, *line_end;
	char line[LINE_SIZE];
	int is_request_line;
	size_t len;

	is_request_line = 1;
	line_start = buffer;

	while ((line_end = strchr(line_start, '\n')) != NULL)
	{
		len = (size_t)(line_end - line_start);
		if (len > 0 && line_start[len - 1] == '\r')
			len--;
		if (len >= sizeof(line))
			len = sizeof(line) - 1;

		memcpy(line, line_start, len);
		line[len] = '\0';

		if (is_request_line)
			is_request_line = 0;
		else if (len == 0)
			break;
		else
			print_header_line(line);

		line_start = line_end + 1;
	}
}
