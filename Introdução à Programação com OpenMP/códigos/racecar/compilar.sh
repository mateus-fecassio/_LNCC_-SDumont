#!/bin/bash

module load gcc-5

gcc -fopenmp racecar_omp.c -o racecar_omp
gcc -fopenmp racecar_single_two_task.c -o racecar_single_two_task
gcc -fopenmp racecar_task_wait.c -o racecar_task_wait
gcc -fopenmp racecar.c -o racecar
gcc -fopenmp racecar_omp_single.c -o racecar_omp_single
gcc -fopenmp racecar_task.c -o racecar_task

echo '*** COMPILACAO COMPLETADA ***'

