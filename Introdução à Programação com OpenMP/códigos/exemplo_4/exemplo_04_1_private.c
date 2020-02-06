

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,m,id,i;

    n=10;
    m=10;
    #pragma omp parallel private(id,m) 
    {
       #pragma omp for 
        for (i = 0; i < n; i++)
        { 
          id= omp_get_thread_num();
        printf("Thread = %d, i= %d , n=%d\n",id,i,m);
        }
    }
}
