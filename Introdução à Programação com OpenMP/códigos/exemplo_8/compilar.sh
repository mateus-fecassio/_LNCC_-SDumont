#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_08.c -o exemplo_08
gcc -fopenmp exemplo_08_com_barrier.c -o exemplo_08_com_barrier
gcc -fopenmp exemplo_08_corrida.c -o exemplo_08_corrida
gcc -fopenmp exemplo_08_sem_barrier.c -o exemplo_08_sem_barrier

echo '*** COMPILACAO COMPLETADA ***'

