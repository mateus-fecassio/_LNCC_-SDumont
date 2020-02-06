

#include <stdio.h>
#include <omp.h>

int main()
{
        printf("omp_in_parallel=%d\n",omp_in_parallel());
    #pragma omp parallel
    {
        printf("Hello World\n");
     #pragma omp single
        printf("omp_in_parallel=%d\n",omp_in_parallel());
    }
}
