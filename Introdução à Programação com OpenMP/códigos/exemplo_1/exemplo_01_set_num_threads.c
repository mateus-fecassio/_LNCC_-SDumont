

#include <stdio.h>
#include <omp.h>

int main()
{
    
        omp_set_num_threads(8);

    #pragma omp parallel num_threads(4) 
    {
        printf("Hello World\n");
     #pragma omp single
     {
       printf(" Numero de threads= %d\n", omp_get_num_threads());
       printf(" Numero max de threads= %d\n", omp_get_max_threads());
     }
    }
}
