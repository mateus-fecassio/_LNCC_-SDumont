#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_04_05.c -o exemplo_04_05
gcc -fopenmp exemplo_04_1.c -o exemplo_04_1
gcc -fopenmp exemplo_04_1_private.c -o exemplo_04_1_private
gcc -fopenmp exemplo_04_2.c -o exemplo_04_2
gcc -fopenmp exemplo_04_3.c -o exemplo_04_3
gcc -fopenmp exemplo_04_4.c -o exemplo_04_4
gcc -fopenmp exemplo_04.c -o exemplo_04
gcc -fopenmp exemplo_04_firstprivate.c -o exemplo_04_firstprivate
gcc -fopenmp exemplo_04_lastprivate.c -o exemplo_04_lastprivate
gcc -fopenmp exemplo_04_ordered.c -o exemplo_04_ordered
gcc -fopenmp exemplo_04_private.c -o exemplo_04_private
gcc -fopenmp exemplo_4_omp_dynamic.c -o exemplo_4_omp_dynamic

echo '*** COMPILACAO OK ***'

