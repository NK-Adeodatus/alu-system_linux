#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket_utils.h"
#include "http_request.h"
#include "http_response.h"
#include "request_parser.h"
#include "todo_store.h"

#define PORT 8080

/**
 * status_text_for - Maps a status code to its HTTP status text
 * @status_code: The HTTP status code
 *
 * Return: The matching status text
 */
static const char *status_text_for(int status_code)
{
	switch (status_code)
	{
	case 201:
		return ("Created");
	case 411:
		return ("Length Required");
	case 422:
		return ("Unprocessable Entity");
	default:
		return ("Not Found");
	}
}

/**
 * handle_post_todos - Handles a POST /todos request: creates a new
 * todo from the request body and sends the appropriate response
 * @client_fd: The connected client's socket file descriptor
 * @buffer: The raw HTTP request
 *
 * Return: The HTTP status code sent back to the client
 */
static int handle_post_todos(int client_fd, const char *buffer)
{
	char title[256], description[1024];
	int has_title, has_description;
	todo_t *todo;
	char *json;

	if (get_content_length(buffer) < 0)
	{
		send_status_response(client_fd, 411, status_text_for(411));
		return (411);
	}

	has_title = get_body_param(buffer, "title", title, sizeof(title));
	has_description = get_body_param(buffer, "description",
					  description, sizeof(description));

	if (!has_title || !has_description)
	{
		send_status_response(client_fd, 422, status_text_for(422));
		return (422);
	}

	todo = todo_create(title, description);
	json = todo_to_json(todo);
	send_json_response(client_fd, 201, status_text_for(201), json);
	free(json);

	return (201);
}

/**
 * handle_client - Handles a single client connection: reads the
 * request, routes it, and logs the outcome
 * @client_fd: The connected client's socket file descriptor
 * @ip: The connected client's IP address
 */
void handle_client(int client_fd, const char *ip)
{
	char buffer[REQUEST_BUFFER_SIZE];
	char method[16], path[256];
	int status;

	if (read_request(client_fd, buffer, sizeof(buffer)) < 0)
	{
		close(client_fd);
		return;
	}

	get_method_and_path(buffer, method, sizeof(method),
			     path, sizeof(path));

	if (strcmp(method, "POST") == 0 && strcmp(path, "/todos") == 0)
		status = handle_post_todos(client_fd, buffer);
	else
	{
		send_status_response(client_fd, 404, status_text_for(404));
		status = 404;
	}

	printf("%s %s %s -> %d %s\n", ip, method, path,
	       status, status_text_for(status));

	close(client_fd);
}

/**
 * main - Entry point. Opens an IPv4/TCP socket, listens on port 8080,
 * and handles incoming REST API requests one connection at a time
 *
 * Return: Always 0 (never reached, loop runs forever)
 */
int main(void)
{
	int sockfd, client_fd;
	char ip[INET_ADDRSTRLEN];

	setvbuf(stdout, NULL, _IOLBF, 0);

	sockfd = create_socket();
	bind_socket(sockfd, PORT);

	printf("Server listening on port %d\n", PORT);

	while (1)
	{
		client_fd = accept_connection_with_ip(sockfd, ip, sizeof(ip));
		handle_client(client_fd, ip);
	}

	close(sockfd);
	return (0);
}
