CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror -std=c17 -O3 -march=native -g -fopenmp

TARGETS=local_variable array padding

.PHONY: all clean

all: $(TARGETS)

clean:
	$(RM) $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
