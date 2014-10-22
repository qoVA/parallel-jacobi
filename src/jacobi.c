#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "timer.h"
#include "results.h"
#include "jacobi.h"

/*
 * Accept as parameter:
 *  - input file
 *  - # of threads
 *  - algorithm
 *    - 0: serial
 *    - 1: pthread parallel
 *    - 2: OpenMP parallel
 *    - 3: MPI parallel
 *
 * 1. Read matrix from file
 * 2. Solve linear system through jacobi method
 * 3. Write output
 *
 * Output
 * 	Result: success or failure
 * 	Elapsed time
 */

int main(int argc, char *argv[]) {

	if ( argc != 5 ) {
		puts("Inform input file and number of threads!");
		exit(1);
	}

	//# of threads
	int thread_count = strtol(argv[2], NULL, 10);
	//algorithm
	int algorithm = strtol(argv[3], NULL, 10);
	//console output level
	int verbose = strtol(argv[4], NULL, 10);

	if (verbose) puts("---BEGIN---");

	//prints info
	if (verbose)
		printf("Input file: '%s', thread count: %i, algorithm: %i\n\n", argv[1], thread_count, algorithm);

	//loads matrix
	matrix *m = matrix_load(argv[1]);

	//prints matrix
	if (verbose)
		matrix_print(m);

	//starts timer
	timer* t = start_timer();

	jacobi_result* result;

	switch (algorithm) {
		case 1: // pthread parallel
			result = jacobi_parallel_pthread_better(m, thread_count, verbose);
			break;
		case 2: // OpenMP parallel
			result = jacobi_parallel_omp(m, thread_count, verbose);
			break;
		case 3: // MPI parallel
			break;
		case 0: // serial
		default:
			result = jacobi_serial(m, verbose);
	}
	//Sleep(500);

	//stops timer
	stop_timer(t, verbose);

	//prints result
	if (verbose) {
		int i;
		printf("\nResults: ");
		for (i = 0; i < m->size; i++) {
			printf("%f, ", result->x[i]);
		}
		printf("\nIterations: %i ", result->k);
	}

	//saves results
	write_results(t, argv[1], thread_count, algorithm, m->size);

	//frees matrix
	matrix_destroy(m);

	//frees timer
	free(t);

	//frees result
	free(result->x);
	free(result);

	if (verbose) puts("\n\n---END---");
	return EXIT_SUCCESS;
}
