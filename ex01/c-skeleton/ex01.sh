#!/bin/bash -l
#
#SBATCH --nodes=1
#SBATCH --time=0:05:00
#SBATCH --export=NONE

unset SLURM_EXPORT_ENV

module load intel

make distclean

make 

srun ./exe-ICC sanju

make distclean