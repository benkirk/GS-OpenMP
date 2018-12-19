#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <omp.h>

#define MAX_ITER 100

// Maximum value of the matrix element
#define MAX 100
#define TOL 0.000001




// Generate a random float number with the maximum value of max
float rand_float(const int max) {
	return ((float)rand() / (float)(RAND_MAX)) * max;
}



// Allocate 2D matrix with random floats
void alloc_matrix(float **mat, const int n, const int m) {

	*mat = (float *) malloc(n * m * sizeof(float));

	const int size = n*m;
	for (int i = 0; i < size; i++) {
		(*mat)[i] = rand_float(MAX);
	}
}



// Solves the matrix splitting the rows into different threads
void solver(float **mat, const int n, const int m, const int num_ths, const int max_cells_per_th) {

	float diff;

	int done = 0;
	int cnt_iter = 0;
	const int mat_dim = n * n;

	while (!done && (cnt_iter < MAX_ITER)) {
		diff = 0;

		// Neither the first row nor the last row are solved
		// (that's why both 'i' and 'j' start at 1 and go up to '[nm]-1')
		#pragma omp parallel for num_threads(num_ths) schedule(static, max_cells_per_th) collapse(2) reduction(+:diff)
		for (int i = 1; i < n-1; i++) {
			for (int j = 1; j < m-1; j++) {

				const int pos = (i * m) + j;
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

		if (diff/mat_dim < TOL) {
			done = 1;
		}
		cnt_iter ++;
	}
}




int main(int argc, char *argv[]) {

	if (argc < 2) {
		exit(1);
	}

	const int n = atoi(argv[1]);


	float *mat;
	alloc_matrix(&mat, n, n);

	// Calculate how many cells as maximum per thread
	const int max_threads = omp_get_max_threads();
	const int max_rows = (int)(ceil((n-2) / max_threads) + 2);
	const int max_cells = max_rows * (n-2);


	// Parallelized solver
	solver(&mat, n, n, max_threads, max_cells);


	free(mat);

}
