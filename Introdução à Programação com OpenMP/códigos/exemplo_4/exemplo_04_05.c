

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,id,i;
    int var=0;
    n = 10;

    #pragma omp parallel 
    {
       #pragma omp for
        for (i = 0; i < n; i++)
        { 
          id= omp_get_thread_num();
          var=id;
        printf("Thread = %d, i= %d,var=%d \n",id,i,var);
        }

     }
       var=0;

    #pragma omp parallel  
     {
       #pragma omp for 
        for (i = 0; i < n; i++)
        {
          id= omp_get_thread_num();
          var=id;
        printf("loop 2 Thread = %d, i= %d,var=%d \n",id,i,var);
       }
          
    }
}
