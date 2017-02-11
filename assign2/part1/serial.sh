#!/bin/sh
#SBATCH --job-name=SLICHTEN_SERIAL   # Job name
#SBATCH --mail-type=ALL              # Mail events (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=slichtenheld@ufl.edu  # Where to send mail	
#SBATCH --ntasks=1                  # Number of MPI ranks
#SBATCH --cpus-per-task=1            # Number of cores per MPI rank 
#SBATCH --nodes=1                   # Number of nodes
#SBATCH --ntasks-per-node=1         # How many tasks on each node
#SBATCH --ntasks-per-socket=1       # How many tasks on each CPU or socket
#SBATCH --distribution=cyclic:cyclic # Distribute tasks cyclically on nodes and sockets
#SBATCH --mem-per-cpu=1gb            # Memory per processor
#SBATCH --time=00:1:00              # Time limit hrs:min:sec
#SBATCH --output=SERIAL%j.out     # Standard output and error log
pwd; hostname; date

echo "hw2_part_1_lichtenheld on $SLURM_JOB_NUM_NODES nodes with $SLURM_NTASKS tasks, each with $SLURM_CPUS_PER_TASK cores."

module load intel/2016.0.109 openmpi/1.10.2

srun --mpi=pmi2 serial -1.0 1.0 1000000

date