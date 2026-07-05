#include "strace.h"
#include "syscalls.h"

#define NUM_SYSCALLS (sizeof(syscalls_64_g) / sizeof(syscalls_64_g[0]))

/**
 * get_syscall_entry - looks up the syscall table entry for a given number
 * @nr: the syscall number
 *
 * Return: pointer to the matching table entry, or NULL if unknown
 */
syscall_t const *get_syscall_entry(unsigned long nr)
{
	if (nr < NUM_SYSCALLS)
		return (&syscalls_64_g[nr]);
	return (NULL);
}

/**
 * print_enter - prints the syscall name and its raw parameters in hex,
 * without a closing parenthesis or trailing newline, so the traced
 * program's own output can land in between, exactly as real strace does
 * @entry: pointer to the resolved syscall table entry
 * @regs: pointer to the child's registers at syscall-enter
 */
void print_enter(syscall_t const *entry, struct user_regs_struct const *regs)
{
	unsigned long args[6];
	size_t i;
	size_t n;

	args[0] = regs->rdi;
	args[1] = regs->rsi;
	args[2] = regs->rdx;
	args[3] = regs->r10;
	args[4] = regs->r8;
	args[5] = regs->r9;

	n = entry->nb_params;
	if (n == 1 && entry->params[0] == VOID)
		n = 0;

	printf("%s(", entry->name);
	for (i = 0; i < n; i++)
	{
		if (i > 0)
			printf(", ");
		if (entry->params[i] == VARARGS)
		{
			printf("...");
			break;
		}
		printf("%#lx", args[i]);
	}
}

/**
 * print_exit - prints the closing parenthesis and the return value in
 * hexadecimal, or '?' if no return value could be retrieved
 * @child: pid of the traced child
 * @have_retval: 1 if the exit-stop was reached, 0 if the child exited
 */
void print_exit(pid_t child, int have_retval)
{
	struct user_regs_struct regs;

	if (have_retval && ptrace(PTRACE_GETREGS, child, NULL, &regs) != -1)
		printf(") = %#lx\n", (unsigned long)regs.rax);
	else
		printf(") = ?\n");
}

/**
 * trace_step - waits for the next syscall-enter stop, prints its name
 * and parameters, then waits for the matching exit-stop and prints its
 * closing parenthesis and return value (or '?' if the child exited)
 * @child: pid of the traced child
 *
 * Return: 1 to keep tracing, 0 once the child is done
 */
int trace_step(pid_t child)
{
	int status;
	struct user_regs_struct regs;
	syscall_t const *entry;

	if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1)
		return (0);
	if (waitpid(child, &status, 0) == -1 || !WIFSTOPPED(status))
		return (0);
	if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
		return (0);

	entry = get_syscall_entry((unsigned long)regs.orig_rax);
	if (entry != NULL)
		print_enter(entry, &regs);

	if (ptrace(PTRACE_SYSCALL, child, NULL, NULL) == -1 ||
		waitpid(child, &status, 0) == -1 || !WIFSTOPPED(status))
	{
		if (entry != NULL)
			print_exit(child, 0);
		return (0);
	}

	if (entry != NULL)
		print_exit(child, 1);
	return (1);
}

/**
 * parent_trace_params - waits for and traces the child, printing each
 * syscall's name, raw hex parameters, and hex return value
 * @child: pid of the child process to trace
 */
void parent_trace_params(pid_t child)
{
	int status;
	struct user_regs_struct regs;
	syscall_t const *entry;

	if (waitpid(child, &status, 0) == -1 || !WIFSTOPPED(status))
		return;
	if (ptrace(PTRACE_GETREGS, child, NULL, &regs) == -1)
		return;

	/* the initial exec-stop already carries both syscall nr and rax */
	entry = get_syscall_entry((unsigned long)regs.orig_rax);
	if (entry != NULL)
	{
		print_enter(entry, &regs);
		printf(") = %#lx\n", (unsigned long)regs.rax);
	}

	while (trace_step(child))
		;
}
