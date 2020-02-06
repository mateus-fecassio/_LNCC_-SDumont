#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_10.c -o exemplo_10

echo 'COMPILACAO OK'

