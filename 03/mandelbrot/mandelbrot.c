#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Include that allows to print result as an image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Default size of image
#define X 1280
#define Y 720
#define MAX_ITER 10000

void calc_mandelbrot(uint8_t image[Y][X]) {
	for(int y_pixel = 0; y_pixel < Y; y_pixel++) {
		for(int x_pixel = 0; x_pixel < X; x_pixel++) {
			double cx, cy, x, y, x_tmp;
			int iteration, norm_iteration;

			x = 0.0;
			y = 0.0;
			cx = (double)x_pixel / X * 3.5 - 2.5;
			cy = (double)y_pixel / Y * 2.0 - 1.0;
			iteration = 0;

			while(x * x + y * y <= 2 * 2 && iteration < MAX_ITER) {
				x_tmp = x * x - y * y + cx;
				y = 2 * x * y + cy;
				x = x_tmp;
				iteration++;
			}

			norm_iteration = (int)((double)iteration / MAX_ITER * 255);
			image[y_pixel][x_pixel] = norm_iteration;
		}
	}
}
int main() {
	uint8_t image[Y][X];

	calc_mandelbrot(image);

	const int channel_nr = 1, stride_bytes = 0;
	stbi_write_png("mandelbrot.png", X, Y, channel_nr, image, stride_bytes);
	return EXIT_SUCCESS;
}