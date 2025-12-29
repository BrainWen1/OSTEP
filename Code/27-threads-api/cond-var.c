#include <pthread.h>
#include <stdio.h>
#include "my_threads.h"

typedef struct Args {
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
} Args;

static volatile int global = 0;

void *thread_wait(void *args) {
    Args *thread_args = (Args *)args;

    Pthread_mutex_lock(thread_args->mutex); // 加锁
    fprintf(stdout, "Waiting for signal...\n");

    while (global == 0) { // 循环检查并等待
        Pthread_cond_wait(thread_args->cond, thread_args->mutex); // 等待条件变量
    }

    fprintf(stdout, "Received signal: global = %d\n", global);
    Pthread_mutex_unlock(thread_args->mutex); // 解锁

    return NULL;
}

void *thread_signal(void *args) {
    Args *thread_args = (Args *)args;

    Pthread_mutex_lock(thread_args->mutex); // 加锁
    fprintf(stdout, "Before sending signal: global = %d\n", global);

    global = 1; // 修改共享变量
    Pthread_cond_signal(thread_args->cond); // 发送信号

    Pthread_mutex_unlock(thread_args->mutex); // 解锁

    return NULL;
}

int main(void) {
    // 声明并初始化锁
    pthread_mutex_t mutex;
    Pthread_mutex_init(&mutex, NULL);
    // 声明并初始化条件变量
    pthread_cond_t cond;
    Pthread_cond_init(&cond, NULL);

    // 创建等待线程和信号线程
    pthread_t wait_thread, signal_thread;
    Args args = {&mutex, &cond};

    Pthread_create(&wait_thread, NULL, thread_wait, (void *)&args);
    Pthread_create(&signal_thread, NULL, thread_signal, (void *)&args);

    // 等待线程完成
    Pthread_join(wait_thread, NULL);
    Pthread_join(signal_thread, NULL);

    // 销毁锁和条件变量
    Pthread_mutex_destroy(&mutex);
    Pthread_cond_destroy(&cond);

    return 0;
}
