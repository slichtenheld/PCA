
#!/bin/bash
#SBATCH --job-name=<JOBNAME>
##SBATCH --mail-user=<EMAIL>
#SBATCH --mail-type=FAIL,END
#SBATCH --output <my_job-%j.out>
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=4gb
#SBATCH --time=72:00:00
#SBATCH --account=<GROUP>
#SBATCH --array=<BEGIN-END>
date;hostname;pwd
module load ncbi_blast

blastn -db nt -query input.fa -outfmt 6 -out results.xml --num_threads 4

date
