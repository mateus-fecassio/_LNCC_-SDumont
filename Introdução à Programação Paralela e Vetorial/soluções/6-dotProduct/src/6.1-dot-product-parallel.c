double dot_product(double *a, double *b, long long int N){
	long long int i = 0;
	double dot = 0;

	#pragma omp parallel for private(i) reduction(+ : dot)
	for(i = 0; i < N; i++)
		dot += a[i] * b[i];
			
	return dot;
}