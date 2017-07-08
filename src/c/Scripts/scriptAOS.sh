#!/bin/bash
tpop=("50" "76" "100")

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

for ((j = 0; j < 3; j++))
do
  for i in {1..2}
  do
    $SCRIPTPATH/aga_jsp $SCRIPTPATH/$1 $i ${tpop[$j]}
  done
done
