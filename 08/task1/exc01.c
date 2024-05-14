#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if(argc != 3) {
		printf("Usage %s <num_iterations> <num_threads>", argv[0]);
	}

	const int num_threads = atoi(argv[2]);
	const size_t num_iterations = atol(argv[1]);
	size_t result = 0;
	omp_set_num_threads(num_threads);

	double start_time = omp_get_wtime();

#pragma omp parallel for proc_bind(spread)
	for(size_t i = 0; i < num_iterations; i++) {
#pragma omp atomic
		result += 1;
	}

	double end_time = omp_get_wtime();
	printf("num_threads: %d, res: %lu, time in seconds: %2.2f\n", num_threads, result,
	       end_time - start_time);
}