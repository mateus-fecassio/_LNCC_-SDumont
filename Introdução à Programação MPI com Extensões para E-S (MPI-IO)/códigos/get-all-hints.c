#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int ret, size, myrank, nkeys, flag;
	char key[MPI_MAX_INFO_KEY], value[MPI_MAX_INFO_VAL];

	MPI_File fh;
	MPI_Status s;
	MPI_Request r;
	MPI_Info info_used;

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Open the file
	// TODO

	// Get the info object associated with the file handle
	// TODO

	if (myrank == 0) {
		// Get the number of defined keys from the info object used on the file handle
		// TODO

		printf("there are %d hints set:\n", nkeys);

		int i= 0;
		for (i = 0; i < nkeys; i++) {
			// Get the nth key to access its value
			// TODO

			// Get the key value
			// TODO

			printf("  %s: %s (%s)\n", key, value, flag ? "true" : "false");
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// Close the file
	// TODO

	MPI_Finalize();

	return 0;
}
