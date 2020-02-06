

#include <stdio.h>
#include <omp.h>

int main()
{
    omp_set_nested(0);
    #pragma omp parallel
    {
       #pragma omp parallel
        printf("Hello World\n");
       #pragma omp single 
        printf("omp_get_nested=%d\n",omp_get_nested());
    }
}
