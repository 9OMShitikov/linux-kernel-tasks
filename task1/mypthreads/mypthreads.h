typedef void* mypthread_t;

int mypthread_create(mypthread_t *thread, void *(*start)(void *), void *arg);

int mypthread_join (mypthread_t THREAD_ID, void ** DATA);

int mypthread_detach(mypthread_t thread);