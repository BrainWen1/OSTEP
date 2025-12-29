#include <pthread.h>
#include <stdio.h>
#include "my_threads.h"

typedef struct my_args {
    int a;
    int b;
} my_args;

typedef struct my_rets {
    int a;
    int b;
} my_rets;

void *thread_function(void *arg) {
    my_args *args = (my_args *)arg;
    printf("|--> %d %d\n", args->a, args->b);
    
    my_rets *rets = Malloc(sizeof(my_rets));
    rets->a = args->a + 1;
    rets->b = args->b + 1;
    return (void *)rets;
}

int main(void) {
    pthread_t thread;
    my_args args = {10, 20};

    Pthread_create(&thread, NULL, thread_function, &args);

    my_rets *rets;
    Pthread_join(thread, (void **)&rets);

    printf("Returned values: %d %d\n", rets->a, rets->b);
    free(rets);

    return 0;
}
