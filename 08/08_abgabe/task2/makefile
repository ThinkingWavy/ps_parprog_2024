CFLAGS = -std=gnu11 -Wall -Wextra -lm -fopenmp -O3

.PHONY: all
all: hadamard_ser hadamard_par hadamard_static

.PHONY: clean
clean:
	$(RM) hadamard_par hadamard_ser hadamard_static

hadamard_par: hadamard_par.c

hadamard_ser: hadamard_ser.c

hadamard_static: hadamard_static.c
