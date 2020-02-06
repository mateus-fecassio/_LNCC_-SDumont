

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,id,i;
    static int var=0;
    #pragma omp threadprivate(var)    
    n = atoi(argv[1]);
    omp_set_nested(1);

    #pragma omp parallel 
    {
       #pragma omp for
        for (i = 0; i < n; i++)
        { 
          id= omp_get_thread_num();
          var=id*10;
        printf("Thread = %d, i= %d,var=%d \n",id,i,var);
        }

       #pragma omp master
        {
          id= omp_get_thread_num();
        printf("Master Thread = %d, i= %d,var=%d \n",id,i,var);
        }

       #pragma omp single copyprivate(var)
          {
          id= omp_get_thread_num();
        printf("Single Thread = %d, i= %d,var=%d \n",id,i,var);
           }
    }


    #pragma omp parallel if (n > 10) 
     {
       #pragma omp for 
        for (i = 0; i < n; i++)
        {
           var=var+3;
          id= omp_get_thread_num();
        printf("loop 2 Thread = %d, i= %d,var=%d \n",id,i,var);
       }
          
    }
}
