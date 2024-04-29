#!/bin/bash

# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name=exc1_given
# Redirect output stream to this file
#SBATCH --output=results/%x.log
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=1
# Maximum number of tasks (=processes) to start per node
#SBATCH --ntasks-per-node=1
# Enforce exclusive node allocation, do not share with other jobs
#SBATCH --exclusive

# Check if there's an argument provided

executables=("/scratch/cb761139/ps_parprog_2024/05/ex1/ex1_given" "/scratch/cb761139/ps_parprog_2024/05/ex1/ex1_opt")

# Define the number of threads
threads=(1)

# Loop through the number of threads and execute the program
for executable in "${executables[@]}"; do
    for t in "${threads[@]}"; do
        export OMP_NUM_THREADS=$t
        echo "Executing $executable with $t threads"
        $executable 500
        echo ""
    done
done

