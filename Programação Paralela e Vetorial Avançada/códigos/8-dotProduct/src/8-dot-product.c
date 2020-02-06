double dot_product(double *a, double *b, long long int N){
	long long int i = 0;
	double dot = 0;

	for(i = 0; i < N; i++)
		dot += a[i] * b[i];
			
	return dot;
}