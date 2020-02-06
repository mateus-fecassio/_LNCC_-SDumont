#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
int main()
{
    int i;
    int a[10000];
    for(i = 0; i < 10000; i++)
        a[i] = 0;

    #pragma omp parallel 
    {
             int id = omp_get_thread_num();
                for(i = 0; i < 1000; i++)
                    a[i] = a[i] + id;

             printf(" Thread = %d, a[1] = %d\n", id, a[1] );
             printf(" Depois Thread = %d, a[1] = %d\n", id, a[1] );

    }
    return 0;
}
