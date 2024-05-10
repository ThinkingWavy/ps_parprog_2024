#include <stdio.h>
#include <stdlib.h>
#define SIZE 1024

void copy(__restrict__ int* x, __restrict__ int* y) {
	for(int i = 0; i < SIZE; i++) {
		x[i] = y[i];
	}
}

int main(int argc, char** argv) {

	int a[SIZE];
	int b[SIZE];

	for(int i = 0; i < SIZE; ++i) {
		a[i] = argc;
	}

	copy(a, b);

	// output data to prevent compiler from removing any code
	for(int i = 0; i < SIZE; ++i) {
		printf("%d ", a[i]);
		printf("%d ", b[i]);
	}
	printf("\n");

	return 0;
}
