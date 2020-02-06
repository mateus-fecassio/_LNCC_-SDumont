#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define DIMENSION_X 16
#define DIMENSION_Y 16

int main(int argc, char *argv[]) {
	int r, size, myrank, offset;

	MPI_File fh;
	MPI_Status s;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Function to Open the file - HINT: verify the access modes
	// TODO

	int sizes[2], subsizes[2], starts[2];
	double subarray[DIMENSION_X][DIMENSION_Y/size];

	// Dimensions of the array
	sizes[0] = DIMENSION_X;				// i (row)
	sizes[1] = DIMENSION_Y;				// j (column)

	// Dimensions of the subarray
	subsizes[0] = DIMENSION_X;			// i (row)
	subsizes[1] = DIMENSION_Y / size;	// j (column)

	// Where the subarray starts
	starts[0] = 0;						// i (row)
	starts[1] = subsizes[1] * myrank;	// j (column)

	MPI_Datatype filetype;

	// Function to Create the datatype - HINT: verify how to create a subarray datatype
	// TODO

	// Function to Commit the datatype
	// TODO

	// Function to Set the file view - HINT: attention to the etype and datatype used
	// TODO

	int count = 0;
	int i = 0;
	int j = 0;

	// Each process will fill its local array with the value of (myrank + (count / 100.0))
	for (i = 0; i < subsizes[0]; i++) {
		for (j = 0; j < subsizes[1]; j++) {
			// TODO
		}
	}

	// Function to Write the subarray - HINT: use the collective implicit offset function, and remember how many items will be written per process
	// TODO

	// Function to Close the file
	// TODO

	MPI_Finalize();

	return 0;
}
