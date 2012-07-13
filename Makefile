CC := clang
CFLAGS := -std=c99 -O3
WFLAGS := -Werror -Weverything -Wno-padded
OBJECTS := core.o compile.o disassemble.o

.PHONY : build clean check

build : $(OBJECTS)

clean :
	rm -f $(OBJECTS) test

check : test
	./$<

test : test.c $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $(WFLAGS) $^

%.o : %.c m0.h
	$(CC) -c -o $@ $(CFLAGS) $(WFLAGS) $<

core.o : CC := gcc
core.o : WFLAGS := -Werror -Wall -Wextra -Wno-uninitialized
