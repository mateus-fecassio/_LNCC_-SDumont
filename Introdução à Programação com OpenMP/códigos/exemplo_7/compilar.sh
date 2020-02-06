#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_07_atomic.c -o exemplo_07_atomic
gcc -fopenmp exemplo_07.c -o exemplo_07
gcc -fopenmp exemplo_07_corrida.c -o exemplo_07_corrida

echo '*** COMPILACAO COMPLETADA ***'

