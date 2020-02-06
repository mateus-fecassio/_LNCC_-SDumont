#include <stdio.h>
#include <omp.h>

///////////////////////////////////////////////
//            PRODUTO ESCALAR
///////////////////////////////////////////////


//Funcao responsavel pelo preenchimento dos vetores
void InicializaVetores(int num,int *V1,int *V2)
{
   int i;
   for(i=0;i<num;i++)
    {
	  V1[i] = V2[i] = 1;
    }
}

int main (int argc, char * argv[])
{
	int *V1,*V2;
	int i,j,id;
	int num_threads;
	int par,num,dot,aux_dot;
        
        omp_lock_t  lock;
        omp_init_lock(&lock);
	
//Dimensao dos Vetores

	num = atoi(argv[1]);

//Alocacao dinamica dos vetores
	V1=(int*)malloc(num*sizeof(int));
	V2=(int*)malloc(num*sizeof(int));

//Inicializacao dos vetores
        InicializaVetores(num,V1,V2);
	
//Inicilializao da variavel dot  
	dot = 0;

	
	#pragma omp parallel shared(lock,dot)	
         {
	int aux_dot = 0;       
#pragma omp single
	   printf("numeros de threads = %d   id = %d\n",omp_get_num_threads(),omp_get_thread_num());
        #pragma omp for  
	   for(i=0;i<num;i++)
               {
		aux_dot += V1[i]*V2[i];
		}	
                omp_set_lock(&lock); 
                dot += aux_dot;
                omp_unset_lock(&lock);
               
        #pragma omp single
	   printf(" \n\n End Of Execution ::: dot = %d\n\n\n",dot);
         }
        omp_destroy_lock(&lock);
        return 0;
	
}
