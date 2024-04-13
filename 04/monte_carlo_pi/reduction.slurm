#!/bin/bash

# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name=mc_pi_reduction
# Redirect output stream to this file
#SBATCH --output=results/%x.out
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=1
# Maximum number of tasks (=processes) to start per node
#SBATCH --ntasks-per-node=1
# Enforce exclusive node allocation, do not share with other jobs
#SBATCH --exclusive

# Check if there's an argument provided

executable="/scratch/cb761139/ps_parprog_2024/04/monte_carlo_pi/mc_pi_reduction"

# Define the number of threads
threads=(1 2 4 6 12)

# Loop through the number of threads and execute the program
for t in "${threads[@]}"; do
    export OMP_NUM_THREADS=$t
    echo "Executing $executable with $t threads"
    /usr/bin/time $executable
    echo ""
done