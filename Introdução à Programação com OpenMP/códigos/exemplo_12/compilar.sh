#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_12.c -o exemplo_12

echo '*** COMPILACAO COMPLETADA ***'

