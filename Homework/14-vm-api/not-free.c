// 4.
// ./bin/not-free
// 程序允许时看起来一切正常，没有报错报警告，程序没有崩溃。

// gdb ./bin/not-free
// [Inferior 1 (process 10378) exited normally]
// 使用 gdb 调试程序，程序正常退出，没有崩溃，看上去好像一切正常。

// valgrind --leak-check=yes ./bin/not-free
// ==10416== HEAP SUMMARY:
// ==10416==     in use at exit: 4 bytes in 1 blocks // 堆内存使用总结
// ==10416==   total heap usage: 2 allocs, 1 frees, 1,028 bytes allocated // 整个过程中调用了 2 次内存分配函数，调用了 1 次内存释放函数，堆内存总分配量为 1028 字节
// ==10416==
// ==10416== 4 bytes in 1 blocks are definitely lost in loss record 1 of 1 // 有4字节内存肯定丢失了（内存泄漏）
// ==10416==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so) // 内存泄漏发生在 malloc 函数中
// ==10416==    by 0x10917E: main (not-free.c:5) // 内存泄漏是由main函数中的第5行代码引起的，即 malloc(sizeof(int))
// ==10416==
// ==10416== LEAK SUMMARY:
// ==10416==    definitely lost: 4 bytes in 1 blocks // 明确丢失：4 字节，1 个内存块
// ==10416==    indirectly lost: 0 bytes in 0 blocks // 间接丢失：0 字节，0 个内存块
// ==10416==      possibly lost: 0 bytes in 0 blocks // 可能丢失：0 字节，0 个内存块
// ==10416==    still reachable: 0 bytes in 0 blocks // 仍然可达：0 字节，0 个内存块
// ==10416==         suppressed: 0 bytes in 0 blocks // 抑制的：0 字节，0 个内存块
// ==10416==
// ==10416== For lists of detected and suppressed errors, rerun with: -s
// ==10416== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0) // 错误总结：程序有 1 个内存错误

#include <stdlib.h>
#include <stdio.h>

int main(void) {
	int *ptr = (int *)malloc(sizeof(int));

	*ptr = 666;
	printf("ptr -> %d\n", *ptr);

	// 假装忘记free

	return 0;
}
