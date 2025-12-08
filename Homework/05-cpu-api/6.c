// 6．对前一个程序稍作修改，这次使用 waitpid() 而不是 wait()。
// 什么时候 waitpid() 会有用？

// waitpid() 在需要等待特定子进程结束时非常有用，尤其是在有多个子进程的情况下。
// 它允许父进程指定要等待的子进程的 PID，从而避免了等待错误的子进程。
// 此外，waitpid() 还提供了更多的选项，例如非阻塞等待和处理信号中断等。
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is running...\n", getpid());
        sleep(1); // Simulate some work in the child
        printf("Child process (PID: %d) is exiting...\n", getpid());
        exit(42); // Exit with a specific status code
    } else {
        // Parent process
        int status;
        pid_t waited_pid = waitpid(pid, &status, 0); // Wait for the specific child

        if (waited_pid == -1) {
            perror("waitpid failed");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            printf("Parent process: Child (PID: %d) exited with status %d\n", waited_pid, WEXITSTATUS(status));
        } else {
            printf("Parent process: Child (PID: %d) did not exit normally\n", waited_pid);
        }
    }
    return 0;
}
