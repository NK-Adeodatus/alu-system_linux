CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

all: strace_0

strace_0: main.c trace.c strace.h
	$(CC) $(CFLAGS) main.c trace.c -o strace_0

clean:
	rm -f strace_0

.PHONY: all clean
