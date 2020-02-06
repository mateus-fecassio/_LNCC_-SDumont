

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n=10,id,i,aux;
    

    #pragma omp parallel  
    {
       #pragma omp for ordered
        for (i = 0; i < n; i++)
        { 
          #pragma omp ordered
          {
          id= omp_get_thread_num();
        printf("Thread = %d, i= %d\n",id,i);
          }
        }
    }
}
