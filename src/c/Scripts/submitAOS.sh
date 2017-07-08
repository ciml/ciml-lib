#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

for ((j = $1; j <= $2; j++))
do
  qsub -q all.q@compute-0-18.local /$SCRIPTPATH/jobAOS.sh ta$j
done

