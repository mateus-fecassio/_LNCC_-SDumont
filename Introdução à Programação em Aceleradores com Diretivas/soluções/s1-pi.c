#include <stdio.h>
#include <omp.h>
#define N 9999999999
int main(void) {
	double pi = 0.0f; long i;
	printf("Numero de processadores = %d\n", omp_get_max_threads());
	#pragma omp parallel for reduction(+: pi) /* OpenMP  */
	#pragma acc parallel loop reduction(+: pi) /* OpenACC */
	for (i=0; i<N; i++)
	{
		double t=(double) ((i+0.5)/N);
		pi += 4.0/(1.0+t*t);
	}
	printf("pi=%f\n",pi/N);
	return 0;
}
