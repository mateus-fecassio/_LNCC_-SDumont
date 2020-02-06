#!/bin/bash

module load gcc-5

gcc -fopenmp exemplo_11.c -o exemplo_11

echo 'COMPILACAO OK'

