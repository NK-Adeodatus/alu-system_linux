#ifndef STRACE_H
#define STRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>

extern char **environ;

void child_exec(char **argv);
void parent_trace(pid_t child);
void parent_trace_name(pid_t child);
void parent_trace_retval(pid_t child);
void parent_trace_params(pid_t child);

#endif /* STRACE_H */
