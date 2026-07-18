#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

#include <stddef.h>

int create_socket(void);
void bind_socket(int sockfd, int port);
int accept_connection(int sockfd);
int accept_connection_with_ip(int sockfd, char *ip_str, size_t size);

#endif /* SOCKET_UTILS_H */
