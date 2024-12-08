#!/bin/bash

cd "$(dirname "$0")"
BIN=../../bin
SRC=../../src
FILE="${SRC}/data/timing_results_depth_5.csv"
CMD="${BIN}/TimingTests 5"

> $FILE
echo "num_threads,trial,time,num_nodes" | tee $FILE
for num_threads in $(seq 1 32);
do
    export OMP_NUM_THREADS=$num_threads
    for trial in $(seq 1 3);
    do
        (echo -n "$num_threads,$trial,"; eval $CMD) | tee -a $FILE
    done
done
