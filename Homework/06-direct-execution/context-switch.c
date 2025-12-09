// 测量上下文切换的成本
// lmbench 基准测试的实现方法，是在单个 CPU 上运行两个进程并在它们之间设置两个 UNIX 管道。
// 管道只是 UNIX 系统中的进程可以相互通信的许多方式之一。第一个进程向第一个管道写入数据，然后等待第二个数据的读取。
// 由于看到第一个进程等待从第二个管道读取的内容，OS 将第一个进程置于阻塞状态，并切换到另一个进程，该进程从第一个管道读取数据，
// 然后写入第二个管理。当第二个进程再次尝试从第一个管道读取时，它会阻塞，从而继续进行通信的往返循环。通过反复测量这种通信的成本，
// lmbench 可以很好地估计上下文切换的成本。你可以尝试使用管道或其他通信机制（例如 UNIX 套接字），重新创建类似的东西。
// 
// 在具有多个 CPU 的系统中，测量上下文切换成本有一点困难。在这样的系统上，你需要确保你的上下文切换进程处于同一个处理器上。
// 幸运的是，大多数操作系统都会提供系统调用，让一个进程绑定到特定的处理器。例如，在 Linux 上，sched_setaffinity()调用就是你要查找的内容。
// 通过确保两个进程位于同一个处理器上，你就能确保在测量操作系统停止一个进程并在同一个 CPU 上恢复另一个进程的成本。
#define _POSIX_C_SOURCE 200809L // 启用 POSIX.1-2008 标准功能
#define _GNU_SOURCE // 启用 GNU 扩展，使用 CPU_SET 等宏

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>
#include <sched.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_ITERATIONS 1000000 // 上下文切换迭代一百万次

// 将当前进程绑定到指定的 CPU
void set_cpu_affinity(int cpu) {
    cpu_set_t cpuset; // CPU 集合

    CPU_ZERO(&cpuset); // 清空 CPU 集合
    CPU_SET(cpu, &cpuset); // 将指定的 CPU 添加到集合中

    // 设置当前进程的 CPU 亲和性
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    int pipe1[2], pipe2[2]; // 两个管道
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); // 创建子进程
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // 子进程
        set_cpu_affinity(0); // 绑定到 CPU 0
        char buf;
        for (int i = 0; i < NUM_ITERATIONS; i++) {
            read(pipe1[0], &buf, 1); // 从管道1读取
            write(pipe2[1], &buf, 1); // 向管道2写入
        }
        exit(0);
    } else { // 父进程
        set_cpu_affinity(0); // 绑定到 CPU 0
        char buf = 'x';
        struct timeval start, end;
        long total_time;

        gettimeofday(&start, NULL); // 获取开始时间

        for (int i = 0; i < NUM_ITERATIONS; i++) {
            write(pipe1[1], &buf, 1); // 向管道1写入
            read(pipe2[0], &buf, 1); // 从管道2读取
        }

        gettimeofday(&end, NULL); // 获取结束时间

        // 计算总时间和平均时间
        total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
        printf("Total time for %d context switches: %ld microseconds\n", NUM_ITERATIONS, total_time);
        printf("Average time per context switch: %.2f microseconds\n", (double)total_time / NUM_ITERATIONS);

        wait(NULL); // 等待子进程结束
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    return 0;
}
