#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <sys/types.h>

#define REQUEST_BUFFER_SIZE 65536

ssize_t read_request(int client_fd, char *buffer, size_t size);
void print_request_line(const char *buffer);

#endif /* HTTP_REQUEST_H */
