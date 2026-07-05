CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

all: strace_0 strace_1 strace_2

strace_0: main.c trace.c strace.h
	$(CC) $(CFLAGS) main.c trace.c -o strace_0

strace_1: main_1.c trace.c trace_1.c strace.h syscalls.h
	$(CC) $(CFLAGS) main_1.c trace.c trace_1.c -o strace_1

strace_2: main_2.c trace.c trace_2.c strace.h syscalls.h
	$(CC) $(CFLAGS) main_2.c trace.c trace_2.c -o strace_2

clean:
	rm -f strace_0 strace_1 strace_2

.PHONY: all clean
