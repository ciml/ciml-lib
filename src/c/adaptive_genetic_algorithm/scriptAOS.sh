#!/bin/bash
tpop=("50" "76" "100")
#pcro=("0.7" "0.8" "0.9")
#pmut=("0.3" "0.4" "0.5")
for ((j = 0; j < 3; j++))
do
  for i in {1..5}
  do
    /home/guilherme.ferreira/Adaptive_Pursuit/aga_jsp /home/guilherme.ferreira/Adaptive_Pursuit/$1 $i ${tpop[$j]} > saidascript
  done
done
