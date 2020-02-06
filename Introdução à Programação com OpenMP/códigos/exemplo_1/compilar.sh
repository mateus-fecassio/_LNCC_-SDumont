#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_01.c -o exemplo_01
gcc -fopenmp exemplo_01_num_threads.c -o exemplo_01_num_threads
gcc -fopenmp exemplo_01_parallel.c -o exemplo_01_parallel
gcc -fopenmp exemplo_01_set_num_threads.c -o exemplo_01_set_num_threads

echo 'COMPILACAO COMPLETADA'

