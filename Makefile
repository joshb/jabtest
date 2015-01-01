CC=cc
CFLAGS=-Wall -ansi -pedantic
OBJS=jabtest.o jabtest_example.o

jabtest_example:	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o jabtest_example

clean:
	rm -f jabtest_example
	rm -f $(OBJS)

jabtest.o: jabtest.c
jabtest_example.o: jabtest_example.c
