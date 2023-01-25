#!/bin/bash
#SBATCH --job-name="game"
#SBATCH --time=00:10:00
#SBATCH --partition=normal
#SBATCH --output=pi_%j.out
#SBATCH --error=pi_%j.err

module purge
module load 2020
module load GCC

echo "OpenMP and MPI parallelism"
echo

for nodes in 1 2; do
  for tasks in 3 4; do
    for cpus in 1 2; do
      # Set the values for the job
      #SBATCH --nodes=$nodes
      #SBATCH --ntasks=$tasks
      #SBATCH --cpus-per-task=$cpus

      export OMP_NUM_THREADS=$cpus

      echo "Nodes: " $nodes " Tasks: " $tasks " THREADS: " $OMP_NUM_THREADS " CPU: " $cpus
      echo "Nodes: " $nodes " Tasks: " $tasks " THREADS: " $OMP_NUM_THREADS " CPU: " $cpus >&2
      ./simple_try
      echo "DONE"
    done
  done
done
