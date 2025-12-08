// 3．使用 fork()编写另一个程序。子进程应打印“hello”，父进程应打印“goodbye”。
//  你应该尝试确保子进程始终先打印。你能否不在父进程调用 wait() 而做到这一点呢？

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("hello\n");
    } else {
        // Parent process
        // Sleep for a short time to allow child to print first
        usleep(100); // Sleep for 100 microseconds
        printf("goodbye\n");
    }

    return 0;
}
