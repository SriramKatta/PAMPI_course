#!/bin/bash -l

rm -rf size_*

for i in 50 100 150
do 
    for omg in 1.2 1.5 1.7 1.9
    do 
        for proc in 1 4 8 16 32 64 72 
        do 
            mkdir -p ./size_$i/omg_$omg/proc_$proc
            cp poisson.par ./size_$i/omg_$omg/proc_$proc
            cd ./size_$i/omg_$omg/proc_$proc
            sed -i "s/^\( *imax *\)[0-9]\+/\1$i/" "poisson.par"
            sed -i "s/^\( *jmax *\)[0-9]\+/\1$i/" "poisson.par"
            sed -i "s/^\( *omg *\)[0-9]\+\(\.[0-9]*\)\?/\1$omg/" "poisson.par"
            cd ../../..
        done
    done
done
