CFLAGS=-Wno-deprecated-declarations -g

all: my_malloc.o main.o
	cc -o main $^ $(CFLAGS)

%.o: %.c
	cc -c $< -o $@ $(CFLAGS)

test: all
	./main

.PHONY: clean
clean:
	rm -f *.o main

