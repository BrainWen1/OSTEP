
all: t0 t1

clean:
	rm -f ./bin/t0 ./bin/t1

t0: ./t0.c
	gcc -o ./bin/t0 ./t0.c -Wall -pthread
t1: ./t1.c
	gcc -o ./bin/t1 ./t1.c -Wall -pthread

.PHONY: all clean
