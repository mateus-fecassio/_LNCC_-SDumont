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
	double array[DIMENSION_X][DIMENSION_Y]; //global array
	double subarray[DIMENSION_X][DIMENSION_Y/size]; //local array

	// Dimensions of the array
	sizes[0] = DIMENSION_X;				// i (row)
	sizes[1] = DIMENSION_Y;				// j (column)

	// Dimensions of the subarray
	subsizes[0] = DIMENSION_X;			// i (row)
	subsizes[1] = DIMENSION_Y / size;		// j (column)

	// Where the subarray starts
	starts[0] = 0;					// i (row)
	starts[1] = myrank * subsizes[1];		// j (column)

	MPI_Datatype filetype;

	// Function to Create the datatype - HINT: verify how to create a subarray datatype
	// TODO

	// Function to Commit the datatype
	// TODO

	// Function to Set the file view - HINT: attention to the etype and datatype used. This time the it will write the entire buffer as a whole
	// TODO

	int count = 0;
	int i = 0;
	int j = 0;


	// Each process will fill its own local part of the global array with the value of myrank + (count++ / 100.0)
	//Remember to replace the [i-local-start], [i-local-end], [j-local-start] and [i-local-end] below with the corresponding correct values
	for (i = [i-local-start]; i < [i-local-end]; i++) {
		for (j = [j-local-start]; j < [j-local-start]; j++) {
			// TODO
		}
	}


	// Function to Write the whole part of the local subarray - HINT: use the collective implicit offset function, and remember how many items will be written per process
	// TODO

	// Free the created datatype
	// TOFO

	// Function to Close the file
	// TODO

	MPI_Finalize();

	return 0;
}
