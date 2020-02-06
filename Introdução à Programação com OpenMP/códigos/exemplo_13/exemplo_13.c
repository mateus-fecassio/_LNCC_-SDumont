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
	int par,num,dot;
        double start,end;
	
	printf("\n\n======================================\n");
	printf("\tPRODUTO ESCALAR\n");
	printf("======================================\n\n");
	
//Dimensao dos Vetores

	num = atoi(argv[1]);

//Alocacao dinamica dos vetores
	V1=(int*)malloc(num*sizeof(int));
	V2=(int*)malloc(num*sizeof(int));

//Inicializacao dos vetores
        InicializaVetores(num,V1,V2);
	
//Inicilializao da variavel dot  
	dot = 0;
	start = omp_get_wtime();
//Calculo do produto escalar
	#pragma omp parallel	
         {
        #pragma omp single
	   printf("numero de threads = %d   id = %d\n",omp_get_num_threads(),omp_get_thread_num());
        #pragma omp for reduction(+:dot)
	   for(i=0;i<num;i++)
		dot += V1[i]*V2[i];
        #pragma omp single
	   printf(" \n\n End Of Execution ::: dot = %d\n\n\n",dot);
         }
	end = omp_get_wtime();
	   printf(" \n\n Tempo decorrido ::: tempo = %lf\n\n\n",end-start);
        return 0;
	
}
