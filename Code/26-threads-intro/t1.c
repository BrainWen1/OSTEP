// 多线程共享数据
#include <stdio.h>
#include <pthread.h>
#include "my_thread.h"

static volatile int g_count = 0; // 全局变量，多个线程共享

void *my_thread(void *arg) {
    printf("%s: begin\n", (char *)arg);

    for (int i = 0; i < 1e7; i++) {
        g_count++; // 对共享变量进行操作
    }

    printf("%s: end\n", (char *)arg);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;

    printf("main: begin(counter = %d)\n", g_count);

    // 创建线程
    Pthread_create(&t1, NULL, my_thread, "A");
    Pthread_create(&t2, NULL, my_thread, "B");
    // 等待线程结束
    Pthread_join(t1, NULL);
    Pthread_join(t2, NULL);

    printf("main: end(counter = %d)\n", g_count);
    return 0;
}
