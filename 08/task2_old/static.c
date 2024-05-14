#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000

// Function to compute the Hadamard product of two matrices in parallel
int hadamard_product_parallel(double** a, double** b, double** c, size_t n) {
	int threads;
#pragma omp parallel
	{
		threads = omp_get_num_threads();
#pragma omp for collapse(2) schedule(static)
		for(size_t i = 0; i < n; ++i) {
			for(size_t j = 0; j < n; ++j) {
				c[i][j] = a[i][j] * b[i][j];
			}
		}
	}
	return threads;
}

// Function to print a matrix
void print_matrix(double** mat, size_t n) {
	for(size_t i = 0; i < n; ++i) {
		for(size_t j = 0; j < n; ++j) {
			printf("%.2f\t", mat[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char** argv) {
	// Size of the matrices
	const size_t n = atoi(argv[1]);

	// Allocate memory for matrices
	double** a = (double**)malloc(n * sizeof(double*));
	double** b = (double**)malloc(n * sizeof(double*));
	double** c = (double**)malloc(n * sizeof(double*));
	for(size_t i = 0; i < n; ++i) {
		a[i] = (double*)malloc(n * sizeof(double));
		b[i] = (double*)malloc(n * sizeof(double));
		c[i] = (double*)malloc(n * sizeof(double));
	}

	// Initialize matrices a and b
	for(size_t i = 0; i < n; ++i) {
		for(size_t j = 0; j < n; ++j) {
			a[i][j] = i + j;
			b[i][j] = i * 10 + j;
		}
	}

	double start_time = omp_get_wtime(); // Start timing

	// Compute Hadamard product of matrices a and b in parallel
	int threads = hadamard_product_parallel(a, b, c, n);

	double end_time = omp_get_wtime(); // End timing

	printf("num_threads: %d, res:0  time:%.4f\n", threads, end_time - start_time);
	// print_matrix(c, n);  // Printing large matrices can be overwhelming

	// Free dynamically allocated memory
	for(size_t i = 0; i < n; ++i) {
		free(a[i]);
		free(b[i]);
		free(c[i]);
	}
	free(a);
	free(b);
	free(c);

	return 0;
}
