#include "../mypthreads/mypthreads.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void* print_pid(void* arg) {
    int pid = getpid();
    printf("Pid of process: %d\n", pid);
    int *res = (int*)calloc(1, sizeof(int));
    *res = pid;
    return res;
}

void* sleep_fn(void* arg) {
    sleep(1000);
}

int main() {
    mypthread_t thread_id;
    // checking thread execution
    if (mypthread_create(&thread_id, print_pid, NULL)) {
        printf("failed to create thread");
    }

    void* result;
    if (mypthread_join(thread_id, &result)) {
        printf("failed to finish thread");
    }
    printf("Result: %d", *(int *)result);
    free(result);

    // checking thread detach
    if (mypthread_create(&thread_id, print_pid, NULL)) {
        printf("failed to create thread");
    }

    if (mypthread_detach(thread_id)) {
        printf("failed to finish thread");
    }
}