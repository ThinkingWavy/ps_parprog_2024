#define _POSIX_C_SOURCE 199506L

#include <omp.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define START_SEED 0

typedef uint32_t count_t;

int main(int argc, char** argv) {
	// read input arguments
	if(argc > 2) {
		fprintf(stderr, "Usage: %s [<thread_count>]\n", argv[0]);
		return EXIT_FAILURE;
	}

	// setting thread number and iterations
	const int32_t requested_threads = (argc <= 1) ? 1 : atoi(argv[1]);
	const count_t total_iterations = ((count_t)700) * 1000 * 1000;
	int num_threads = 0;

	// start time measurement
	double start = omp_get_wtime();

	count_t* points_in_circle = calloc(requested_threads, sizeof(count_t));
	count_t global_points = 0;

#pragma omp parallel num_threads(requested_threads)
	{
		num_threads = omp_get_num_threads();
		int thread_num = omp_get_thread_num();
		count_t* local_count = &points_in_circle[thread_num];
		unsigned int seed = START_SEED + omp_get_thread_num();

#pragma omp for
		for(count_t i = 0; i < total_iterations; ++i) {
			float x = (rand_r(&seed) / (float)RAND_MAX);
			float y = (rand_r(&seed) / (float)RAND_MAX);

			if(x * x + y * y <= 1.0f) {
				++(*local_count);
			}
		}
	}

	for(int i = 0; i < requested_threads; i++) {
		global_points += points_in_circle[i];
	}

	double pi_approximation = 4.0 * (global_points / (double)total_iterations);

	// print result and elapsed time
	double end = omp_get_wtime();
	double open_mp_elapsed_time = end - start;

	printf("Approximation of PI took %.3f seconds with %u threads - value: %.10f\n",
	       open_mp_elapsed_time, num_threads, pi_approximation);

	// Freeing
	free(points_in_circle);

	return EXIT_SUCCESS;
}
