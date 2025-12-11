// 6.
// ./bin/data-free-print
// 输出 data[1] = 5，但 data 已经被 free 释放，访问已释放内存，未定义行为，可能引发段错误。

// gdb ./bin/data-free-print
// 输出 data[1] = 5
// [Inferior 1 (process 12443) exited normally]
// 使用 gdb 运行时没有段错误，但这并不意味着代码是正确的，访问已释放内存是未定义行为。

// valgrind ./bin/data-free-print
// ==12646== Invalid read of size 4 // 访问已释放的内存
// ==12646==    at 0x1091C5: main (data-free-print.c:12) // 发生在 main 函数的第 12 行
// ==12646==  Address 0x4a83044 is 4 bytes inside a block of size 40 free'd // 地址 0x4a83044 位于一个大小为 40 字节的已释放内存块内
// ==12646==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so) // 该内存块是在 free 函数中释放的
// ==12646==    by 0x1091BC: main (data-free-print.c:10) // 释放发生在 main 函数的第 10 行
// ==12646==  Block was alloc'd at // 该内存块是在以下位置分配的
// ==12646==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
// ==12646==    by 0x1091AC: main (data-free-print.c:8)
// ==12646==
// data[1] = 0 // 程序输出的值，未定义行为，可能是任何值
// ==12646==
// ==12646== HEAP SUMMARY: // 堆内存使用总结
// ==12646==     in use at exit: 0 bytes in 0 blocks // 程序退出时没有内存泄漏
// ==12646==   total heap usage: 2 allocs, 2 frees, 1,064 bytes allocated // 总共进行了 2 次分配和 2 次释放，共分配了 1064 字节
// ==12646==
// ==12646== All heap blocks were freed -- no leaks are possible // 所有堆内存块都已释放，没有内存泄漏
// ==12646==
// ==12646== For lists of detected and suppressed errors, rerun with: -s
// ==12646== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0) // 总共检测到 1 个内存错误

#include <stdlib.h>
#include <stdio.h>

int main(void) {
    const int SIZE = 10;
    int *data = (int *)malloc(SIZE * sizeof(int));

    free(data);

    printf("data[%d] = %d\n", 1, data[1]); // 访问已释放的内存，未定义行为，可能引发段错误

    return 0;
}
