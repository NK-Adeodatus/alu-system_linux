#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "http_response.h"

/**
 * send_ok_response - Sends a bare HTTP/1.1 200 OK response
 * @client_fd: The connected client's socket file descriptor
 */
void send_ok_response(int client_fd)
{
	const char *response = "HTTP/1.1 200 OK\r\n\r\n";

	if (send(client_fd, response, strlen(response), 0) == -1)
		perror("send");
}
