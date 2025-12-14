// 测试 gettimeofday 的精度
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

int main(void) {
    const int iterations = 1000000000; // 迭代次数

    struct timeval start, end;

    gettimeofday(&start, NULL);

    for (volatile int i = 0; i < iterations; i++);

    gettimeofday(&end, NULL);

    // 计算平均每次调用的开销
    long elapsed_nsec = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_usec - start.tv_usec) * 1000L;
    double overhead_per_call = (double)elapsed_nsec / iterations;
    printf("gettimeofday overhead: %.2f ns per call\n", overhead_per_call);
    
    return 0;
}
