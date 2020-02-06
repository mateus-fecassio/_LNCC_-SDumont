

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,m,id,i;

    n=10;
    #pragma omp parallel 
    {
       #pragma omp for  lastprivate(i)
        for (i = 0; i < n; i++)
        { 
          id= omp_get_thread_num();
        printf("Thread = %d, i=  %d\n",id,i);
        }
        printf("Thread = %d, i=  %d\n",id,i);
    }
}
