#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]) {
	int r, size, myrank, offset;
	double buffer[BUFFER_SIZE];

	MPI_File fh;
	MPI_Status s;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Function to Open the file - HINT: verify the access modes
	// TODO

	// Fill the buffer - HINT: Rebember, it's in double value!
	int i=0;
	for (i=0; i<BUFFER_SIZE; i++) {
		// TODO
	}

	// Set a displacement of the buffer size for each process based on its rank - HINT: Use the function to change the file view
	// TODO

	// Function to Write with individual file pointer the entire buffer at once
	// TODO

	// Function to Close the file
	// TODO

	MPI_Finalize();

	return 0;
}
