// 1. 当free()的参数是NULL指针时，函数不会执行任何操作，即为合法的无操作。

// 2. [Inferior 1 (process 8826) exited normally] : 第 1 个调试实例 (进程号8826) 正常退出]

// 3. valgrind --leak-check=yes ./bin/null
// ==8959== HEAP SUMMARY:						   // 堆内存使用总结 (左边的数字为进程号 PID)
// ==8959==     in use at exit: 0 bytes in 0 blocks                        // 退出的时候没有未释放的堆内存
// ==8959==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated       // 整个过程中没有调用内存分配函数、没有实际的内存释放操作、堆内存总分配量为 0
// ==8959==
// ==8959== All heap blocks were freed -- no leaks are possible		   // 所有堆空间都释放了，不可能发生内存泄漏
// ==8959==
// ==8959== For lists of detected and suppressed errors, rerun with: -s	   // 对于检测到的和抑制的错误列表，使用：-s 重新运行
// ==8959== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0) // 错误总结：程序没有任何内存错误

#include <stdlib.h>

int main(void) {
	int *ptr = NULL;

	free(ptr);

	return 0;
}
