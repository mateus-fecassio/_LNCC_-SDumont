#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main()
{
    int i,  n = 10;
    int a[100], b[100], c[100];


    for(i = 0; i < n; i++)
    {
        a[i] = 0;
        b[i] = 0;
        c[i] = 0;
    }
    
    #pragma omp parallel 
    {

                  int  id = omp_get_thread_num();
      int j=0;

        #pragma omp sections nowait
        {
            #pragma omp section
            {
                for(i = 0; i < n; i++)
                {
                    a[i] = a[i] + 1;
                    id = omp_get_thread_num();
                    printf("vetor A Thread = %d, I = %d\n", id, i);
                }
            }

            #pragma omp section
            {
                for(j = 0; j < n;j++)
                {
                    b[j] = b[j] + 1;
                    id = omp_get_thread_num();
                    printf("vetor B Thread = %d, I = %d\n", id, j);
                }
            }
           }
            #pragma omp single
                    printf("vetor C Thread = %d\n", omp_get_thread_num());

        }


    return 0;
}
