#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_06_1.c -o exemplo_06_1
gcc -fopenmp exemplo_06_1_nowait.c -o exemplo_06_1_nowait
gcc -fopenmp exemplo_06_2.c -o exemplo_06_2
gcc -fopenmp exemplo_06_3.c -o exemplo_06_3
gcc -fopenmp exemplo_06.c -o exemplo_06

echo '*** COMPILACAO COMPLETADA ***'

