// 1．编写一个调用 fork()的程序。在调用 fork()之前，
// 让主进程访问一个变量（例如 x）并将其值设置为某个值（例如 100）。
// 子进程中的变量有什么值？当子进程和父进程都改变 x 的值时，变量会发生什么变化？
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {

    printf("hello world (pid:%d)\n", (int) getpid());

    int x = 100;
    printf("Before fork: x = %d (pid:%d)\n", x, (int) getpid());

    int rc = fork(); // create a new process

    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        printf("Child before change: x = %d (pid:%d)\n", x, (int) getpid());
        x = 200;
        printf("Child after change: x = %d (pid:%d)\n", x, (int) getpid());
    } else {
        // parent goes down this path (original process)
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
           rc, wc, (int) getpid());
        printf("Parent before change: x = %d (pid:%d)\n", x, (int) getpid());
        x = 300;
        printf("Parent after change: x = %d (pid:%d)\n", x, (int) getpid());
    }

    return 0;
}
