
all: syscall context-switch

clean:
	rm -f ./bin/syscall ./bin/context-switch

syscall: syscall.c
	gcc -o ./bin/syscall syscall.c -Wall -std=c11

context-switch: context-switch.c
	gcc -o ./bin/context-switch context-switch.c -Wall -std=c11

.PHONY: all clean syscall context-switch
