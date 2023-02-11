#!/usr/bin/env bash
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


MEM_GB=4
MAX_MINUTES=10
N_NODES=(10)
N_PROCESS=(1 4 16 25 64 100 400) # 4 16 25 64 100 400
MODES=("default" "pack" "scatter") # -l place=pack:excl, -l place=scatter:excl

#run multiple times with different configuration
i=0
n_runs=8

module load mpich-3.2
mpicc -g -Wall -o /home/matteo.greco/HPC_Project/parallel /home/matteo.greco/HPC_Project/parallel.c -lm
for test_file in $INPUT_DIR/*.txt; do
    filename=$(basename "$test_file" .txt) 
    dir_name="${filename}_out"
    #echo $out_file
    mkdir -p "benchmarks/${dir_name}"
    for PROCESS in ${N_PROCESS[@]}; do
        for NODES in ${N_NODES[@]}; do
            for MODE in ${MODES[@]}; do
                    #result=$("scale=2 ; $PROCESS / $NODES" | bc)
                    result=$(((PROCESS / NODES) + 1))
                    CPUS=$(printf "%.0f" $result)
                    out_file="${filename}_${MODE}_P${PROCESS}"
                for i in {0..3}; do 
                    # qsub run.sh <arguments>
                    #bash run.sh $NODES $CPUS $MEM_GB $MAX_MINUTES $MODE $PROCESS $filename $dir_name $out_file $i
                    cat >startJOB.sh <<EOL
#!/bin/bash
#PBS -l select=$NODES:ncpus=$CPUS:mem=${MEM_GB}gb

#PBS -l walltime=0:$MAX_MINUTES:00

#PBS -q short_cpuQ
#PBS -e /home/matteo.greco/HPC_Project/benchmarks/$dir_name/${out_file}_$i.stderr.log
#PBS -o /home/matteo.greco/HPC_Project/benchmarks/$dir_name/${out_file}_$i.stdout.log
if [[ "$MODE" != "default" ]]; then
    #PBS -l place=$MODE
    true
fi

module load mpich-3.2
mpirun.actual -n $((PROCESS + 1)) ./HPC_Project/parallel /home/matteo.greco/HPC_Project/inputs/$filename.txt $PROCESS /home/matteo.greco/HPC_Project/benchmarks/$dir_name/${out_file}_$i.txt
EOL
                    chmod +x startJOB.sh
                    N_JOBS=$(qstat |grep matteo.greco | grep "^.*$" -c)
                    while [[ $N_JOBS -gt 10 ]]; do
                        echo "max number of jobs submitted, waiting for some to finish"
                        sleep 10
                        N_JOBS=$(qstat |grep matteo.greco | grep "^.*$" -c)
                    done
                    qsub startJOB.sh
                done
            done
        done
    done
done    