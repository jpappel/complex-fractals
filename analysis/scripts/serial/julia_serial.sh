#!/bin/bash
# SBATCH -p shared
# -t 2:30:00
# -o analysis/data/%x.%j.csv
# -e analysis/error/%x.%j.err

THREADS=1
BLOCK_SIZE=1

square_resolutions="10 100 1000 10000 10000"
square_resolutions+=" 16 32 64 128 256 512 1024 2048 4096 8192 16384" 

for res in $square_resolutions; do
    performance_info=$(build/serial-fractals -p -c −0.8+0.156i -r 4 -x $res -y $res -o /dev/null -f julia)
    echo "$performance_info,$THREADS,$BLOCK_SIZE"
done
