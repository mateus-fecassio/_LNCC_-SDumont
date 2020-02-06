#include <stdio.h>
#include <omp.h>

///////////////////////////////////////////////
//PRODUTO ESCALAR
///////////////////////////////////////////////


//Funcao responsavel pelo preenchimento dos vetores
void InicializaVetores(int num,int *V1,int *V2)
{
   int i;
   for(i=0;i<num;i++){
	  V1[i] = V2[i] = i;
    }

}

int main (int argc, char * argv[])
{
	int *V1,*V2, *V3;
	int i,j,id;
	int num_threads;
	int par,num,dot;
	double t1, t2;
	
	printf("\n\n======================================\n");
	printf("\tPRODUTO ESCALAR\n");
	printf("======================================\n\n");

	//Dimensao dos Vetores
	num = atoi(argv[1]);
       
	//Alocacao dinamica dos vetores
	V1=(int*)malloc(num*sizeof(int));
	V2=(int*)malloc(num*sizeof(int));
	V3=(int*)malloc(num*sizeof(int));
  	
        //Inicializacao dos vetores
        InicializaVetores(num,V1,V2);

	//Inicilializao da variavel dot
	dot = 0;

	t1 = omp_get_wtime();
	//Calculo do produto escalar
	#pragma omp parallel
	{
     #pragma omp single
	   printf("Numero de threads = %d   id = %d\n",omp_get_num_threads(),omp_get_thread_num());   
        
	   #pragma omp for 
	   for(i=0;i<num;i++)
		   V3[i] = V1[i]*V2[i];

     #pragma omp single
	   printf(" \n\n End Of Execution ::: dot = %d\n\n\n",dot);
	
	}
  t2 = omp_get_wtime();
  printf("Tempo do codigo paralelo: %f (s)\n", t2-t1);
  
	return 0;
	
}
