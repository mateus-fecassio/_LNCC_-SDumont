#!/bin/bash
#SBATCH --nodes=1                      #Numero de Nós
#SBATCH --ntasks-per-node=1            #Numero de tarefas por Nó
#SBATCH --ntasks=1                     #Numero total de tarefas MPI
#SBATCH --cpus-per-task=24             #Numero de threads
#SBATCH -p treinamento                 #Fila (partition) a ser utilizada
#SBATCH -J semana-sdumont-openmp       #Nome job

NUM_THREAD=${1}
EXEC=${2}
ARG1=${3}

export OMP_NUM_THREADS=${NUM_THREAD}

srun -N 1 -c ${NUM_THREAD} ${EXEC} ${ARG1}
