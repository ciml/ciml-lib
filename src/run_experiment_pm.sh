#$ -S /bin/bash
#$ -q linux.q
#----------------------------------------------------------
# Parameters:
# 1- seed
#----------------------------------------------------------

problema=(alu4.ep cordic.ep t481.ep vda.ep)
evals=(8400000 18400000 6400000 10200000)
ncols=(7763 20769 7151 8135)
ngates=(5175 13846 4767 5423)
mediangen=(10200000 18400000 6400000 10200000)

for i in `seq 0 3`;
do
    #echo "Solving problem:${problema[i]} with SAM - seed:${1}"
    echo "$(./bin/cgp tables/${problema[i]} seed=${1} ncol=$((ncols[i])) maxgen=$((evals[i])) mutation=3 ngates=$((ngates[i])) mediangen=$((mediangen[i])) results/pm/${problema[i]}_seed${1})"
done




