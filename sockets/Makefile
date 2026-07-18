CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

TODO_API_0_SRC = todo_api_0.c socket_utils.c http_request.c http_response.c
TODO_API_1_SRC = todo_api_1.c socket_utils.c http_request.c http_response.c \
		  query_parser.c

.PHONY: all clean

all: todo_api_0 todo_api_1

todo_api_0: $(TODO_API_0_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_0_SRC)

todo_api_1: $(TODO_API_1_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_1_SRC)

clean:
	rm -f todo_api_0 todo_api_1
