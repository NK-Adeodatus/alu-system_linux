#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>

/**
 * resolve_and_connect - Resolves a host/port and connects to it
 * @host: The hostname or IP address to connect to
 * @port: The port number (as a string) to connect to
 *
 * Return: A connected socket file descriptor, exits on failure
 */
int resolve_and_connect(char *host, char *port)
{
	struct addrinfo hints, *res, *rp;
	int sockfd, status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(host, port, &hints, &res);
	if (status != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	for (rp = res; rp != NULL; rp = rp->ai_next)
	{
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1)
			continue;
		if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;
		close(sockfd);
	}

	freeaddrinfo(res);

	if (rp == NULL)
	{
		fprintf(stderr, "Error: could not connect to %s:%s\n", host, port);
		exit(EXIT_FAILURE);
	}

	return (sockfd);
}

/**
 * main - Entry point. Connects to a listening server at <host> <port>
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on error or bad usage
 */
int main(int argc, char *argv[])
{
	int sockfd;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sockfd = resolve_and_connect(argv[1], argv[2]);

	printf("Connected to %s:%s\n", argv[1], argv[2]);

	close(sockfd);
	return (EXIT_SUCCESS);
}
