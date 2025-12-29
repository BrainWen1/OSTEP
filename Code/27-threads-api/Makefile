
all: threads lock

clean:
	rm -f ./bin/threads ./bin/lock

threads:
	gcc -o ./bin/threads ./threads.c -Wall -pthread

lock:
	gcc -o ./bin/lock ./lock.c -Wall -pthread

.PHONY: all clean threads lock
