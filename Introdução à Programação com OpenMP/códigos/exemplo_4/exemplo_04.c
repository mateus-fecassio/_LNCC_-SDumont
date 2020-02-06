

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,id,i;

    n=30;

    #pragma omp parallel 
    {
       #pragma omp for schedule (runtime)
        for (i = 0; i < n; i++)
        { 
          id= omp_get_thread_num();
        printf("Thread = %d, i= %d\n",id,i);
        }
    }
}
