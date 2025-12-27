// 简单线程创建示例
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

void *my_thread(char *arg) {
    printf("%s\n", (char *)arg);
    return NULL;
}

int main(void) {
    pthread_t p1, p2;
    int rc;

    printf("main: begin\n");
    // 创建线程并错误检查
    rc = pthread_create(&p1, NULL, (void *)my_thread, "A"); assert(rc == 0);
    rc = pthread_create(&p2, NULL, (void *)my_thread, "B"); assert(rc == 0);

    // 等待线程结束
    rc = pthread_join(p1, NULL); assert(rc == 0);
    rc = pthread_join(p2, NULL); assert(rc == 0);

    printf("main: end\n");
    return 0;
}
