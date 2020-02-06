#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int size, myrank;

	MPI_File fh;
	MPI_Status s;
	MPI_Request r;
	MPI_Info info_used;

	// Initializes the MPI
	MPI_Init(&argc, &argv);

	// Get the total number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &size);

  	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	//Open a file
	r= MPI_File_open(MPI_COMM_WORLD, "my-file.data", MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	// YOUR SOLUTION GOES HERE



	// Close the file
	MPI_File_close(&fh);

	// Finalizes the MPI
	MPI_Finalize();

	return 0;
}
