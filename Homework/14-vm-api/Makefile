
all: hello null not-free data-100 data-free-print free-mid-ptr vector-realloc

clean:
	rm -f ./bin/hello ./bin/null ./bin/not-free ./bin/data-100 ./bin/data-free-print ./bin/free-mid-ptr ./bin/vector-realloc

hello:
	gcc -o ./bin/hello hello.c -g

null:
	gcc -o ./bin/null null.c -g

not-free:
	gcc -o ./bin/not-free not-free.c -g

data-100:
	gcc -o ./bin/data-100 data-100.c -g

data-free-print:
	gcc -o ./bin/data-free-print data-free-print.c -g

free-mid-ptr:
	gcc -o ./bin/free-mid-ptr free-mid-ptr.c -g

vector-realloc:
	gcc -o ./bin/vector-realloc vector-realloc.c -g

.PHONY: all clean hello null not-free data-100 data-free-print free-mid-ptr