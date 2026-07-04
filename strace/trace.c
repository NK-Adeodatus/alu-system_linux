#include "strace.h"

/**
 * child_exec - starts being traced, then executes the given command
 * @argv: argument vector, argv[1] is the full path of the binary to run
 */
void child_exec(char **argv)
{
	if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1)
	{
		perror("ptrace");
		exit(1);
	}

	execve(argv[1], &argv[1], environ);
	perror("execve");
	exit(1);
}

/**
 * print_syscall - reads and prints the syscall number of the traced child
 * @child: pid of the traced child process
 */
void print_syscall(pid_t child)
{
	struct user_regs_struct regs;

	if (ptrace(PTRACE_GETREGS, child, NULL, &regs) != -1)
		printf("%lu\n", (unsigned long)regs.orig_rax);
}

/**
 * parent_trace - waits for and traces the child, printing every syscall
 * number as soon as it is intercepted
 * @child: pid of the child process to trace
 */
void parent_trace(pid_t child)
{
	int status;
	int entering = 1;

	waitpid(child, &status, 0);

	while (WIFSTOPPED(status))
	{
		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			break;
		if (waitpid(child, &status, 0) == -1)
			break;

		if (!WIFSTOPPED(status))
			break;

		if (entering)
			print_syscall(child);
		entering = !entering;
	}
}
