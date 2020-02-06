#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <algorithm>
#include <omp.h>

const int nIters = 1000;

//Macro for checking cuda errors following a cuda launch or api call
#define cudaCheckError() {                                          \
 cudaError_t e=cudaGetLastError();                                 \
 if(e!=cudaSuccess) {                                              \
   printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));           \
   exit(0); \
 }                                                                 \
}

#define SYNC() \
  for(int d=0;d<numDev;d++) {                                         \
    cudaSetDevice(d);                                                 \
    cudaDeviceSynchronize();                                          \
    cudaCheckError();                                                 \
  }                                                                   \
  cudaSetDevice(0);


void time_solve(int ROWS, int COLS, int numDev);

#define IDX(row,col,LDA) ( (row)*(LDA)+(col))
__global__ void simpleLaplaceIter_kernel(int ROWS, int COLS, double * Ain, double * Aout)
{
  int lda=COLS+2;
  int col=blockIdx.x*blockDim.x+threadIdx.x+1; //Pad to skip BC
  int row=blockIdx.y*blockDim.y+threadIdx.y+1; //Pad to skip BC

  if(row<=ROWS && col<=COLS) { //Ensure i and j are not BCs
    Aout[IDX(row,col,lda)]= .25 * 
      (  
       + Ain[IDX(row  ,col-1,lda)] 
       + Ain[IDX(row  ,col+1,lda)] 
       + Ain[IDX(row-1,col  ,lda)] 
       + Ain[IDX(row+1,col  ,lda)] 
      );
  }
}

int main(int argc, char** argv)
{
  printf("OMP Threads: %d\n", omp_get_max_threads()); 
  int numDev; cudaGetDeviceCount(&numDev); printf("Num GPUs: %d\n", numDev);

  for(int i=0;i<numDev;i++) {
    cudaSetDevice(i);
    for(int j=0;j<numDev;j++) {
      if(i!=j) {
        int access;
        cudaDeviceCanAccessPeer(&access,i,j);
        if(access) {
          cudaDeviceEnablePeerAccess(j,0);
          //printf("Peer Access %d -> %d\n",i,j);
          cudaCheckError();
        }
      }
    }
  }

  time_solve(2048, 2048, numDev);
  time_solve(4096, 4096, numDev);
  //time_solve(8192, 8192, numDev);

  printf("done\n");
  return 0;
}

double laplace_solve_reference(double *A, int ROWS, int COLS) {
  int LDA=COLS+2;
  size_t bytes=(ROWS+2)*(LDA)*sizeof(double);  //Pad for BCs
  int numDev=1;
  cudaSetDevice(0);
  
  double *d_Ain;
  double *d_Aout;
  //allocate device memory
  cudaMalloc(&d_Ain,bytes);
  cudaMalloc(&d_Aout,bytes);
  cudaCheckError();

  //copy initial conditions to both buffers
  cudaMemcpy(d_Ain,A,bytes,cudaMemcpyHostToDevice);
  cudaMemcpy(d_Aout,d_Ain,bytes,cudaMemcpyDeviceToDevice);
  cudaCheckError();
  
  //start timer
  SYNC();
  double start=omp_get_wtime();
  
  dim3 blockSize(16,16);
  dim3 gridSize( ceil((double)(COLS)/blockSize.x), ceil((double)(ROWS)/blockSize.y));
  //perform iterative solve
  for(int i=0; i<nIters; i++)
  {
    simpleLaplaceIter_kernel<<<gridSize,blockSize>>>(ROWS,COLS,d_Ain,d_Aout);
    cudaCheckError();
    std::swap(d_Ain,d_Aout);
  }
 
  //stop timer
  SYNC();
  double end=omp_get_wtime();
  
  //copy results back to host
  cudaMemcpy(A,d_Ain,bytes,cudaMemcpyDeviceToHost);
  cudaCheckError();
  
  //free device memory
  cudaFree(d_Ain);
  cudaFree(d_Aout);
  cudaCheckError();

  return (end-start);
}

#include "laplace.h" 

void time_solve(int ROWS, int COLS, int numDev) {
  size_t bytes=(ROWS+2)*(COLS+2)*sizeof(double);  //Pad for BCs
  double *A, *Aserial;
  //Allocate pinned host memory
  cudaMallocHost(&A,bytes);
  cudaMallocHost(&Aserial,bytes);
  cudaCheckError();

  int LDA=COLS+2;
  //set initial conditions
  for(int i=0;i<ROWS+2;i++)
    for(int j=0;j<COLS+2;j++)
      Aserial[IDX(i,j,LDA)]=A[IDX(i,j,LDA)]=(i==0 || j==0 || i==ROWS+1 || j==COLS+1) ? 1 : 0;

  double time_s;
  double time_p;
  //solve in serial
  time_s=laplace_solve_reference(Aserial,ROWS,COLS);
  //solve in parallel
  time_p=laplace_solve(A,ROWS,COLS,numDev);

#if 0
  printf("Serial:\n");
  for(int i=0;i<ROWS+2;i++) {
    for(int j=0;j<COLS+2;j++) {
      printf("%6.2lg ",Aserial[IDX(i,j,LDA)]);
    }
    printf("\n");
  }
  printf("\n");
  printf("\n");
  printf("Parallel:\n");
  for(int i=0;i<ROWS+2;i++) {
    for(int j=0;j<COLS+2;j++) {
      printf("%6.2lg ",A[IDX(i,j,LDA)]);
    }
    printf("\n");
  }
  printf("\n");
  printf("\n");
#endif

  //verify answers are identical
  for(int i=0;i<ROWS+2;i++){
    for(int j=0;j<COLS+2;j++){
      if(Aserial[IDX(i,j,LDA)]!=A[IDX(i,j,LDA)]) {
        printf("%dx%d: Error solutions do not match at i: %d, j: %d\n", ROWS,COLS,i,j);
        goto cleanup;
      }
    }
  }
  

cleanup:
  printf("%dx%d: solve time, 1 GPU: %lg s, %d GPUs: %lg s, speedup: %lg, efficiency: %lg%%\n", ROWS, COLS, time_s, numDev, time_p, time_s/time_p, time_s/time_p/numDev*100);
  //free host memory
  cudaFreeHost(A);
  cudaFreeHost(Aserial);
  cudaCheckError();
}
