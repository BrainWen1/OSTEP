
all: threads lock cond-var

clean:
	rm -f ./bin/threads ./bin/lock ./bin/cond-var

threads:
	gcc -o ./bin/threads ./threads.c -Wall -pthread

lock:
	gcc -o ./bin/lock ./lock.c -Wall -pthread

cond-var:
	gcc -o ./bin/cond-var ./cond-var.c -Wall -pthread

.PHONY: all clean threads lock cond-var
