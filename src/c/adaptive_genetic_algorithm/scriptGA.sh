#!/bin/bash
tpop=("50" "76" "100")
pcro=("0.7" "0.8" "0.9")
pmut=("0.3" "0.4" "0.5")

for ((j = 0; j < 3; j++))
do
  for ((k = 0; k < 3; k++))
  do
    for ((l = 0; l < 3; l++))
    do
      for i in {1..5}
      do
        /home/guilherme.ferreira/Default_GA/aga_jsp /home/guilherme.ferreira/Default_GA/$1 $i ${tpop[$j]} ${pcro[$k]} ${pmut[$l]}
      done
    done
  done
done
