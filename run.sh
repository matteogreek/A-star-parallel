#!/bin/bash

select=$1
cpu=$2
mem=$3
time=$4
strat=$5
workers=$6
nodes=$((workers+1))
input=$7
out_dir=$8
out_name=$9
run=${10}


#PBS -l select=$select:ncpus=$cpu:mem=${mem}gb

#PBS -l walltime=0:$time:00

if [[ "$strat" != "d" ]]; then
    echo "PBS -l place=$strat"
fi
#PBS -q short_cpuQ

#module load mpich-3.2
#mpicc -g -Wall -o HPC_Project/parallel HPC_Project/MPI_parallel.c -lm
echo "mpirun.actual -n $nodes ./HPC_Project/parallel /home/nicola.marchioro/HPC_Project/inputs/$input.txt $workers /home/nicola.marchioro/HPC_Project/$out_dir/${out_name}_$run.txt"