
all: syscall

clean:
	rm -f ./bin/syscall

syscall: syscall.c
	gcc -o ./bin/syscall syscall.c -Wall -std=c11

.PHONY: all clean syscall
