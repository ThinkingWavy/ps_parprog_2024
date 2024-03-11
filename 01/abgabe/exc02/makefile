CFLAGS = -std=gnu11 -Wall -Werror -Wextra -fopenmp -O3

.PHONY: all
all: slow medium fast

.PHONY: clean
clean:
	$(RM) *.o slow medium fast

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -g -c -o $@

slow: slow.o
	$(CC) $(CFLAGS) -o $@ $^

medium: medium.o
	$(CC) $(CFLAGS) -o $@ $^

fast: fast.o
	$(CC) $(CFLAGS) -o $@ $^	
