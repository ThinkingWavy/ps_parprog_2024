#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_REP 1000000

int main(int argc, char** argv) {

	if(argc != 2) {
		printf("Usage: %s <size>\n", argv[0]);
		return 1;
	}

	const size_t n = atoi(argv[1]);

	// Allocate memory for matrices
	float* a = (float*)calloc(n, sizeof(float));
	float* b = (float*)malloc(n * sizeof(float));
	float* c = (float*)malloc(n * sizeof(float));

	if(a == NULL || b == NULL || c == NULL) {
		printf("Memory allocation failed\n");
		return 1;
	}

	for(size_t i = 0; i < n; ++i) {
		b[i] = 1.00f + (i * 2);
		c[i] = 2.00f + i;
	}

	double start_time = omp_get_wtime();

	int threads = omp_get_num_threads();
	for(size_t run = 0; run < MAX_REP; ++run) {
		for(size_t i = 0; i < n; ++i) {
			a[i] += b[i] * c[i];
		}
	}

	double end_time = omp_get_wtime();

	printf("group: %ld, num_threads: %d, time: %.4f\n", n, threads, end_time - start_time);

	FILE* fptr = fopen("test_output.txt", "a");
	if(fptr == NULL) {
		printf("Error opening file!\n");
		free(a);
		free(b);
		free(c);
		return 1;
	}

	// Write the first 10 elements of the result to the file
	fprintf(fptr, "Size of Elements: %ld\n", n);
	for(int i = 0; i < 10; ++i) {
		fprintf(fptr, "a[%d] = %f\n", i, a[i]);
	}

	// Close the file
	fclose(fptr);

	free(a);
	free(b);
	free(c);

	return 0;
}