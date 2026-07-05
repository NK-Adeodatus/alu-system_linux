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
 * prints the matching syscall name
 * @child: pid of the traced child process
 */
void print_syscall_name(pid_t child)
{
	struct user_regs_struct regs;
	char const *name;

	if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
		return;

	name = get_syscall_name((unsigned long)regs.orig_rax);
	if (name != NULL)
		printf("%s\n", name);
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

	if (waitpid(child, &status, 0) == -1)
		return;

	if (WIFSTOPPED(status))
		print_syscall_name(child);

	while (WIFSTOPPED(status))
	{
		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			break;
		if (waitpid(child, &status, 0) == -1)
			break;

		if (!WIFSTOPPED(status))
			break;

		if (entering)
			print_syscall_name(child);
		entering = !entering;
	}
}
