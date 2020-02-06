

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,id,i;

    n=1000;

    #pragma omp parallel  
    {
       #pragma omp for schedule (dynamic,1)
        for (i = 0; i < n; i++)
        { 
          id= omp_get_thread_num();
           i=i++;
        printf(" omp_get_thread_num = %d, id=%d \n",omp_get_thread_num(),id);
        }
    }
}
