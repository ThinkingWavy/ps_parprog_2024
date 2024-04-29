#!/bin/bash

# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name benchmark_matrix_multi
# Redirect output stream to this file
#SBATCH --output=output.log
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=1
# Maximum number of tasks (=processes) to start per node
#SBATCH --ntasks-per-node=1
# Enforce exclusive node allocation, do not share with other jobs
#SBATCH --exclusive

#echo "Normal"
#for i in {1,2,4,6,12}
#do
#    for j in {1..10}
#        do 
#        ./ex1 1500 $i
#    done
#done

echo "Fast"
for i in {1,2,4,6,12}
do
    for j in {1..10}
        do 
        ./ex1-opt 1500 $i
    done
done