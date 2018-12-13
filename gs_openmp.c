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
void write_to_file(int n, char *schedule_type, float total_time, float exec_time) {

	FILE *f;
	char* file_name = "results.csv";

	if (access(file_name, F_OK) == -1) {
 		f = fopen(file_name, "a");
		fprintf(f, "Matrix size;Schedule type;Total time;Operations time;\n");
	}
	else {
		f = fopen(file_name, "a");
	}

	fprintf(f, "%d;%s;%f;%f;\n", n, schedule_type, total_time, exec_time);
	fclose(f);
}




// Solves the matrix splitting the rows into different threads
void solver(float **mat, int n, int m, int max_cells) {

	float temp;
	float diff = 0;

	int done = 0
	int cnt_iter = 0;
	int mat_dim = n * n;


	while (!done && (cnt_iter < MAX_ITER)) {
		diff = 0;

		// Neither the first row nor the last row are solved
		// (that's why both 'i' and 'j' start at 1 and go up to '[nm]-1')
		#pragma omp parallel for schedule(static, max_cells) collapse(2) reduction(+:diff) 
		for (int i = 1; i < n-1; i++) {
			for (int j = 1; j < m-1; j++) {

				const int pos = (i * m) + j;

				/*
				const int pos_up = pos - n;
				const int pos_do = pos + n;
				const int pos_le = pos - 1;
				const int pos_ri = pos + 1;
				*/

				const float temp = (*mat)[pos];

				(*mat)[pos] = 
					0.2 * (
						(*mat)[pos]
						+ (*mat)[pos - 1]
						+ (*mat)[pos - n]
						+ (*mat)[pos + 1]
						+ (*mat)[pos + n]
					);
				diff += abs((*mat)[pos] - temp);
			}
		}

		#pragma omp barrier	// TODO: Necessary?

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
		printf("\t matrix_size: Add 2 to a power of 2 (e.g. : 18, 1026)\n");
		exit(1);
	}

	n = atoi(argv[1]);

	// Start recording the time
	clock_t i_total_t = clock();

	float *mat;
	alloc_matrix(&mat, n, n);

	// Calculate how many cells as maximum per thread
	int max_rows = get_max_rows(np, n);
	int max_cells = max_rows * (n-2);
	

	// Initial operation time
	clock_t i_exec_t = clock();

	// Parallelized solver
	solver(&a, n, n, max_cells);

	// Final operation time
	clock_t f_exec_t = clock();


	free(mat);

	// Finish recording the time
	clock_t f_total_t = clock();

	float total_time = (float)(f_total_t - i_total_t) / CLOCKS_PER_SEC;
	float exec_time = (float)(f_exec_t - i_exec_t) / CLOCKS_PER_SEC;
	printf("Total time: %f\n", total_time);
	printf("Operations time: %f\n", exec_time);

	write_to_file(n, "static", total_time, exec_time);
}
