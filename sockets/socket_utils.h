#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

int create_socket(void);
void bind_socket(int sockfd, int port);
int accept_connection(int sockfd);

#endif /* SOCKET_UTILS_H */
