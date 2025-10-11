#!/bin/bash
#SBATCH --job-name=kendallbenchmarkpacha
#SBATCH --output=/scratch/s/shirimb/msep/kendall/kendallbenchmark.txt
#SBATCH --nodes=1
#SBATCH --ntasks=40
#SBATCH --time=12:00:00
#SBATCH --account=def-shirimb

# Load necessary modules
module load gcc/8.3.0
module load r/4.2.2-batteries-included

export MKLROOT=/gpfs/fs1/scinet/intel/2020u2/compilers_and_libraries_2020.2.254/linux/mkl
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MKLROOT/lib/intel64_lin
export R_LIBS_USER=/scratch/s/shirimb/msep/kendall/rpkgs

# Run the script
Rscript replication.r
