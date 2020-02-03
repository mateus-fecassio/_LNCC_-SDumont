#!/bin/bash

   source /scratch/app/modulos/intel-psxe-2017.1.043.sh

   mpiicc teste01.c -o teste01
   mpiicc teste02.c -o teste02
   mpiicc teste03.c -o teste03
   mpiicc teste04.c -o teste04
   mpiicc teste05.c -o teste05
   mpiicc teste06.c -o teste06
   mpiicc teste07.c -o teste07
   mpiicc teste08.c -o teste08
   mpiicc teste09.c -o teste09  
   mpiicc teste10.c -o teste10
   mpiicc teste11.c -o teste11
   mpiicc calc_pi_MPI.c -o calc_pi_MPI

echo "  **** COMPILACAO COMPLETA ****  " 
