#include <errno.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define PERROR fprintf(stderr, "%s:%d: error: %s\n", __FILE__, __LINE__, strerror(errno))
#define PERROR_GOTO(label) \
	do { \
		PERROR; \
		goto label; \
	} while(0)

#define INIT_ARRAY(arr, label) \
	do { \
		if(!(arr)) PERROR_GOTO(label); \
		for(long i = 0; i < n; ++i) { \
			(arr)[i] = malloc(sizeof(**(arr)) * n); \
			if(!(arr)[i]) PERROR_GOTO(label); \
		} \
	} while(0)

void free_2d_array(int** arr, long len) {
	if(!arr) {
		return;
	}
	for(long i = 0; i < len; ++i) {
		if(!arr[i]) {
			break;
		}
		free(arr[i]);
	}
	free(arr);
}

int main(int argc, char** argv) {
	// handle input
	if(argc != 3) {
		fprintf(stderr, "Error: usage: %s <n> <number threads>\n", argv[0]);
		return EXIT_FAILURE;
	}

	omp_set_num_threads(atoi(argv[2]));
	errno = 0;
	char* str = argv[1];
	char* endptr;
	long n = strtol(str, &endptr, 0);
	if(errno != 0) {
		perror("strtol");
		return EXIT_FAILURE;
	}
	if(endptr == str) {
		fprintf(stderr, "Error: no digits were found!\n");
		return EXIT_FAILURE;
	}
	if(n < 0) {
		fprintf(stderr, "Error: matrix size must not be negative!\n");
		return EXIT_FAILURE;
	}

	// allocate memory
	int status = EXIT_FAILURE;
	int** a = malloc(sizeof(*a) * n);
	INIT_ARRAY(a, error_a);
	int** b = malloc(sizeof(*b) * n);
	INIT_ARRAY(b, error_b);
	// ERROR: NOT INITIALIZED
	int** c = malloc(sizeof(*c) * n);
	INIT_ARRAY(c, error_c);
	unsigned* local_res = malloc(omp_get_max_threads() * sizeof(*local_res));
	if(!local_res) PERROR_GOTO(error_c);
	status = EXIT_SUCCESS;

	// fill matrix
	srand(time(NULL));
	for(long i = 0; i < n; ++i) {
		for(long j = 0; j < n; ++j) {
			a[i][j] = rand();
			b[i][j] = rand();
		}
	}

	int num_threads = 0;
	double start_time = omp_get_wtime();
#pragma omp parallel default(none) shared(n, a, b, c, local_res, num_threads)
	{
		num_threads = omp_get_num_threads();
		// matrix multiplication
#pragma omp parallel for default(none) shared(n, a, b, c)

		for(long i = 0; i < n; ++i) {
			for(long j = 0; j < n; ++j) {
				for(long k = 0; k < n; ++k) {
					// ERROR: TENTS TO OVERFLOW
					c[i][j] += a[i][k] * b[k][j];
				}
			}
		}

		// sum of matrix c
#pragma omp parallel for default(none) shared(n, a, b, c, local_res)
		for(long i = 0; i < n; ++i) {
			for(long j = 0; j < n; ++j) {
				// TO IMPROVE
				local_res[omp_get_thread_num()] += c[i][j];
			}
		}
	}
	unsigned long res = 0;
	// ERROR: NOT DEFINED OMP_GET_NUM_THREADS
	for(int l = 0; l < omp_get_num_threads(); ++l) {
		res += local_res[l];
	}
	double end_time = omp_get_wtime();
	printf("num_threads: %d, res: %lu, time: %2.2f seconds\n", num_threads, res,
	       end_time - start_time);

	// cleanup
	free(local_res);
error_c:
	free_2d_array(c, n);
error_b:
	free_2d_array(b, n);
error_a:
	free_2d_array(a, n);
	return status;
}