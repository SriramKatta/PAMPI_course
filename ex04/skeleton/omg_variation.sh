#!/bin/bash -l
#
#SBATCH --job-name=poission_omg_var_internode
#SBATCH --output=Fritz_ICX_poission_omg_var_internode
#SBATCH --partition=singlenode
#SBATCH --time=10:00:00
#SBATCH --export=NONE
#SBATCH --cpu-freq=2400000-2400000:performance

unset SLURM_EXPORT_ENV

module load likwid intel intelmpi

make distclean
make

./possio_filecreator.sh


echo "#size, omg, processor count, iter count,residual" > resfile_omgvar

for i in 50 100 150
do 
    for omg in 1.2 1.5 1.7 1.9
    do 
        for proc in 1 4 8 16 32 64 72 
        do 
        mpirun -n $proc ./exe-ICC ./size_$i/omg_$omg/proc_$proc/poisson.par > out
        residual=$(tail -n 2 out | head -n 1 | awk '{print $(NF-2)}')
        iter=$(tail -n 2 out | head -n 1 | awk '{print $(3)}')

        echo "$i $omg $proc $iter $residual" >> resfile_omgvar
        echo "$i $omg $proc $iter $residual done"
done
done
done

make distclean

