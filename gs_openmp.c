#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define MAX_ITER 100

// Maximum value of the matrix element
#define MAX 100
#define TOL 0.000001




// Generate a random float number with the maximum value of max
float rand_float(int max) {
	return ((float)rand() / (float)(RAND_MAX)) * max;
}




// Calculates how many rows are given, as maximum, to each thread
int get_max_rows(int num_nodes, int n) {
	return (int)(ceil((n-2) / num_nodes) + 2);
}




// Allocate 2D matrix with random floats
void allocate_matrix(float **mat, int n, int m){

	*mat = (float *) malloc(n * m * sizeof(float));
	for (int i = 0; i < (n*m); i++) {
		(*mat)[i] = rand_float(MAX);
	}
}




// Write the time results into a CSV file
void write_to_file(int n, int num_blocks, int num_threads, float total_time, float exec_time) {

	FILE *f;
	char* file_name = "results.csv";

	if (access(file_name, F_OK) == -1) {
 		f = fopen(file_name, "a");
		fprintf(f, "Matrix size;Blocks;Threads per block;Total time;Operations time;\n");
	}
	else {
		f = fopen(file_name, "a");
	}

	fprintf(f, "%d;%d;%d;%f;%f;\n", n, num_blocks, num_threads, total_time, exec_time);
	fclose(f);
}




// Solves the matrix splitting the rows into different threads
void solver(float **mat, int n, int m) {

	float temp;
	float diff = 0;

	int done = 0
	int cnt_iter = 0;

	const int mat_dim = n*n;
	const int max_cell = (n*m) - (2*n);


  	while (!done && (cnt_iter < MAX_ITER)) {
  		diff = 0;

  		// Neither the first row nor the last row are solved
  		// (that's why it starts at "n" and it goes up to "num_elems - 2n")
		#pragma omp parallel for private(i, temp) reduction (+:diff) 				// TODO: Choose static | guided | dynamic 
  		for (int i = n; i < max_cell; i++) {

  			// Additionally, neither the first nor last column are solved
  			// (that's why the first and last positions of "rows" are skipped)
  			if ((i % n == 0) || (i+1 % n == 0)) {
				continue;
			}

  			const int pos_up = i - n;
  			const int pos_do = i + n;
  			const int pos_le = i - 1;
  			const int pos_ri = i + 1;

  			temp = (*mat)[i];
			(*mat)[i] = 0.2 * ((*mat)[i] + (*mat)[pos_le] + (*mat)[pos_up] + (*mat)[pos_ri] + (*mat)[pos_do]);
			diff += abs((*mat)[i] - temp);
      	}

		#pragma omp barrier															// TODO: Necessary?

		if (diff/mat_dim < TOL) {
			done = 1;
		}
		cnt_iter ++;
	}

	printf("Solver converged after %d iterations\n", cnt_iter);
}




int main(int argc, char *argv[]) {

	if (argc < 2) {
    	printf("Call this program with two parameters: matrix_size communication \n");
    	printf("\t matrix_size: Add 2 to a power of 2 (e.g. : 18, 1026)\n");				// TODO: Put more arguments
    	exit(1);
	}

	n = atoi(argv[1]);

	// Start recording the time
	clock_t i_total_t = clock();

	float *mat;
	alloc_matrix(&mat, n, n);

	// Calculate how many cells as maximum per thread
	int max_rows = get_max_rows(np, n);
	int max_cells = max_rows * n;
	

	// Initial operation time
	clock_t i_exec_t = clock();

	// Parallelized solver
	solver(&a, n, n);

	// Final operation time
	clock_t f_exec_t = clock();


	free(mat);

	// Finish recording the time
	clock_t f_total_t = clock();

	float total_time = (float)(f_total_t - i_total_t) / CLOCKS_PER_SEC;
	float exec_time = (float)(f_exec_t - i_exec_t) / CLOCKS_PER_SEC;
	printf("Total time: %f\n", total_time);
	printf("Operations time: %f\n", exec_time);

	write_to_file(n, num_blocks, threads_per_block, total_time, exec_time);			// TODO: Change arguments
}
