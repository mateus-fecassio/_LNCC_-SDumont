#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "src/dot.h"

static int check (double r, int n){	
	return (r == (double)(n*2));
}

int main(int argc, char **argv){
	static char *awnser[] = { "bad", "ok" };
	
	long long int i, n;
	double start, end, elapsed, r;
	
	if(argc < 2){
		fprintf(stderr, "Usage: %s <number of elements>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	n = atoll(argv[1]);
	n *= 10e7;
	
	n = n % 8 == 0 ? n : n + (8 - n % 8);
	
	printf("number of elements: %lld x 10E7\n", (long long int) (n / 10e7));
	
	double *va = _mm_malloc(n * sizeof(double), 64);
	if(va == NULL){
		fprintf(stderr, "Out of memory!\n");
		exit(EXIT_FAILURE);
	}
	
	double *vb = _mm_malloc(n * sizeof(double), 64);
	if(vb == NULL){
		fprintf(stderr, "Out of memory!\n");
		exit(EXIT_FAILURE);
	}
	
	for (i=0; i<n; i++) {
		va[i] = 1;
		vb[i] = 2;
	}
		
	start = omp_get_wtime();
	r = dot_product(va, vb, n);
	end = omp_get_wtime();
	
	elapsed = end - start;
	
	printf("output is %s\ntotal time: %.3f seconds\n", awnser[check(r, n)], elapsed);
	
	_mm_free(va);
	_mm_free(vb);

	return 0;
}