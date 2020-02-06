void matrix_mult(double *A, double *B, double *C, int N){
	int i, j, k;

	#pragma omp parallel for private(i, j, k)
	for(i = 0; i < N; i++)
		for(k = 0; k < N; k++){
			#pragma vector aligned
			#pragma omp simd
			for(j = 0; j < N; j++)
				C[i * N + j] += A[i * N + k] * B[k * N + j];
		}
}