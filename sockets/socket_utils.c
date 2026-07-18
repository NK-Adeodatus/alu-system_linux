#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "socket_utils.h"

#define BACKLOG 128

/**
 * create_socket - Creates an IPv4/TCP socket
 *
 * Return: The socket file descriptor on success, exits on failure
 */
int create_socket(void)
{
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	return (sockfd);
}

/**
 * bind_socket - Binds and starts listening on a socket, on any address
 * @sockfd: The socket file descriptor to bind
 * @port: The port number to listen on
 */
void bind_socket(int sockfd, int port)
{
	struct sockaddr_in addr;
	int opt;

	opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
		       &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
}

/**
 * accept_connection - Accepts one incoming connection and prints its IP
 * @sockfd: The listening socket file descriptor
 *
 * Return: The connected client's socket file descriptor
 */
int accept_connection(int sockfd)
{
	struct sockaddr_in client_addr;
	socklen_t addr_len;
	int client_fd;
	char ip_str[INET_ADDRSTRLEN];

	addr_len = sizeof(client_addr);
	client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
	if (client_fd == -1)
	{
		perror("accept");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
	printf("Client connected: %s\n", ip_str);

	return (client_fd);
}
