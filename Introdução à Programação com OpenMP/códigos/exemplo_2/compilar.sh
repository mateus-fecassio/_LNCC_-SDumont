#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_02.c -o exemplo_02
gcc -fopenmp exemplo_02_get_nested.c -o exemplo_02_get_nested

echo '*** COMPILACAO COMPLETADA ***'

