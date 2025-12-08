// 8．编写一个程序，创建两个子进程，并使用 pipe()系统调用，
// 将一个子进程的标准输出连接到另一个子进程的标准输入。

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    pid_t c1, c2;

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    c1 = fork();
    if (c1 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (c1 == 0) {
        // First child process
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipefd[1]); // Close original write end

        // Execute a command that produces output
        execlp("ls", "ls", NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    c2 = fork();
    if (c2 < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (c2 == 0) {
        // Second child process
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        close(pipefd[0]); // Close original read end

        // Execute a command that consumes input
        execlp("wc", "wc", "-l", NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipefd[0]); // Close both ends of the pipe in parent
    close(pipefd[1]);

    waitpid(c1, NULL, 0); // Wait for first child
    waitpid(c2, NULL, 0); // Wait for second child

    return 0;
}
