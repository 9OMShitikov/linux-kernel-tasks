#pragma once

typedef void *mypthread_t;

int mypthread_create(mypthread_t *thread, void *(*start)(void *), void *arg);

int mypthread_join(mypthread_t thread_id, void **data);

int mypthread_detach(mypthread_t thread);