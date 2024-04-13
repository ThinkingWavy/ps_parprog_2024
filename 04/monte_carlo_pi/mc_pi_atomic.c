#define _POSIX_C_SOURCE 199506L

#include <omp.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define START_SEED 0

typedef uint64_t timepoint_t;
typedef uint32_t count_t;

static timepoint_t time_ns();
static double elapsed_seconds(timepoint_t start, timepoint_t end);

int main(int argc, char** argv) {
	// read input arguments
	if(argc > 2) {
		fprintf(stderr, "Usage: %s [<thread_count>]\n", argv[0]);
		return EXIT_FAILURE;
	}

	const int32_t requested_threads = (argc <= 1) ? 1 : atoi(argv[1]);
	const count_t total_iterations = ((count_t)700) * 1000 * 1000;
	int num_threads = 0;

	// start time measurement
	timepoint_t start = time_ns();

	count_t points_in_circle = 0;
#pragma omp parallel
	{

		omp_set_num_threads(requested_threads);
		num_threads = omp_get_num_threads();
		unsigned int seed = START_SEED + omp_get_thread_num();

#pragma omp for
		for(count_t i = 0; i < total_iterations; ++i) {
			float x = (rand_r(&seed) / (float)RAND_MAX);
			float y = (rand_r(&seed) / (float)RAND_MAX);

			if(x * x + y * y <= 1.0f) {
#pragma omp atomic
				++points_in_circle;
			}
		}
	}
	double pi_approximation = 4.0 * (points_in_circle / (double)total_iterations);

	// print result and elapsed time
	timepoint_t end = time_ns();
	double elapsed_time = elapsed_seconds(start, end);

	printf("Approximation of PI took %.3f seconds with %u threads - value: %.10f\n", elapsed_time,
	       num_threads, pi_approximation);

	return EXIT_SUCCESS;
}

static timepoint_t time_ns() {
	// Note: We assume that all time information fits into an uint64_t
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return ((timepoint_t)now.tv_sec) * ((timepoint_t)1E9) + now.tv_nsec;
}

static double elapsed_seconds(timepoint_t start, timepoint_t end) {
	return ((start > end) ? start - end : end - start) / 1E9;
}
