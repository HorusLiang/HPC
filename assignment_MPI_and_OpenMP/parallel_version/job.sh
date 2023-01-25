#!/bin/bash
#SBATCH --job-name="pi"
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16
#SBATCH --time=00:10:00
#SBATCH --partition=normal
#SBATCH --output=pi_%j.out
#SBATCH --error=pi_%j.err

module purge
module load 2020
module load GCC

echo "OpenMP parallelism"
echo

for ncores in {24}

do
  export OMP_NUM_THREADS=$ncores

  echo "CPUS: " $OMP_NUM_THREADS
  echo "CPUS: " $OMP_NUM_THREADS >&2
  ./simple_try
  echo "DONE "
done