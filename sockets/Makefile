CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic

TODO_API_0_SRC = todo_api_0.c socket_utils.c http_request.c http_response.c
TODO_API_1_SRC = todo_api_1.c socket_utils.c http_request.c http_response.c \
		  query_parser.c
TODO_API_2_SRC = todo_api_2.c socket_utils.c http_request.c http_response.c \
		  header_parser.c
TODO_API_3_SRC = todo_api_3.c socket_utils.c http_request.c http_response.c \
		  body_parser.c
TODO_API_4_SRC = todo_api_4.c socket_utils.c http_request.c http_response.c \
		  request_parser.c todo_store.c
TODO_API_5_SRC = todo_api_5.c socket_utils.c http_request.c http_response.c \
		  request_parser.c todo_store.c

.PHONY: all clean

all: todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5

todo_api_0: $(TODO_API_0_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_0_SRC)

todo_api_1: $(TODO_API_1_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_1_SRC)

todo_api_2: $(TODO_API_2_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_2_SRC)

todo_api_3: $(TODO_API_3_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_3_SRC)

todo_api_4: $(TODO_API_4_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_4_SRC)

todo_api_5: $(TODO_API_5_SRC)
	$(CC) $(CFLAGS) -o $@ $(TODO_API_5_SRC)

clean:
	rm -f todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5
