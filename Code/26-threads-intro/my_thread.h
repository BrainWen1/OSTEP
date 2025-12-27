#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int Pthread_create(
    pthread_t *__restrict__ __newthread,
    const pthread_attr_t *__restrict__ __attr,
    void *(*__start_routine)(void *),
    void *__restrict__ __arg) {
    // 创建线程并检查错误
    int rc = pthread_create(__newthread, __attr, __start_routine, __arg);
    if (rc != 0) {
        // 处理错误（这里简单地打印错误信息并退出）
        fprintf(stderr, "Error creating thread: %d\n", rc);
        exit(1);
    }
    return rc;
}

int Pthread_join(pthread_t __th, void **__thread_return) {
    // 等待线程结束并检查错误
    int rc = pthread_join(__th, __thread_return);
    if (rc != 0) {
        // 处理错误（这里简单地打印错误信息并退出）
        fprintf(stderr, "Error joining thread: %d\n", rc);
        exit(1);
    }
    return rc;
}

#endif // MY_THREAD_H