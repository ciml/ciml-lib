#!/bin/bash
tpop=("50" "76" "100")
pcro=("0.7" "0.8" "0.9")
pmut=("0.3" "0.4" "0.5")
pls=("0.015" "0.03" "0.045")

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

for ((j = 0; j < 3; j++))
do
  for ((k = 0; k < 3; k++))
  do
    for ((l = 0; l < 3; l++))
    do
      for((m = 0; m < 3; m++))
      do
        for i in {1..2}
        do
          $SCRIPTPATH/aga_jsp $SCRIPTPATH/$1 $i ${tpop[$j]} ${pcro[$k]} ${pmut[$l]} ${pls[$m]}
        done
      done
    done
  done
done
