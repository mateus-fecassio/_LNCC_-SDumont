#pragma once

#define MAX_DEVICES 8
double laplace_solve(double *A, int ROWS, int COLS, int numDev) {
  int LDA=COLS+2;
  size_t bytes=(ROWS+2)*(LDA)*sizeof(double);  //Pad for BCs
  
  double *d_Ain[MAX_DEVICES];
  double *d_Aout[MAX_DEVICES];
  //allocate device memory
  for(int d=0;d<numDev;d++) {
    cudaSetDevice(d);
    cudaMalloc(&d_Ain[d],bytes);
    cudaMalloc(&d_Aout[d],bytes);
    cudaCheckError();
  }

  //copy initial conditions to both buffers
  for(int d=0;d<numDev;d++) {
    cudaSetDevice(d);
    cudaMemcpyAsync(d_Ain[d],A,bytes,cudaMemcpyHostToDevice,0);
    cudaMemcpyAsync(d_Aout[d],d_Ain[d],bytes,cudaMemcpyDeviceToDevice,0);
    cudaCheckError();
  }
  
  //start timer
  SYNC();
  double start=omp_get_wtime();
 
  dim3 blockSize(16,16);
  dim3 gridSize( ceil((double)(COLS)/blockSize.x), ceil((double)(ROWS)/blockSize.y));
  //perform iterative solve
  for(int i=0; i<nIters; i++)
  {
    for(int d=0;d<numDev;d++) {
      cudaSetDevice(d);
      simpleLaplaceIter_kernel<<<gridSize,blockSize>>>(ROWS,COLS,d_Ain[d],d_Aout[d]);
      cudaCheckError();
    }

    for(int d=0;d<numDev;d++) {
      std::swap(d_Ain[d],d_Aout[d]);
    }
  }
 
  //stop timer
  SYNC();
  double end=omp_get_wtime();
  
  //copy results back to host
  for(int d=0;d<numDev;d++) {
    cudaSetDevice(d);
    cudaMemcpyAsync(A,d_Ain[d],bytes,cudaMemcpyDeviceToHost,0);
    cudaCheckError();
  }
  
  //free device memory
  for(int d=0;d<numDev;d++) {
    cudaSetDevice(d);
    cudaFree(d_Ain[d]);
    cudaFree(d_Aout[d]);
    cudaCheckError();
  }

  return (end-start);
}

