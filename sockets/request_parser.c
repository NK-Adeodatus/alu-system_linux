#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stddef.h>

void get_method_and_path(const char *buffer, char *method,
			  size_t method_size, char *path, size_t path_size);
long get_content_length(const char *buffer);
int get_body_param(const char *buffer, const char *name,
		    char *value, size_t size);

#endif /* REQUEST_PARSER_H */
