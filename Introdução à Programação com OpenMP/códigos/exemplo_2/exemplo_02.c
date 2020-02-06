

#include <stdio.h>
#include <omp.h>

int main()
{
    omp_set_nested(1);
    #pragma omp parallel
    {
       #pragma omp parallel
        printf("Hello World\n");
    }
}
