// 5.
// ./bin/data-100
// 数组越界属于 “未定义行为”，并非必然触发段错误，这里越界访问的地址仍属于进程的合法地址空间

// gdb ./bin/data-100
// [Inferior 1 (process 11638) exited normally]
// 使用 gdb 运行该程序时，程序正常退出，没有触发段错误。

// valgrind ./bin/data-100
// ==12026== Invalid write of size 4 // 越界写入
// ==12026==    at 0x10917B: main (data-100.c:13) // 发生在 data-100.c 文件的 main 函数的第 13 行
// ==12026==  Address 0x4a831d0 is 0 bytes after a block of size 400 alloc'd // 访问的地址在分配的 400 字节块之后
// ==12026==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so) // 丢失的内存块是在 malloc 函数中分配的
// ==12026==    by 0x10916C: main (data-100.c:11) // 发生在 data-100.c 文件的 main 函数的第 11 行
// ==12026==
// ==12026==
// ==12026== HEAP SUMMARY: // 堆内存使用概况
// ==12026==     in use at exit: 400 bytes in 1 blocks // 程序退出时仍有 400 字节未释放
// ==12026==   total heap usage: 1 allocs, 0 frees, 400 bytes allocated // 总共分配了 1 次内存，未释放，总共分配了 400 字节
// ==12026==
// ==12026== 400 bytes in 1 blocks are definitely lost in loss record 1 of 1 // 明确丢失了 400 字节内存
// ==12026==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so) // 丢失的内存由 malloc 分配
// ==12026==    by 0x10916C: main (data-100.c:11) // 发生在 data-100.c 文件的 main 函数的第 11 行
// ==12026==
// ==12026== LEAK SUMMARY: // 内存泄漏概况
// ==12026==    definitely lost: 400 bytes in 1 blocks // 明确丢失了 400 字节内存
// ==12026==    indirectly lost: 0 bytes in 0 blocks   // 间接丢失了 0 字节内存
// ==12026==      possibly lost: 0 bytes in 0 blocks   // 可能丢失了 0 字节内存
// ==12026==    still reachable: 0 bytes in 0 blocks   // 仍然可达 0 字节内存
// ==12026==         suppressed: 0 bytes in 0 blocks   // 抑制的 0 字节内存
// ==12026==
// ==12026== For lists of detected and suppressed errors, rerun with: -s
// ==12026== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0) // 总共检测到 2 个内存错误

#include <stdlib.h>

int main(void) {
	const int SIZE = 100;
	int *data = (int *)malloc(SIZE * sizeof(int));

	data[100] = 0; // 越界访问，未定义行为，可能引发段错误

	return 0;
}
