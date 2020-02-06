#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>

void schedule(int *v, int N){
  int i = 0;

  for(i = 0; i < N; i++){
     v[i] = 0;
     sleep(1);
  }

}

int main(){
	int i, j, n;

	omp_set_num_threads(4);
	n = 24;
	
	printf("number of elements: %d\n", n);
	
	int *vector = (int *) calloc(n, sizeof(int));
	if(vector == NULL){
		fprintf(stderr, "Out of memory!\n");
		exit(EXIT_FAILURE);
	}

	schedule(vector, n);

	for(j = 0; j < 4; j++){
		printf("\nT%d: ", j);
		for(i = 0; i < n; i++)
			if(vector[i] == j)
				printf("%2d ", i);
	}
	printf("\n");
	
	return 0;
}