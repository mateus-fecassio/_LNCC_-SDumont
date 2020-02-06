#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <openacc.h>

int main(void)
{

	int n = 1024 * 1024 * 1024 / 2; float a = 2.0f;
	float *x = (float*) malloc(n * sizeof(float));
	float *y = (float*) malloc(n * sizeof(float));
	float *z = (float*) malloc(n * sizeof(float));

	long ngpu = acc_get_num_devices(acc_device_nvidia);

#pragma omp parallel num_threads(ngpu)
	{
		int tid = omp_get_thread_num();
		int inicio = n / ngpu * tid;
		int fim = n / ngpu * (tid + 1) - 1;
		acc_set_device_num(tid, acc_device_nvidia);
		printf("%d %d %d\n", tid, inicio, fim);

#pragma acc data create(x[0:n], y[0:n], z[0:n])
		{
#pragma acc kernels
#pragma acc loop independent
			for(int i=0; i<n; ++i) {
				x[i]=i;
				y[i]=5.0*i-1.0;
			}

#pragma acc kernels
#pragma acc loop independent
			for (int i = inicio; i < fim; ++i)
#pragma acc loop independent
				for (int j = 0; j < 512; ++j)
					z[i] = a*x[i] + y[i] * j;
		}
	}

	free(x);
	free(y);
	free(z);

	exit(0);

}
