#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "http_request.h"

/**
 * read_request - Reads an HTTP request from a client socket and
 * prints the raw request as received
 * @client_fd: The connected client's socket file descriptor
 * @buffer: Buffer to store the received request in
 * @size: Size of the buffer
 *
 * Return: The number of bytes read
 */
ssize_t read_request(int client_fd, char *buffer, size_t size)
{
	ssize_t bytes_read;

	bytes_read = recv(client_fd, buffer, size - 1, 0);
	if (bytes_read == -1)
	{
		perror("recv");
		return (-1);
	}

	buffer[bytes_read] = '\0';
	printf("Raw request: \"%s\"\n", buffer);

	return (bytes_read);
}

/**
 * print_request_line - Prints the Method, Path and Version breakdown
 * of the first line of an HTTP request
 * @buffer: The raw HTTP request (null-terminated)
 */
void print_request_line(const char *buffer)
{
	char line[1024];
	char *method, *path, *version;
	size_t i;

	for (i = 0; buffer[i] != '\0' && buffer[i] != '\r' &&
	     buffer[i] != '\n' && i < sizeof(line) - 1; i++)
		line[i] = buffer[i];
	line[i] = '\0';

	method = strtok(line, " ");
	path = strtok(NULL, " ");
	version = strtok(NULL, " ");

	printf("Method: %s\n", method ? method : "");
	printf("Path: %s\n", path ? path : "");
	printf("Version: %s\n", version ? version : "");
}
