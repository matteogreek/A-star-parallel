#!/bin/bash

# Script for submitting the job to the cluster with selected parameters to perform benchmarks.

mkdir benchmarks

path_parallel="./parallel.sh"

#data dir path passed as argument
INPUT_DIR="inputs"

#check if data directory exists
if [[ ! -d $INPUT_DIR ]]; then
  echo "dir not found"
  exit 1
fi

#set mem and walltime


MEM_GB=2
MAX_MINUTES=10
N_NODES=(1 2 4)
N_PROCESS=(4 16 25 64 100 400)
MODES=("d" "pack" "scatter") # -l place=pack:excl, -l place=scatter:excl

#run multiple times with different configuration
i=0
n_runs=8

for test_file in $INPUT_DIR/*.txt; do
    filename=$(basename "$test_file" .txt) 
    dir_name="${filename}_out"
    #echo $out_file
    mkdir -p "benchmarks/${dir_name}"
    for PROCESS in ${N_PROCESS[@]}; do
        for NODES in ${N_NODES[@]}; do
            for MODE in ${MODES[@]}; do
                    #result=$("scale=2 ; $PROCESS / $NODES" | bc)
                    result=$(((PROCESS + 1) / NODES))
                    CPUS=$(printf "%.0f" $result)
                    out_file="${filename}_${MODE}_C${CPUS}_N${NODES}"
                for i in {0..8}; do 
                    # qsub run.sh <arguments>
                    bash run.sh $NODES $CPUS $MEM_GB $MAX_MINUTES $MODE $PROCESS $filename $dir_name $out_file $i
                done
            done
        done
    done
done    