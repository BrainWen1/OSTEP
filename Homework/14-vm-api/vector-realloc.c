// 8．尝试一些其他接口来分配内存。
// 例如，创建一个简单的向量似的数据结构，以及使用 realloc() 来管理向量的相关函数。
// 使用数组来存储向量元素。当用户在向量中添加条目时， 请使用 realloc() 为其分配更多空间。
// 这样的向量表现如何？它与链表相比如何？使用 valgrind 来帮助你发现错误。

// ./bin/vector-realloc
// 输出：0 1 2 3 4 5 6 7 8 9

// 表现：表现良好，能够动态调整大小以适应更多元素。
// 与链表相比：支持随机访问，内存使用更紧凑，但在扩展时可能涉及更多的内存复制操作。

// valgrind ./bin/vector-realloc
// ==14168== HEAP SUMMARY:
// ==14168==     in use at exit: 0 bytes in 0 blocks
// ==14168==   total heap usage: 5 allocs, 5 frees, 1,160 bytes allocated
// ==14168== 
// ==14168== All heap blocks were freed -- no leaks are possible
// ==14168== 
// ==14168== For lists of detected and suppressed errors, rerun with: -s
// ==14168== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
// 程序没有内存泄漏，表现良好。

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;

    size_t size;
    size_t capacity;
} Vector;

void append(Vector *vec, int value) {
    if (vec->size >= vec->capacity) { // 如果当前大小达到容量，则扩展容量
        vec->capacity *= 2; // 增加容量的 2 倍
        vec->data = realloc(vec->data, vec->capacity * sizeof(int));
    }

    vec->data[vec->size++] = value; // 添加新元素并增加大小
}

Vector *creat(size_t initial_capacity) {
    Vector *vec = malloc(sizeof(Vector)); // 分配 Vector 结构体的内存
    vec->data = malloc(initial_capacity * sizeof(int)); // 分配数据数组的内存
    
    vec->size = 0; // 初始化大小为 0
    vec->capacity = initial_capacity; // 设置初始容量

    return vec;
}

void Free(Vector *vec) {
    free(vec->data); // 释放数据数组的内存
    free(vec); // 释放 Vector 结构体的内存
}

void Print(Vector *vec) {
    for (size_t i = 0; i < vec->size; i++) {
        printf("%d ", vec->data[i]); // 打印每个元素
    }
    printf("\n");
}

int main(void) {
    Vector *vec = creat(4); // 创建一个初始容量为 4 的向量

    for (int i = 0; i < 10; i++) {
        append(vec, i); // 向向量中添加元素
    }

    Print(vec); // 打印向量中的所有元素

    Free(vec); // 释放向量的内存

    return 0;
}
