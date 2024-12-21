#!/bin/bash

# Define test parameters
BOARD_POS=${1:-0} # 0: early game, 1: end game
DEPTH=${2:-5}
echo "Using board position ${BOARD_POS} and depth ${DEPTH}..."

# Execute test
cd "$(dirname "$0")"
BIN=../../bin
SRC=../../src
FILE="${SRC}/data/timing_results_${BOARD_POS}_${DEPTH}.csv"
CMD="${BIN}/TimingTests ${BOARD_POS} ${DEPTH}"

> $FILE
echo "num_threads,trial,time_shared,num_nodes_shared,correct_shared,time_local,num_nodes_local,correct_local,time_blended,num_nodes_blended,correct_local,pos_idx" | tee $FILE
for num_threads in $(seq 1 32);
do
    export OMP_NUM_THREADS=$num_threads
    for trial in $(seq 1 3);
    do
        (echo -n "$num_threads,$trial,"; eval $CMD) | tee -a $FILE
    done
done
