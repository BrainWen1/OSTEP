
all: test-precision tlb

clean:
	rm -f ./bin/test-precision ./bin/tlb

test-precision: test-precision.c
	gcc -o ./bin/test-precision test-precision.c -Wall -g

tlb: tlb.c
	gcc -o ./bin/tlb tlb.c -Wall -O0 -g

.PHONY: all clean test-precision tlb
