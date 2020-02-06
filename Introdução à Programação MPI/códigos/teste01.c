# include "mpi.h"
# include <stdio.h>

int main(int argc, char *argv[])
{
	int processId; 	/* RANK DOS PROCESSOS */
	int nProcesses;	/* NÚMERO DE PROCESSOS */
	int nameSize;	/* TAMANHO DO NOME */
	char computerName[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
	MPI_Get_processor_name(computerName, &nameSize);

	fprintf(stderr, "Ola gerado pelo processo %d, n maquina %s.\n", processId, computerName);

	MPI_Finalize();
	return 0;
}
