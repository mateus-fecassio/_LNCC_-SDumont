#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_flush.c -o exemplo_flush

echo '*** COMPILACAO COMPLETADA ***'

