#!/bin/bash
#SBATCH --job-name=exercise1     # create a short name for your job
#SBATCH --nodes=1                # node count
#SBATCH --ntasks=1               # total number of tasks across all nodes
#SBATCH --cpus-per-task=1        # cpu-cores per task (>1 if multi-threaded tasks)
#SBATCH --mem-per-cpu=4G         # memory per cpu-core (4G is default)
#SBATCH --time=00:01:00          # total run time limit (HH:MM:SS)

module load openmpi/4.1.5
module load quantum-espresso/7.2

pw.x < Si.sample.in > Si.sample.out
