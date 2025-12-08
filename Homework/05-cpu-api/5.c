// 5．现在编写一个程序，在父进程中使用 wait()，等待子进程完成。
// wait() 返回什么？如果你在子进程中使用 wait() 会发生什么？

// wait() 函数用于使父进程等待其子进程的终止。它会阻塞父进程，直到有一个子进程终止，然后返回该子进程的 PID。
// 如果在子进程中直接调用 wait()，则会失败并返回 -1，因为子进程没有子进程可以等待。
// 但是可以先在子进程中创建另一个子进程（即孙子进程），然后在子进程中调用 wait() 来等待孙子进程的终止。
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
        // // Child process
        // printf("Child process (PID: %d) is running...\n", getpid());
        // sleep(1); // Simulate some work in the child process
        // printf("Child process (PID: %d) is exiting...\n", getpid());
        // exit(42); // Exit with a specific status code

        // Create a grandchild process
        pid_t grandchild_pid = fork();
        if (grandchild_pid < 0) {
            perror("Fork failed in child");
            exit(EXIT_FAILURE);
        } else if (grandchild_pid == 0) {
            // Grandchild process
            printf("Grandchild process (PID: %d) is running...\n", getpid());
            sleep(1); // Simulate some work in the grandchild process
            printf("Grandchild process (PID: %d) is exiting...\n", getpid());
            exit(84); // Exit with a specific status code
        } else {
            // Child process waits for grandchild
            int status;
            pid_t waited_pid = wait(&status);

            if (waited_pid == -1) {
                perror("Wait failed in child");
                exit(EXIT_FAILURE);
            }

            printf("Child process (PID: %d) waited for grandchild (PID: %d)\n", getpid(), waited_pid);

            if (WIFEXITED(status)) {
                printf("Grandchild exited with status: %d\n", WEXITSTATUS(status));
            } else {
                printf("Grandchild did not exit normally\n");
            }
        }
        exit(42); // Exit with a specific status code
    } else {
        // Parent process
        int status;
        pid_t waited_pid = wait(&status);

        if (waited_pid == -1) {
            perror("Wait failed");
            exit(EXIT_FAILURE);
        }

        printf("Parent process (PID: %d) waited for child (PID: %d)\n", getpid(), waited_pid);

        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not exit normally\n");
        }
    }

    return 0;
}
