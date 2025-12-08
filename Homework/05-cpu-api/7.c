// 7．编写一个创建子进程的程序，然后在子进程中关闭标准输出（STDOUT_FILENO）。
// 如果子进程在关闭描述符后调用 printf() 打印输出，会发生什么？

// 关闭标准输出的文件标识符之后，子进程调用 printf() 打印输出时，
// 该输出将不会显示在终端或任何标准输出设备上，因为标准输出已经被关闭。
// 这通常会导致输出丢失，除非标准输出被重定向到其他地方。
// 但是程序不会崩溃或报错，printf() 调用仍然会成功执行，只是输出不会被看到。
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // child process
        close(STDOUT_FILENO); // Close standard output
        printf("This message will not be printed to standard output.\n");
    } else {
        // parent process
        wait(NULL); // Wait for child process to finish
        printf("Child process has finished execution.\n");
    }

    return 0;
}
