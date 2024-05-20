CFLAGS = -std=gnu11 -Wall -Wextra -lm -fopenmp -O1

.PHONY: all
all: serial_exc01 serial_vect

.PHONY: clean
clean:
	$(RM) -r serial_exc01 serial_vect test_output.txt ./results

serial_exc01: serial_exc01.c
	$(CC) $(CFLAGS) -o $@ $^

serial_vect: CFLAGS += -ftree-vectorize
serial_vect: serial_exc01.c
	$(CC) $(CFLAGS) -o $@ $^
