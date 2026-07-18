#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
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
 * bind_socket - Binds a socket to any address on the given port
 * @sockfd: The socket file descriptor to bind
 */
void bind_socket(int sockfd)
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
	addr.sin_port = htons(PORT);

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
}

/**
 * accept_client - Accepts one incoming connection and prints its IP
 * @sockfd: The listening socket file descriptor
 */
void accept_client(int sockfd)
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

	close(client_fd);
}

/**
 * main - Entry point. Opens an IPv4/TCP socket, listens on port 12345,
 * and accepts a single incoming connection
 *
 * Return: Always 0 on normal termination
 */
int main(void)
{
	int sockfd;

	sockfd = create_socket();
	bind_socket(sockfd);

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	accept_client(sockfd);

	close(sockfd);
	return (0);
}
