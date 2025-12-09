// 测量系统调用的成本
// 可以重复调用一个简单的系统调用（例如，执行 0 字节读取）并记下所花的时间。
// 将时间除以迭代次数，就可以估计系统调用的成本。
// 时钟精确度：gettimeofday vs. clock_gettime vs. rdtsc
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>

#define NUM_ITERATIONS 1000000

int main(void) {

    // gettimeofday 测量
    struct timeval start, end; // 记录开始和结束时间
    long total_time; // 总时间（微秒）

    int fd = open("/dev/null", O_RDONLY); // 打开 /dev/null 进行读取，null 是空设备，不会产生任何实际数据
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL); // 获取开始时间

    for (int i = 0; i < NUM_ITERATIONS; i++) { // 重复系统调用一百万次
        ssize_t result = read(fd, NULL, 0); // 执行 0 字节读取
        if (result < 0) { // 错误处理
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    gettimeofday(&end, NULL); // 获取结束时间
    close(fd);

    // 计算总时间和平均时间
    total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    printf("Total time for %d iterations: %ld microseconds\n", NUM_ITERATIONS, total_time);
    printf("Average time per syscall: %.2f microseconds\n", (double)total_time / NUM_ITERATIONS);

    // clock_gettime 测量
    printf("--------------------------------\n");
    struct timespec start_ts, end_ts; // 记录开始和结束时间
    long total_time_ns; // 总时间（纳秒）

    fd = open("/dev/null", O_RDONLY); // 打开 /dev/null 进行读取
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_MONOTONIC, &start_ts); // 获取开始时间

    for (int i = 0; i < NUM_ITERATIONS; i++) { // 重复系统调用一百万次
        ssize_t result = read(fd, NULL, 0); // 执行 0 字节读取
        if (result < 0) { // 错误处理
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end_ts); // 获取结束时间
    close(fd);

    // 计算总时间和平均时间
    total_time_ns = (end_ts.tv_sec - start_ts.tv_sec) * 1000000000 + (end_ts.tv_nsec - start_ts.tv_nsec);
    printf("Total time for %d iterations: %ld nanoseconds\n", NUM_ITERATIONS, total_time_ns);
    printf("Average time per syscall: %.2f nanoseconds\n", (double)total_time_ns / NUM_ITERATIONS);

    // rdtsc 测量
    printf("--------------------------------\n");
    unsigned long long start_cycles, end_cycles, total_cycles; // CPU 时钟周期数
    unsigned int aux; // 用于存储 rdtsc 的高 32 位

    fd = open("/dev/null", O_RDONLY); // 打开 /dev/null 进行读取
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 使用内联汇编读取时间戳计数器
    __asm__ volatile ("lfence; rdtsc" : "=a" (start_cycles), "=d" (aux) : : "memory");
    start_cycles = (start_cycles) | ((unsigned long long)aux << 32); // 拼接高低 32 位

    for (int i = 0; i < NUM_ITERATIONS; i++) { // 重复系统调用一百万次
        ssize_t result = read(fd, NULL, 0); // 执行 0 字节读取
        if (result < 0) { // 错误处理
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    // 再次读取时间戳计数器
    __asm__ volatile ("rdtsc; lfence" : "=a" (end_cycles), "=d" (aux) : : "memory");
    end_cycles = (end_cycles) | ((unsigned long long)aux << 32); // 拼接高低 32 位
    close(fd);

    // 计算总周期数和平均周期数
    total_cycles = end_cycles - start_cycles;
    printf("Total cycles for %d iterations: %llu cycles\n", NUM_ITERATIONS, total_cycles);
    printf("Average cycles per syscall: %.2f cycles\n", (double)total_cycles / NUM_ITERATIONS);

    return 0;
}
