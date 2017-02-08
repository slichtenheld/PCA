#!/bin/sh
#SBATCH --job-name=hw2_part1_hassan      # Job name
#SBATCH --mail-type=ALL              # Mail events (NONE, BEGIN, END, FAIL, ALL)
#SBATCH --mail-user=khaledjhassan@ufl.edu  # Where to send mail	
#SBATCH --ntasks=32                  # Number of MPI ranks
#SBATCH --cpus-per-task=1            # Number of cores per MPI rank 
#SBATCH --nodes=8                    # Number of nodes
#SBATCH --ntasks-per-node=4          # How many tasks on each node
#SBATCH --ntasks-per-socket=2        # How many tasks on each CPU or socket
#SBATCH --distribution=cyclic:cyclic # Distribute tasks cyclically on nodes and sockets
#SBATCH --mem-per-cpu=1gb            # Memory per processor
#SBATCH --time=00:30:00              # Time limit hrs:min:sec
#SBATCH --output=hw2_part1_hassan_%j.out     # Standard output and error log
pwd; hostname; date

module load intel/2017.1.132 openmpi/2.0.1

mpirun hw2_part1_hassan -1.0 1.0 1000000

date
