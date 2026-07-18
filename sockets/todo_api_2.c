#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket_utils.h"
#include "http_request.h"
#include "http_response.h"
#include "header_parser.h"

#define PORT 8080

/**
 * handle_client - Handles a single client connection: reads the
 * request, prints its header key/value pairs, and sends back an OK
 * response
 * @client_fd: The connected client's socket file descriptor
 */
void handle_client(int client_fd)
{
	char buffer[REQUEST_BUFFER_SIZE];

	if (read_request(client_fd, buffer, sizeof(buffer)) >= 0)
		print_headers(buffer);

	send_ok_response(client_fd);
	close(client_fd);
}

/**
 * main - Entry point. Opens an IPv4/TCP socket, listens on port 8080,
 * and handles incoming HTTP requests one connection at a time
 *
 * Return: Always 0 (never reached, loop runs forever)
 */
int main(void)
{
	int sockfd, client_fd;

	setvbuf(stdout, NULL, _IOLBF, 0);

	sockfd = create_socket();
	bind_socket(sockfd, PORT);

	printf("Server listening on port %d\n", PORT);

	while (1)
	{
		client_fd = accept_connection(sockfd);
		handle_client(client_fd);
	}

	close(sockfd);
	return (0);
}
