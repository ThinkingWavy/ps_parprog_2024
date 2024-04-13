#!/bin/bash

# Execute job in the partition "lva" unless you have special requirements.
#SBATCH --partition=lva
# Name your job to be able to identify it later
#SBATCH --job-name hadamard2
# Redirect output stream to this file
#SBATCH --output=results/%x.out
# Maximum number of tasks (=processes) to start in total
#SBATCH --ntasks=1
# Maximum number of tasks (=processes) to start per node
#SBATCH --ntasks-per-node=1
# Enforce exclusive node allocation, do not share with other jobs
#SBATCH --exclusive

echo "perf"
perf stat -e LLC-load-misses -e LLC-store-misses ./hadamard2


echo ""
echo "cachegrind"
valgrind --tool=cachegrind ./hadamard2