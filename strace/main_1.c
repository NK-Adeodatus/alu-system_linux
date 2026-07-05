#include "strace.h"

/**
 * main - entry point, traces the syscall names made by a given command
 * @argc: argument count
 * @argv: argument vector, argv[1] is the command to trace
 *
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char **argv)
{
	pid_t child;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
		return (1);
	}

	setbuf(stdout, NULL);

	child = fork();
	if (child == -1)
	{
		perror("fork");
		return (1);
	}

	if (child == 0)
		child_exec(argv);
	else
		parent_trace_name(child);

	return (0);
}
