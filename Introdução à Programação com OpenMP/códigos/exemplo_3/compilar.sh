#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_03.c -o exemplo_03

echo '*** COMPILACAO COMPLETADA ***'

