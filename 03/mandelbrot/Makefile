CFLAGS = -std=gnu11 -Wall -Wextra -lm -g 

.PHONY: all
all: mandelbrot

.PHONY: clean
clean:
	$(RM) mandelbrot results/mandelbrot.out results/mandelbrot.png
	rm -r results 

mandelbrot: mandelbrot.c
