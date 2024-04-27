#!/bin/bash
#SBATCH -p shared
#SBATCH -t 0:30:00
#SBATCH -o analysis/data/%x.%j.csv
#SBATCH -e analysis/error/%x.%j.err
#SBATCH --cpus-per-task=2

THREADS=$SLURM_CPUS_PER_TASK
BLOCK_SIZE=1

square_resolutions="10 100 1000 10000 10000"
square_resolutions+=" 16 32 64 128 256 512 1024 2048 4096 8192 16384" 

for res in $square_resolutions; do
    performance_info=$(build/shared-fractals -p -x $res -y $res -o /dev/null -f tricorn)
    echo "$performance_info,$THREADS,$BLOCK_SIZE"
done
