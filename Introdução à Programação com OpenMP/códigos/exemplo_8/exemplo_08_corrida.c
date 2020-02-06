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
//Calculo do produto escalar
	#pragma omp parallel	
         {
        #pragma omp for 
	   for(i=0;i<num;i++)
		dot += V1[i]*V2[i];
         }
	   printf(" \n\n End Of Execution ::: dot = %d\n\n\n",dot);
        return 0;
	
}
