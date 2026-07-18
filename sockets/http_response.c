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

/**
 * send_status_response - Sends a bare HTTP/1.1 status response with
 * no headers or body
 * @client_fd: The connected client's socket file descriptor
 * @status_code: The HTTP status code (e.g. 404)
 * @status_text: The HTTP status text (e.g. "Not Found")
 */
void send_status_response(int client_fd, int status_code,
			   const char *status_text)
{
	char header[128];
	int header_len;

	header_len = snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\n\r\n",
			       status_code, status_text);

	if (send(client_fd, header, (size_t)header_len, 0) == -1)
		perror("send");
}

/**
 * send_json_response - Sends an HTTP status response with a JSON body
 * @client_fd: The connected client's socket file descriptor
 * @status_code: The HTTP status code (e.g. 201)
 * @status_text: The HTTP status text (e.g. "Created")
 * @json: The JSON body to send (null-terminated)
 */
void send_json_response(int client_fd, int status_code,
			 const char *status_text, const char *json)
{
	char header[256];
	int header_len;
	size_t json_len;

	json_len = strlen(json);
	header_len = snprintf(header, sizeof(header),
			       "HTTP/1.1 %d %s\r\n"
			       "Content-Length: %zu\r\n"
			       "Content-Type: application/json\r\n\r\n",
			       status_code, status_text, json_len);

	if (send(client_fd, header, (size_t)header_len, 0) == -1)
	{
		perror("send");
		return;
	}

	if (send(client_fd, json, json_len, 0) == -1)
		perror("send");
}
