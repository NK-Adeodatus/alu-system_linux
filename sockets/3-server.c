#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define BACKLOG 128
#define BUFFER_SIZE 1024

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
 *
 * Return: The connected client's socket file descriptor
 */
int accept_client(int sockfd)
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

/**
 * receive_message - Waits for and prints a message from a client socket
 * @client_fd: The connected client's socket file descriptor
 */
void receive_message(int client_fd)
{
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;

	bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_read == -1)
	{
		perror("recv");
		close(client_fd);
		exit(EXIT_FAILURE);
	}

	buffer[bytes_read] = '\0';
	printf("Message received: \"%s\"\n", buffer);
}

/**
 * main - Entry point. Opens an IPv4/TCP socket, listens on port 12345,
 * accepts a single connection, and prints the client's message
 *
 * Return: Always 0 on normal termination
 */
int main(void)
{
	int sockfd, client_fd;

	sockfd = create_socket();
	bind_socket(sockfd);

	if (listen(sockfd, BACKLOG) == -1)
	{
		perror("listen");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Server listening on port %d\n", PORT);

	client_fd = accept_client(sockfd);
	receive_message(client_fd);

	close(client_fd);
	close(sockfd);
	return (0);
}
