

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,i;
    static int var=0;
    static int id=0;
    #pragma omp threadprivate(var,id)    
    n = atoi(argv[1]);
    omp_set_nested(1);

    #pragma omp parallel if (n > 10)
    {
       #pragma omp for
        for (i = 0; i < n; i++)
        { 
          id= omp_get_thread_num();
          var=var+id;
        printf("Thread = %d, i= %d,var=%d \n",id,i,var);
        }
    }
        printf("codigo serial Thread = %d, i= %d,var=%d \n",id,i,var);
    #pragma omp parallel copyin(var) 
    {
       #pragma omp for
        for (i = 0; i < n; i++)
        {
           var=var+id;
          id= omp_get_thread_num();
        printf("loop 2 Thread = %d, i= %d,var=%d \n",id,i,var);
       }
          
    }
}
