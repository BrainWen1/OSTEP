#ifndef MY_THREADS_API_H
#define MY_THREADS_API_H

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

// 线程创建与等待完成
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

// 动态内存分配
void *Malloc(size_t size) {
    void *ptr = malloc(size);
    assert(ptr != NULL);
    return ptr;
}

// 互斥锁
int Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
    int rc = pthread_mutex_init(mutex, mutexattr);
    assert(rc == 0);
    return rc;
}

int Pthread_mutex_lock(pthread_mutex_t *mutex) {
    int rc = pthread_mutex_lock(mutex);
    assert(rc == 0);
    return rc;
}

int Pthread_mutex_unlock(pthread_mutex_t *mutex) {
    int rc = pthread_mutex_unlock(mutex);
    assert(rc == 0);
    return rc;
}

int Pthread_mutex_destroy(pthread_mutex_t *mutex) {
    int rc = pthread_mutex_destroy(mutex);
    assert(rc == 0);
    return rc;
}

// 条件变量
int Pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *cond_attr) {
    int rc = pthread_cond_init(cond, cond_attr);
    assert(rc == 0);
    return rc;
}

int Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
    int rc = pthread_cond_wait(cond, mutex);
    assert(rc == 0);
    return rc;
}

int Pthread_cond_signal(pthread_cond_t *cond) {
    int rc = pthread_cond_signal(cond);
    assert(rc == 0);
    return rc;
}

int Pthread_cond_destroy(pthread_cond_t *cond) {
    int rc = pthread_cond_destroy(cond);
    assert(rc == 0);
    return rc;
}

#endif // MY_THREADS_API_Hs