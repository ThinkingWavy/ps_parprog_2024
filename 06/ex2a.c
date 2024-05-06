
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	int n = 1000;
	int* x = malloc(sizeof(*x) * n);
	int* y = malloc(sizeof(*y) * n);

	for(long i = 0; i < n; i++) {
		x[i] = rand();
		y[i] = rand();
	}

	int num_threads;
	double start_time = omp_get_wtime();
	double factor = 1;
#pragma omp parallel
	num_threads = omp_get_num_threads();
#pragma omp for
	for(int i = 0; i < n; i++) {
		x[i] = factor * y[i];
#pragma omp atomic
		factor = factor / 2;
	}
	double end_time = omp_get_wtime();

	printf("num_threads: %d, time in seconds: %2.2f\n", num_threads, end_time - start_time);
}