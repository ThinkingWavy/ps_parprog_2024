#!/bin/bash
# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name=static
# Redirect output stream to this file
#SBATCH --output=results/%x.log
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=1
# Maximum number of tasks (=processes) to start per node
#SBATCH --ntasks-per-node=1
# Enforce exclusive node allocation, do not share with other jobs
#SBATCH --exclusive

for n in {10000,20000,30000}
do 
    for threads in {1,2,4,6,12}
    do
        for i in {1..10}
        do  
            export OMP_NUM_THREADS=$threads
            ./hadamard_static $n
        done
    done
done
