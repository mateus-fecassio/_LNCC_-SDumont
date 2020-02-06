#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_05.c -o exemplo_05

echo '*** COMPILACAO COMPLETADA ***'

