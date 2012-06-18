CC := gcc
CFLAGS := -std=c99 -O3
OBJECTS := core.o ops.o

.PHONY : build clean check

build : $(OBJECTS) test

clean :
	rm -f $(OBJECTS) test

check : test
	./$<

test : test.c $(OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^

%.o : %.c m0.h
	$(CC) -c -o $@ $(CFLAGS) $<
