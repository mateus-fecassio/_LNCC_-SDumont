

#include <stdio.h>
#include <omp.h>

int main (int argc, char * argv[])
{
    int n,id,i,j;

    n=1000000000;

    #pragma omp parallel 
    {
       #pragma omp for schedule (runtime)
        for (i = 0; i < n; i++)
        { 
          j=i/2;
        }
    }
}
