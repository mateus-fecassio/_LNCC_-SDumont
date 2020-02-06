#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
int main()
{
    int i, n = 1000, id,flag = 1,data=0;
    int a[1000];
    id = omp_get_thread_num();
    #pragma omp parallel
    {
        #pragma omp sections 
        {
            #pragma omp section
            {
             id = omp_get_thread_num();
             printf("section 5 Thread = %d, data = %d\n", id, data);
            }
         
            #pragma omp section 
            {
                for(i = 0; i < 1000; i++)
                    a[i] = 0;

                 flag=0;
                 #pragma omp flush (flag)
                 id = omp_get_thread_num();
                 printf("Parallel 1 vetor A Thread = %d, flag = %d\n", id, flag);
            }


            #pragma omp section
            {
                 while (flag==1); 
                    id = omp_get_thread_num();
                    printf("section 2 vetor B Thread = %d, data = %d\n", id, data);
                for(i = 0; i < n; i++)
                    a[i]=i++;
                 data=1;
                 #pragma omp flush (data)
                    printf("section 2 vetor B Thread = %d, data = %d\n", id, data);
            }
            #pragma omp section
             {
             id = omp_get_thread_num();
             printf("section 3 Thread = %d, data = %d\n", id, data);
             }

            #pragma omp section
             {
                 while (data==0); 
             id = omp_get_thread_num();
             printf("section 4 Thread = %d, data = %d\n", id, data);
             }
        }
    }
        printf("codigo sequencial flag = %d, data= %d,\n", flag ,data);

    return 0;
}
