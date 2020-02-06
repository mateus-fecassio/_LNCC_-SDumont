#pragma once

#define MAX_DEVICES 8
double laplace_solve(double *A, int ROWS, int COLS, int numDev) {
  int LDA=COLS+2;
  int rows=ROWS/numDev;
  int bytes=(rows+2)*LDA*sizeof(double);
  int int_size = rows * LDA;
  int bc_size = LDA;
  
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
    cudaMemcpyAsync(d_Ain[d],A+IDX(d*rows,0,LDA),bytes,cudaMemcpyHostToDevice,0);
    cudaMemcpyAsync(d_Aout[d],d_Ain[d],bytes,cudaMemcpyDeviceToDevice,0);
    cudaCheckError();
  }
  
  //start timer
  SYNC();
  double start=omp_get_wtime();
 
  dim3 blockSize(16,16);
  dim3 gridSize( ceil((double)(COLS)/blockSize.x), ceil((double)(rows)/blockSize.y));
  //perform iterative solve
  for(int i=0; i<nIters; i++)
  {
    for(int d=0;d<numDev;d++) {
      cudaSetDevice(d);
      simpleLaplaceIter_kernel<<<gridSize,blockSize>>>(rows,COLS,d_Ain[d],d_Aout[d]);
      cudaCheckError();
    }

    for(int d=0;d<numDev;d++) {
      cudaSetDevice(d);
      //Grab lower bc
      if(d>0)  cudaMemcpyPeerAsync(d_Aout[d],d,d_Aout[d-1]+IDX(rows,0,LDA),d-1,bc_size*sizeof(double),0);
      //Grab upper bc
      if(d<numDev-1) cudaMemcpyPeerAsync(d_Aout[d]+IDX(rows+1,0,LDA),d,d_Aout[d+1]+IDX(1,0,LDA),d+1,bc_size*sizeof(double),0);
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
    cudaMemcpyAsync(A+IDX(d*rows+1,0,LDA),d_Ain[d]+IDX(1,0,LDA),int_size*sizeof(double),cudaMemcpyDeviceToHost,0);
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

