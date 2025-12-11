// 7.
// ./bin/free-mid-ptr
// free-mid-ptr.c: In function ‘main’:
// free-mid-ptr.c:8:5: warning: ‘free’ called on pointer ‘ptr’ with nonzero offset 20 [-Wfree-nonheap-object]
//     8 |     free(ptr + 5);  // 向 free 函数传递一个非起始地址的指针
//       |     ^~~~~~~~~~~~~
// free-mid-ptr.c:6:16: note: returned from ‘malloc’
//     6 |     int *ptr = malloc(10 * sizeof(int));
//       |                ^~~~~~~~~~~~~~~~~~~~~~~~
// 编译时报警告，但程序仍然可以编译通过。
// free(): invalid pointer
// [1]    13358 IOT instruction (core dumped)  ./bin/free-mid-ptr
// 程序运行时出现运行时错误，提示无效指针错误。

// 你是否需要工具来找到这种类型的问题？
// 不需要，因为编译器已经通过警告提示了这个问题，并且运行时错误也明确指出了无效指针的使用。

#include <stdlib.h>

int main(void) {
    int *ptr = malloc(10 * sizeof(int));

    // free(ptr + 5);  // 向 free 函数传递一个非起始地址的指针
    free(ptr);  // 正确释放内存

    return 0;
}
