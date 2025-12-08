// 4．编写一个调用 fork() 的程序，然后调用某种形式的 exec() 来运行程序/bin/ls。
// 看看是否可以尝试 exec() 的所有变体，包括 execl()、execle()、execlp()、execv()、execvp() 和 execvpe()。
// 为什么同样的基本调用会有这么多变种？

// 多种变体的 exec() 函数提供了不同的参数传递方式和环境变量处理方式，以满足不同的编程需求。
// 例如，execl() 和 execv() 允许以不同的方式传递参数列表，而 execle() 和 execvpe() 允许指定自定义环境变量。
// execlp() 和 execvp() 则利用 PATH 环境变量来查找可执行文件，简化了调用过程。
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        // Using execl to run /bin/ls
        // execl("/bin/ls", "ls", NULL); // 参数以列表形式传递，最后一个参数必须是NULL

        // char *my_env[] = { "MYVAR=HelloWorld", NULL };
        // execle("/bin/ls", "ls", NULL, my_env); // 参数以列表形式传递，并且需要自定义环境变量S

        // execlp("ls", "ls", NULL); // 参数以列表形式传递，使用 PATH 环境变量自动查找可执行文件

        // char *args[] = { "ls", "-l", NULL };
        // execv("/bin/ls", args); // 参数以数组形式传递

        char *args[] = { "ls", "-l", NULL };
        execvp("ls", args); // 参数以数组形式传递，使用 PATH 环境变量自动查找可执行文件
        
        // If execl returns, it must have failed
        perror("execl failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL); // Wait for child to finish
        printf("Child process completed\n");
    }

    return 0;
}
