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
 * print_name - reads the syscall number of the traced child and prints
 * the matching syscall name, without a trailing newline, so the traced
 * program's own output (sharing the same stdout) can land on the same
 * line before the return value is appended, exactly as real strace does
 * @child: pid of the traced child process
 *
 * Return: 1 if a name was printed, 0 otherwise
 */
int print_name(pid_t child)
{
	struct user_regs_struct regs;
	char const *name;

	if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
		return (0);

	name = get_syscall_name((unsigned long)regs.orig_rax);
	if (name == NULL)
		return (0);

	printf("%s", name);
	return (1);
}

/**
 * print_retval - reads the return value of the traced child's last
 * syscall and prints it in hexadecimal, followed by a newline
 * @child: pid of the traced child process
 */
void print_retval(pid_t child)
{
	struct user_regs_struct regs;

	if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
		return;

	printf(" = %#lx\n", (unsigned long)regs.rax);
}

/**
 * parent_trace_retval - waits for and traces the child, printing each
 * syscall name followed by its return value in hexadecimal; the very
 * last syscall never returns, so a '?' is printed for it instead
 * @child: pid of the child process to trace
 */
void parent_trace_retval(pid_t child)
{
	int status;
	int has_name;

	if (waitpid(child, &status, 0) == -1 || !WIFSTOPPED(status))
		return;

	/* the initial exec-stop already carries both syscall nr and rax */
	has_name = print_name(child);
	if (has_name)
		print_retval(child);

	while (1)
	{
		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
			break;
		if (waitpid(child, &status, 0) == -1 || !WIFSTOPPED(status))
			break;

		has_name = print_name(child);

		if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1 ||
			waitpid(child, &status, 0) == -1 || !WIFSTOPPED(status))
		{
			if (has_name)
				printf(" = ?\n");
			break;
		}

		if (has_name)
			print_retval(child);
	}
}
