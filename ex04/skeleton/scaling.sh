#!/bin/bash -l
#
#SBATCH --job-name=poission_internode
#SBATCH --output=Fritz_ICX_poission_internode
#SBATCH --partition=singlenode
#SBATCH --time=10:00:00
#SBATCH --export=NONE
#SBATCH --cpu-freq=2400000-2400000:performance

unset SLURM_EXPORT_ENV

module load likwid intel intelmpi

make distclean
make

echo "#process vs time" > scaling_results

for proccount in {1..72}
do 
    time=$(likwid-mpirun -mpi slurm -n $proccount \
                            -nperdomain N:72 \
                            ./exe-ICC poisson.par \
                            | grep -i "walltime" \
                            | cut -d ' ' -f 2)
    echo $proccount $time >> scaling_results
done
make distclean

