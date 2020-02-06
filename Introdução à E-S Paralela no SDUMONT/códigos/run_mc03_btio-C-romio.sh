#!/bin/bash
#SBATCH --nodes=2                      # here the number of nodes
#SBATCH --ntasks=36                    # here total number of mpi tasks
#SBATCH --ntasks-per-node=18           # here ppn = number of process per nodes
#SBATCH -p treinamento                 # target partition
#SBATCH -J NPB-BTIO-C-36               # job name
#SBATCH --exclusive                    # to have exclusvie use of your nodes


echo $SLURM_JOB_NODELIST
cd $SLURM_SUBMIT_DIR

echo ""
echo "**********"
echo ""
echo ""
echo ""


###################################
#           COMPILER              #
###################################
module load openmpi/gnu/2.0.4.2

#Configure to use the ROMIO MPI-IO implementation
#TODO

#Configure to use the $SCRATCH/mc03/hints.txt file hints
#TODO

#Configure to show the used hints 
#TODO

#Configure BTIO executable
EXEC=/scratch/app/NPB3.3.1-MZ/bin/bt.C.36.mpi_io_full.ompi20+gnu

#Start the benchmark
srun $EXEC
