#!/bin/bash
#SBATCH --job-name="game"
#SBATCH --time=00:10:00
#SBATCH --partition=normal
#SBATCH --output=pi_%j.out
#SBATCH --error=pi_%j.err
#SBATCH --nodes=16




module purge
module load 2020
module load GCC

echo "OpenMP and MPI parallelism"
echo

for nodes in 16; do
  for tasks in 16; do
    for cpus in 4; do
      # Set the values for the job
      if [ $((tasks*cpus)) -le $((nodes*16)) ] && [ $tasks -ge $nodes ]; then
        export SLURM_NODES=$nodes
        export SLURM_NTASKS=$tasks
        export SLURM_CPUS_PER_TASK=$cpus

        export OMP_NUM_THREADS=$cpus

        echo "Nodes: " $nodes " Tasks: " $tasks " THREADS: " $OMP_NUM_THREADS " CPU: " $cpus
        echo "Nodes: " $nodes " Tasks: " $tasks " THREADS: " $OMP_NUM_THREADS " CPU: " $cpus >&2
        mpirun --npernode $SLURM_CPUS_PER_TASK -np $SLURM_NTASKS  ./second_try
        echo "DONE"
      fi
    done
  done
done

