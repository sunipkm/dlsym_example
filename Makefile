CC = gcc
EDCFLAGS:= -Wall -O2 -Werror -fpic $(CFLAGS)
EDLDFLAGS:= -ldl $(LDFLAGS)

all: testprog testlib

testprog: main.o
	$(CC) main.o -o test.out $(EDLDFLAGS)

testlib: print.o
	$(CC) -shared -o libprint.so print.o

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -vf *.o
	rm -vf *.out
	rm -vf *.so
