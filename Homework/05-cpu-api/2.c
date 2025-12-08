// 2．编写一个打开文件的程序（使用 open() 系统调用），然后调用 fork()创建一个新进程。
// 子进程和父进程都可以访问 open() 返回的文件描述符吗？
// 当它我并发（即同时）写入文件时，会发生什么?

// 两个进程并发的写入同一个文件描述符时，可能会导致数据混乱，因为操作系统不会自动同步对文件的写入操作。
// 为了避免这种情况，通常需要在应用层面实现某种形式的同步机制，例如使用文件锁（flock），以确保同一时间只有一个进程可以写入文件。
// 但是其实对于单次 write 调用，通常允许把 4kb，也就是 4096 个字节的数据原子性的写入文件，
// 也就是说如果两个进程分别写入小于等于 4096 字节的数据，那么它们的写入操作不会相互干扰，
// 但是如果写入的数据量超过 4096 字节，那么就可能会出现数据混乱的情况。
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {

    printf("hello world (pid:%d)\n", (int) getpid());

    // Open a file
    int fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        fprintf(stderr, "Failed to open file\n");
        exit(1);
    }

    int rc = fork(); // create a new process

    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        const char *child_msg = "Child writing to file\n";
        write(fd, child_msg, sizeof("Child writing to file\n") - 1);
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
           rc, (int) getpid());
        const char *parent_msg = "Parent writing to file\n";
        write(fd, parent_msg, sizeof("Parent writing to file\n") - 1);
    }

    close(fd); // Close the file descriptor
    // 子进程关闭他自己的文件描述符，不会影响父进程的文件描述符
    // 文件描述符是进程级别的，每个进程有自己的文件描述符表

    return 0;
}
