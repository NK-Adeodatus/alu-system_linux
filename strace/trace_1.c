#include "strace.h"
#include "syscalls.h"

#define NUM_SYSCALLS (sizeof(syscalls_64_g) / sizeof(syscalls_64_g[0]))

/**
 * get_syscall_name - looks up the name of a syscall by its number
 * @nr: the syscall number
 *
 * Return: pointer to the syscall name, or NULL if unknown
 */
char const *get_syscall_name(unsigned long nr)
{
	if (nr < NUM_SYSCALLS)
		return (syscalls_64_g[nr].name);
	return (NULL);
}

/**
 * print_syscall_name - reads the syscall number of the traced child and
 * prints the matching syscall name, without a trailing newline so the
 * traced program's own output (which shares the same stdout) can land
 * on the same line, exactly as real strace does
 * @child: pid of the traced child process
 * @first: pointer to a flag, 1 if this is the first name printed so far
 */
void print_syscall_name(pid_t child, int *first)
{
	struct user_regs_struct regs;
	char const *name;

	if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
		return;

	name = get_syscall_name((unsigned long)regs.orig_rax);
	if (name == NULL)
		return;

	if (*first)
		*first = 0;
	else
		putchar('\n');
	printf("%s", name);
}

/**
 * parent_trace_name - waits for and traces the child, printing every
 * syscall name as soon as it is intercepted
 * @child: pid of the child process to trace
 */
void parent_trace_name(pid_t child)
{
	int status;
	int entering = 1;
	int first = 1;

	if (waitpid(child, &status, 0) == -1)
		return;

	if (WIFSTOPPED(status))
		print_syscall_name(child, &first);

	while (WIFSTOPPED(status))
	{
		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			break;
		if (waitpid(child, &status, 0) == -1)
			break;

		if (!WIFSTOPPED(status))
			break;

		if (entering)
			print_syscall_name(child, &first);
		entering = !entering;
	}

	if (!first)
		putchar('\n');
}
