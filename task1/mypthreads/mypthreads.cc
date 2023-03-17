#define _GNU_SOURCE
#include "mypthreads.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
  pid_t id;
  void *(*fn)(void *);
  void *fn_arg;
  void *fn_ret;
  void *stack_mem;
} mypthread_mem;

int thread_fn(void *arg) {
  mypthread_mem *t_mem = (mypthread_mem *)arg;
  t_mem->fn_ret = (t_mem->fn)(t_mem->fn_arg);
  return 0;
}

void free_mypthread_mem(mypthread_mem *t_mem) {
  free(t_mem->stack_mem);
  free(t_mem);
}

int mypthread_create(mypthread_t *thread, void *(*start)(void *), void *arg) {
  mypthread_mem *thread_mem = (mypthread_mem *)calloc(1, sizeof(mypthread_mem));

  int stack_size = 1024 * 1024;
  void *pchild_stack = malloc(stack_size);
  if (pchild_stack == NULL) {
    exit(EXIT_FAILURE);
  }
  thread_mem->stack_mem = pchild_stack;
  thread_mem->fn = start;
  thread_mem->fn_arg = arg;

  thread_mem->id = clone(
      thread_fn, pchild_stack + stack_size,
      CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD, thread_mem);
  if (thread_mem->id == -1) {
    return -1;
  }
  *thread = thread_mem;
  return 0;
}

int mypthread_join(mypthread_t thread_id, void **data) {
  mypthread_mem *t_mem = (mypthread_mem *)thread_id;

  int wait_status;
  if (waitpid(t_mem->id, &wait_status, 0) != t_mem->id) {
    return -1;
  }
  if (WIFEXITED(wait_status) == 0) {
    return WEXITSTATUS(wait_status);
  }
  *data = t_mem->fn_ret;
  free_mypthread_mem(t_mem);
  return 0;
}

int mypthread_detach(mypthread_t thread) {
  mypthread_mem *t_mem = (mypthread_mem *)thread;
  kill(t_mem->id, SIGKILL);
  if (waitpid(t_mem->id, NULL, 0) != t_mem->id) {
    return -1;
  }
  free_mypthread_mem(t_mem);
  return 0;
}