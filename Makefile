CC := gcc
CFLAGS := -std=c99 -O3 -ggdb3
OBJECTS := core.o compile.o disassemble.o

.PHONY : build clean check

build : $(OBJECTS)

clean :
	rm -f $(OBJECTS) test

check : test
	./$<

test : test.c $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^

%.o : %.c m0.h
	$(CC) -c -o $@ $(CFLAGS) $<
