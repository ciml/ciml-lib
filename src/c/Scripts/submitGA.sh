#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

for ((j = $1; j <= $2; j++))
do
  qsub -q all.q@compute-0-14.local,all.q@compute-0-15.local,all.q@compute-0-9.local /$SCRIPTPATH/jobGA.sh ta$j
done
