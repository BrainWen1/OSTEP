#ifndef MY_THREADS_API_H
#define MY_THREADS_API_H

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

int Pthread_create(pthread_t *thread,
                     const pthread_attr_t *attr,
                     void *(*start_routine)(void *),
                     void *arg) {
    int rc = pthread_create(thread, attr, start_routine, arg);
    assert(rc == 0);
    return rc;
}

int Pthread_join(pthread_t thread, void **retval) {
    int rc = pthread_join(thread, retval);
    assert(rc == 0);
    return rc;
}

void *Malloc(size_t size) {
    void *ptr = malloc(size);
    assert(ptr != NULL);
    return ptr;
}

#endif // MY_THREADS_API_Hs