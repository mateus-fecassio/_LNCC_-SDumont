#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "src/matrix.h"

int main(int argc, char **argv){
	char *awnser[] = { "bad", "ok" };
	
	int i, j, n, is_ok;
	double start, end, elapsed;
	
	if(argc < 2){
		fprintf(stderr, "Usage: %s <matrix size>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	n = atoi(argv[1]);

	n = n % 8 == 0 ? n : n + (8 - n % 8);
	
	printf("number of rows x cols: %d x %d\n", n, n);
	
	double *A = _mm_malloc(n * n * sizeof(double), 64);
	if(A == NULL){
		fprintf(stderr, "Out of memory!\n");
		exit(EXIT_FAILURE);
	}
	
	double *B = _mm_malloc(n * n * sizeof(double), 64);
	if(B == NULL){
		fprintf(stderr, "Out of memory!\n");
		exit(EXIT_FAILURE);
	}
	
	double *C = _mm_malloc(n * n * sizeof(double), 64);
	if(C == NULL){
		fprintf(stderr, "Out of memory!\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++){
			A[i * n + j] = 1;
			B[i * n + j] = 1;
			C[i * n + j] = 0;
		}
	
	start = omp_get_wtime();
	matrix_mult(A, B, C, n);
	end = omp_get_wtime();
	
	elapsed = end - start;

	is_ok = 1;	
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			if(C[i * n + j] != (double) n){
				is_ok = 0;
				i = n;
				j = n;
			}
	
	printf("multiply is %s\ntime: %.3f seconds\n", awnser[is_ok], elapsed);
	
	_mm_free(A);
	_mm_free(B);
	_mm_free(C);

	return 0;
}