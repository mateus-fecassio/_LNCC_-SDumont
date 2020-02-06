#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int r, size, myrank, offset;
	char myrank_formatted;

	MPI_File fh;
	MPI_Status s;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Function to Open the file - HINT: verify the access modes
	// TODO

	int i=0;
	for (i=0; i<10; i++) {
		// Calculates the offset - HINT: offset = Position in the file expressed as a count of etypes
		// TODO

		// Define the character rank - HINT: Write it as character
		// TODO

		// Function to Write the character at the defined offset - HINT: remember the MPI predefined or derived etypes
		// TODO
	}

	// Function to Close the file
	// TODO

	MPI_Finalize();

	return 0;
}
