#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_09.c -o exemplo_09
gcc -fopenmp exemplo_09_copyin.c -o exemplo_09_copyin

echo 'COMPILACAO OK'

