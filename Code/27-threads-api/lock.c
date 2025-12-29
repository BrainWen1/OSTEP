#include <pthread.h>
#include <stdio.h>
#include "my_threads.h"

typedef struct Args {
    int thread_id;
    pthread_mutex_t *mutex;
} Args;

static volatile int counter = 0; // 共享资源

void *threads_func_unsafe(void *args) {
    fprintf(stdout, "%d: ", (int)args);
    // 临界区开始
    int local_counter = counter; // 读取值
    local_counter++; // 修改值
    counter = local_counter; // 写回值
    // 临界区结束
    fprintf(stdout, "Counter value: %d || ", counter);
    return NULL;
}

void *threads_func_safe(void *args) {
    Args *thread_args = (Args *)args;
    fprintf(stdout, "%d: ", thread_args->thread_id);

    Pthread_mutex_lock(thread_args->mutex);
    // 临界区开始
    int local_counter = counter; // 读取值
    local_counter++; // 修改值
    counter = local_counter; // 写回值
    // 临界区结束
    Pthread_mutex_unlock(thread_args->mutex);

    fprintf(stdout, "Counter value: %d || ", counter);
    return NULL;
}

int main(void) {
    pthread_mutex_t mutex;

    // 初始化互斥锁
    Pthread_mutex_init(&mutex, NULL);
    fprintf(stdout, "====== Mutex initialized ======\n");

    // 使用互斥锁
    Pthread_mutex_lock(&mutex);
    fprintf(stdout, "====== Mutex locked ======\n");
    Pthread_mutex_unlock(&mutex);
    fprintf(stdout, "====== Mutex unlocked ======\n");

    // 销毁互斥锁
    Pthread_mutex_destroy(&mutex);
    fprintf(stdout, "====== Mutex destroyed ======\n");

    // 多线程中的使用
    #define NUM_THREADS 100
    pthread_t threads[NUM_THREADS];

    // 创建多个线程，使用不安全的函数
    for (int i = 0; i < NUM_THREADS; i++) {
        Pthread_create(&threads[i], NULL, threads_func_unsafe, (void *)i);
    }
    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        Pthread_join(threads[i], NULL);
    }
    fprintf(stdout, "\n\nFinal counter value (unsafe): %d\n\n", counter);

    // 安全版本
    counter = 0; // 重置计数器

    pthread_mutex_t shared_mutex;
    Pthread_mutex_init(&shared_mutex, NULL); // 初始化互斥锁

    Args args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].thread_id = i;
        args[i].mutex = &shared_mutex;
        Pthread_create(&threads[i], NULL, threads_func_safe, (void *)&args[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        Pthread_join(threads[i], NULL);
    }
    fprintf(stdout, "\n\nFinal counter value (safe): %d\n", counter);

    Pthread_mutex_destroy(&shared_mutex); // 销毁互斥锁

    return 0;
}
