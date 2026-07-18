CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

TODO_API_0_SRC = todo_api_0.c socket_utils.c http_request.c http_response.c

.PHONY: all clean

all: todo_api_0

todo_api_0: $(TODO_API_0_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_0_SRC)

clean:
	rm -f todo_api_0
